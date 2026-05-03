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
        .mode = INA219_OperatingMode_t::ADCOff
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

Status_t INA219::triggerSingleMeasurement(INA219_OperatingMode_t mode)
{
    Status_t statusRet = STATUS_OKAY;
    uint8_t configBytes[static_cast<uint8_t>(INA219_RegisterSize::Config)] = {0};
    I2CTransfer_t txObj = {0};

    // Create a temporary config object with the new mode
    this->config.mode = mode;

    statusRet = configObjToBytes(this->config, configBytes);

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

    return statusRet;
}

Status_t INA219::readShuntVoltage(float &shuntVoltage_mV)
{
    Status_t statusRet = STATUS_OKAY;
    uint8_t shuntBytes[static_cast<uint8_t>(INA219_RegisterSize::ShuntVoltage)] = {0};
    I2CTransfer_t rxObj = {0};

    rxObj.data    = shuntBytes;
    rxObj.size    = static_cast<size_t>(INA219_RegisterSize::ShuntVoltage);
    rxObj.devAddr = static_cast<uint8_t>(address);
    rxObj.regAddr = static_cast<uint8_t>(INA219_Register::ShuntVoltage);
    rxObj.ackEn   = INA219_ACK_CHECK_STATUS;
    rxObj.ackType = I2CTransferAckType_t::MASTER_LAST_NACK;

    statusRet = read(rxObj);

    if (statusRet == STATUS_OKAY)
    {
        /* 16-bit signed, 10 µV/LSB → convert to mV */
        const int16_t shuntRaw = static_cast<int16_t>((static_cast<uint16_t>(shuntBytes[0]) << 8) | shuntBytes[1]);
        shuntVoltage_mV = static_cast<float>(shuntRaw) * INA219_SHUNT_VOLTAGE_LSB_UV / 1000.0f;
    }

    return statusRet;
}


Status_t INA219::readCurrent(float &current_mA)
{
    Status_t statusRet = STATUS_OKAY;
    uint8_t currentBytes[static_cast<uint8_t>(INA219_RegisterSize::Current)] = {0};
    I2CTransfer_t rxObj = {0};

    rxObj.data    = currentBytes;
    rxObj.size    = static_cast<size_t>(INA219_RegisterSize::Current);
    rxObj.devAddr = static_cast<uint8_t>(address);
    rxObj.regAddr = static_cast<uint8_t>(INA219_Register::Current);
    rxObj.ackEn   = INA219_ACK_CHECK_STATUS;
    rxObj.ackType = I2CTransferAckType_t::MASTER_LAST_NACK;

    statusRet = read(rxObj);

    if (statusRet == STATUS_OKAY)
    {
        /* 16-bit signed, current LSB depends on calibration (not handled here) */
        const int16_t currentRaw = static_cast<int16_t>((static_cast<uint16_t>(currentBytes[0]) << 8) | currentBytes[1]);
        current_mA = static_cast<float>(currentRaw); // Placeholder: actual conversion requires calibration
    }

    return statusRet;
}


Status_t INA219::readBusVoltage(float &busVoltage_V)
{
    Status_t statusRet = STATUS_OKAY;
    uint8_t busBytes[static_cast<uint8_t>(INA219_RegisterSize::BusVoltage)] = {0};
    I2CTransfer_t rxObj = {0};

    rxObj.data    = busBytes;
    rxObj.size    = static_cast<size_t>(INA219_RegisterSize::BusVoltage);
    rxObj.devAddr = static_cast<uint8_t>(address);
    rxObj.regAddr = static_cast<uint8_t>(INA219_Register::BusVoltage);
    rxObj.ackEn   = INA219_ACK_CHECK_STATUS;
    rxObj.ackType = I2CTransferAckType_t::MASTER_LAST_NACK;

    statusRet = read(rxObj);

    if (statusRet == STATUS_OKAY)
    {
        /* bits [15:3], 4 mV/LSB → convert to V */
        const uint16_t busRaw = static_cast<uint16_t>((static_cast<uint16_t>(busBytes[0]) << 8) | busBytes[1]);
        busVoltage_V = static_cast<float>(busRaw >> INA219_BD_SHIFT) * INA219_BUS_VOLTAGE_LSB_MV / 1000.0f;
    }

    return statusRet;
}

Status_t INA219::readShuntAndBusVoltage(float &shuntVoltage_mV, float &busVoltage_V)
{
    Status_t statusRet = readShuntVoltage(shuntVoltage_mV);

    if (statusRet == STATUS_OKAY)
    {
        statusRet = readBusVoltage(busVoltage_V);
    }

    return statusRet;
}

Status_t INA219::readCurrentAndBusVoltage(float &current_mA, float &busVoltage_V)
{
    Status_t statusRet = readCurrent(current_mA);

    if (statusRet == STATUS_OKAY)
    {
        statusRet = readBusVoltage(busVoltage_V);
    }

    return statusRet;
}

