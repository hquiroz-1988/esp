/**
 *******************************************************************************
 * @file    i2c_device.hpp
 * @author  HQ
 * @date    2025-08-22 13:22:36
 * @brief   
 *******************************************************************************
 */

#ifndef I2C_DEVICE_HPP
#define I2C_DEVICE_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "i2c_typedefs.hpp"
#include "i2c_bus.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/

/* forward declaration of I2CBus */
class I2CBus;

class I2CDevice
{
    friend class I2CBus;

    public:
    I2CDevice(I2CBus &_bus);
    virtual ~I2CDevice();


    protected:
    /* accessible by inherited classes */
    Status_t write(I2CTransfer_t &transfer);
    Status_t read(I2CTransfer_t &transfer);

    private:
    I2CBus &i2cBus;
    int deviceId{-1};
    bool busAcquired{false};

    Status_t acquireBus(void);
    Status_t releaseBus(void);
    Status_t writeToBus(I2CTransfer_t &transfer);
    Status_t readFromBus(I2CTransfer_t &transfer);
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // I2C_DEVICE_HPP