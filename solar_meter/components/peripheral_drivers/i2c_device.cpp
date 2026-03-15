/**
 *******************************************************************************
 * @file    i2c_device.cpp
 * @author  HQ
 * @date    2025-08-30 00:52:24
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
extern "C"
{

}
#include "esp_log.h"
#include "i2c_device.hpp"


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
 * STATIC FUNCTION PROTOTYPES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTIONS
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/
I2CDevice::I2CDevice()
{
    // Constructor implementation (if needed)
}

I2CDevice::~I2CDevice()
{
    // Destructor implementation (if needed)
}

Status_t I2CDevice::addBus(I2CBus *_i2cBus, int _deviceId) 
{
    Status_t status = STATUS_OKAY;

    if(_i2cBus == nullptr)
    {
        status = STATUS_NULL_POINTER;
    }

    if( status == STATUS_OKAY 
        && ( (_deviceId < 0) || (_deviceId >= I2CBus::MAX_DEV_COUNT) ) )
    {
        status = STATUS_OUT_OF_BOUNDS;
    }

    if(status == STATUS_OKAY)
    {
        /* add i2c bus and our device id to access the bus */
        i2cBus = _i2cBus;
        deviceId = _deviceId;
    }

    return status;
}


Status_t I2CDevice::write(I2CTransfer_t &transfer)
{
    Status_t status = STATUS_OKAY;

    status = acquireBus();

    if(status == STATUS_OKAY)
    {
        /* write to bus if successfully acquired */
        status = writeToBus(transfer);

        /* if bus was acquired we need to release before exiting */
        releaseBus();
    }

    return status;
}

Status_t I2CDevice::read(I2CTransfer_t & transfer)
{
    Status_t status = STATUS_OKAY;

    status = acquireBus();


    if(status == STATUS_OKAY)
    {
        /* read from bus if successfully acquired */
        status = readFromBus(transfer);

        /* if bus was acquired we need to release before exiting */
        releaseBus();
    }

    return status;
}


Status_t I2CDevice::acquireBus(void)
{
    Status_t status = STATUS_OKAY;

    if(i2cBus != nullptr)
    {
        /* acquire bus from i2c bus class */
        status = i2cBus->acquire(deviceId, portMAX_DELAY);

        if(status == STATUS_OKAY)
        {
            busAcquired = true;
        }
    }
    else
    {
        status = STATUS_NULL_POINTER;
    }
    
    return status;
}

Status_t I2CDevice::releaseBus(void)
{
    Status_t status = STATUS_OKAY;

    if(i2cBus != nullptr)
    {
        /* release bus from i2c bus class */
        status = i2cBus->release(deviceId);
    }
    else
    {
        status = STATUS_NULL_POINTER;
    }

    /* we either never had the bus or just released it*/
    busAcquired = false;
    
    return status;
}

Status_t I2CDevice::writeToBus(I2CTransfer_t &transfer)
{
    Status_t status = STATUS_OKAY;

    if(i2cBus != nullptr)
    {
        if(busAcquired)
        {
            /* write to bus from i2c bus class */
            status = i2cBus->write(transfer);
        }
        else
        {
            status = STATUS_BUSY;
        }
    }
    else
    {
        status = STATUS_NULL_POINTER;
    }
    
    return status;
}

Status_t I2CDevice::readFromBus(I2CTransfer_t &transfer)
{
    Status_t status = STATUS_OKAY;

    if(i2cBus != nullptr)
    {
        if(busAcquired)
        {
            /* read from bus from i2c bus class */
            status = i2cBus->read(transfer);
        }
        else
        {
            status = STATUS_BUSY;
        }
    }
    else
    {
        status = STATUS_NULL_POINTER;
    }

    return status;
}

