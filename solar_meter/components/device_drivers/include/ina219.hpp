/**
 *******************************************************************************
 * @file    ina219.hpp
 * @author  HQ
 * @date    2025-08-22 19:49:06
 * @brief   
 *******************************************************************************
 */

#ifndef INA219_HPP
#define INA219_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "ina219_regs.hpp"
#include "i2c_device.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class INA219 : public I2CDevice
{
public:
    INA219(I2CBus & bus, INA219_Address address = INA219_Address::Device1);
    ~INA219();

    Status_t configure(const INA219_Config_t & config);
    Status_t triggerSingleMeasurement(INA219_OperatingMode_t mode);
    Status_t readShuntAndBusVoltage(float & shuntVoltage_mV, float & busVoltage_V);
    Status_t readCurrentAndBusVoltage(float & current_mA, float & busVoltage_V);




private:
    Status_t configObjToBytes(const INA219_Config_t & configObj, uint8_t * bytes);
    Status_t readShuntVoltage(float & shuntVoltage_mV);
    Status_t readCurrent(float & current_mA);
    Status_t readBusVoltage(float & busVoltage_V);

    INA219_Address address;
    INA219_Config_t config; // To store the current configuration of the INA219
    I2CTransfer_t txObj;
    I2CTransfer_t rxObj;
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // INA219_HPP