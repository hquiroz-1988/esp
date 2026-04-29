/**
 *******************************************************************************
 * @file    ina219.cpp
 * @author  HQ
 * @date    2025-08-22
 * @brief   INA219 stub implementation
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
 *******************************************************************************/
#include "ina219.hpp"

/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS
 *******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PROTECTED FUNCTIONS
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTIONS
 *******************************************************************************/
Status_t INA219::configObjToBytes(const INA219_Config_t &configObj, uint8_t *bytes)
{
    Status_t statusRet = STATUS_OKAY;

    if (bytes != nullptr)
    {
        const uint16_t configWord =
            ((static_cast<uint16_t>(configObj.busVoltageRange) & INA219_BRNG_CFG_MASK) << INA219_BRNG_CFG_BIT) |
            ((static_cast<uint16_t>(configObj.pgaGain) & INA219_PG_CFG_MASK) << INA219_PG_CFG_BITS) |
            ((static_cast<uint16_t>(configObj.busADC) & INA219_BADC_CFG_MASK) << INA219_BADC_CFG_BITS) |
            ((static_cast<uint16_t>(configObj.shuntADC) & INA219_SADC_CFG_MASK) << INA219_SADC_CFG_BITS) |
            ((static_cast<uint16_t>(configObj.mode) & INA219_MODE_CFG_MASK) << INA219_MODE_CFG_BITS);

        bytes[0] = static_cast<uint8_t>(configWord >> 8);
        bytes[1] = static_cast<uint8_t>(configWord & 0x00FFu);
    }
    else
    {
        statusRet = STATUS_NULL_POINTER;
    }

    return statusRet;
}

/*******************************************************************************
 * PUBLIC FUNCTIONS
 *******************************************************************************/

INA219::INA219(I2CBus & bus, INA219_Address address)
    : I2CDevice(bus), address(address)
{
    const INA219_Config_t defaultConfig =
    {
        .busVoltageRange = INA219_BusVoltageRange_t::FSR_32V,
        .pgaGain = INA219_PGAGain_t::Gain_8_320mV,
        .busADC = INA219_ADCResolution_t::Res_12bit,
        .shuntADC = INA219_ADCResolution_t::Res_12bit,
        .mode = INA219_OperatingMode_t::ShuntBus_Cont
    };

    (void)configure(defaultConfig);
}

INA219::~INA219()
{
    //! TODO: implement destructor
}

Status_t INA219::configure(const INA219_Config_t &config)
{
    Status_t statusRet = STATUS_OKAY;
    uint8_t configBytes[static_cast<uint8_t>(INA219_RegisterSize::Config)] = {0};
    I2CTransfer_t txObj = {0};

    statusRet = configObjToBytes(config, configBytes);

    if (statusRet == STATUS_OKAY)
    {
        txObj.data = configBytes;
        txObj.size = static_cast<size_t>(INA219_RegisterSize::Config);
        txObj.devAddr = static_cast<uint8_t>(address);
        txObj.regAddr = static_cast<uint8_t>(INA219_Register::Config);
        txObj.ackEn = INA219_ACK_CHECK_STATUS;
        txObj.ackType = I2CTransferAckType_t::MASTER_LAST_NACK;

        statusRet = write(txObj);
    }

    if (statusRet == STATUS_OKAY)
    {
        this->config = config;
    }

    return statusRet;
}

