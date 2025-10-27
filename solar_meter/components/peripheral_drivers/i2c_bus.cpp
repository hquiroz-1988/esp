/**
 *******************************************************************************
 * @file    i2c_bus.cpp
 * @author  HQ
 * @date    2025-09-01 18:00:28
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "i2c_bus.hpp"

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
I2CBus::I2CBus(Gpio & _sda, Gpio & _scl, i2c_port_t _port) : 
    sda(_sda),
    scl(_scl),
    port(_port),
    clockStretching(0),
    devItr(0),
    devices(MAX_DEV_COUNT)
{
    // Constructor implementation
}

I2CBus::~I2CBus()
{
    // Destructor implementation
}

Status_t I2CBus::initialize(void)
{
    esp_err_t err = ESP_OK;
    Status_t status = STATUS_OKAY;

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda.getPin();
    conf.sda_pullup_en = sda.getPullup();
    conf.scl_io_num = scl.getPin();
    conf.scl_pullup_en = scl.getPullup();
    conf.clk_stretch_tick = clockStretching;

    err = i2c_driver_install(port, conf.mode);

    if(err == ESP_OK)
    {
        err = i2c_param_config(port, &conf);
    }

    if(err != ESP_OK)
    {
        status = STATUS_HAL_ERROR;
    }

    return status;
}

Status_t I2CBus::addDevice(I2CDevice *device)
{
    Status_t status = STATUS_OKAY;

    if(device != nullptr)
    {
        // Device is valid
        if(devItr < MAX_DEV_COUNT)
        {
            devices[devItr++] = device;
        }
        else
        {
            status = STATUS_OUT_OF_BOUNDS;
        }
    }
    else
    {
        status = STATUS_NULL_POINTER;
    }

    return status;
}
