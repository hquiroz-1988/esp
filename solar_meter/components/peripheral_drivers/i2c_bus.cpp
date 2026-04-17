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

extern "C" 
{
    #include "esp_log.h"
}

/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/
static const char *TAG = "i2c_bus";

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
 * PRIVATE FUNCTIONS
 *******************************************************************************/

Status_t I2CBus::createLink(void)
{
    Status_t status = STATUS_OKAY;

    /* create I2C command link */
    cmdHandle = i2c_cmd_link_create();

    if (cmdHandle == nullptr)
    {
        status = STATUS_HAL_ERROR;
    }

    return status;
}

Status_t I2CBus::deleteLink(void)
{

    /* delete I2C command link */
    i2c_cmd_link_delete(cmdHandle);

    return STATUS_OKAY;
}

Status_t I2CBus::configureDriver(void)
{
    Status_t status = STATUS_OKAY;

    // Implementation of configureDriver
    if (i2c_param_config(port, &conf) != ESP_OK)
    {
        status = STATUS_HAL_ERROR;
    }

    return status;
}

Status_t I2CBus::installDriver(void)
{
    Status_t status = STATUS_OKAY;

    // Implementation of installDriver
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)(sda.getPin());
    conf.sda_pullup_en = (gpio_pullup_t)(sda.getPullup());
    conf.scl_io_num = (gpio_num_t)(scl.getPin());
    conf.scl_pullup_en = (gpio_pullup_t)(scl.getPullup());
    conf.clk_stretch_tick = clockStretching;

    if (i2c_driver_install(port, conf.mode) != ESP_OK)
    {
        status = STATUS_HAL_ERROR;
    }

    return status;
}

/*******************************************************************************
 * PUBLIC FUNCTIONS
 *******************************************************************************/
I2CBus::I2CBus(Gpio &_sda, Gpio &_scl, i2c_port_t _port) : sda(_sda),
                                                           scl(_scl),
                                                           port(_port),
                                                           clockStretching(0),
                                                           devItr(0)
{
    Status_t status = initialize();
    // Constructor implementation
    if(status != STATUS_OKAY)
    {
        /* throw error */
        ESP_LOGE(TAG, "Failed to initialize I2C bus, error code: %d", static_cast<int>(status));
    }
    else
    {
        ESP_LOGI(TAG, "I2C Bus initialization complete");   
    }
}

I2CBus::~I2CBus()
{
    // Destructor implementation
}

Status_t I2CBus::initialize(void)
{
    Status_t status = STATUS_OKAY;

    status = installDriver();

    if (status == STATUS_OKAY)
    {
        status = configureDriver();
    }

    if (status == STATUS_OKAY)
    {
        status = busMutex.create();
    }

    return status;
}


Status_t I2CBus::addDevice(I2CDevice *device)
{
    Status_t status = STATUS_OKAY;

    if (device != nullptr)
    {
        // Device is valid
        if (devItr < MAX_DEV_COUNT)
        {
            /* once device is added to devices list, initialize  */
            status = device->addBus(this, devItr);

            if(status == STATUS_OKAY)
            {
                /* add device to devices list  */
                devices[devItr] = device;

                devItr++;
            }
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

Status_t I2CBus::acquire(int dev_id, uint32_t timeout)
{
    Status_t status = STATUS_OKAY;

    /* attempt to take device mutex */
    status = busMutex.lock();

    if (status == STATUS_OKAY)
    {
        if (currDevID == -1)
        {
            /* bus free to acquire, this should be the only acceptable case */
            currDevID = dev_id;
        }
        else if (currDevID != dev_id)
        {
            /* bus aquired by different device, but not locked */
            status = STATUS_BUSY;
            /* unlock and return */
            busMutex.unlock();
        }
        else
        {
            /* device is already acquired, but not locked, should be locked now*/
        }
    }

    return status;
}

Status_t I2CBus::release(int dev_id)
{
    Status_t status = STATUS_OKAY;

    if (currDevID == dev_id)
    {
        currDevID = -1;
    }
    else
    {
        status = STATUS_REQUEST_FAILED;
    }

    if (status == STATUS_OKAY)
    {
        /* release device mutex */
        status = busMutex.unlock();

        if (status != STATUS_OKAY)
        {
            currDevID = dev_id;
        }
    }

    return status;
}

Status_t I2CBus::write(I2CTransfer_t &transfer)
{
    Status_t status = STATUS_OKAY;

    /* create link */
    status = createLink();

    if (status == STATUS_OKAY)
    {
        /*  start i2c command     */
        status = cmdStart();
    }

    if (status == STATUS_OKAY)
    {
        /*  transmit all data */
        status = cmdBuild(transfer, true);
    }

    if (status == STATUS_OKAY)
    {
        /*  stop i2c command     */
        status = cmdEnd();
    }

    if(status == STATUS_OKAY)
    {
        /*  send i2c command     */
        status = cmdSend();
    }

    /* delete link */
    deleteLink();

    return status;
}

Status_t I2CBus::read(I2CTransfer_t &transfer)
{
    Status_t status = STATUS_OKAY;

    /* create link */
    status = createLink();

    if (status == STATUS_OKAY)
    {
        /*  start i2c command     */
        status = cmdStart();
    }

    if (status == STATUS_OKAY)
    {
        /*  transmit all data */
        status = cmdBuild(transfer, false);
    }

    if (status == STATUS_OKAY)
    {
        /*  stop i2c command     */
        status = cmdEnd();
    }

    if(status == STATUS_OKAY)
    {
        /*  send i2c command     */
        status = cmdSend();
    }

    /* delete link */
    deleteLink();

    return status;
}

Status_t I2CBus::cmdStart(void)
{
    esp_err_t err = ESP_OK;
    Status_t status = STATUS_OKAY;

    if (cmdHandle == nullptr)
    {
        status = STATUS_NULL_POINTER;
    }

    if (status == STATUS_OKAY)
    {
        /*  start i2c command     */
        err = i2c_master_start(cmdHandle);
        if (err != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    return status;
}

Status_t I2CBus::cmdBuild(I2CTransfer_t &transfer, bool write)
{
    Status_t status = STATUS_OKAY;

    if (transfer.data == nullptr || cmdHandle == nullptr)
    {
        status = STATUS_NULL_POINTER;
    }

    if (status == STATUS_OKAY && transfer.size == 0)
    {
        status = STATUS_OUT_OF_BOUNDS;
    }

    if (status == STATUS_OKAY)
    {
        /*  transmit device address*/
        if (i2c_master_write_byte(cmdHandle, ((transfer.devAddr << 1) & ( (write) ? I2C_MASTER_WRITE : I2C_MASTER_READ) ), transfer.ackEn) != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    if (status == STATUS_OKAY)
    {
        /*  transmit register address*/
        if (i2c_master_write_byte(cmdHandle, transfer.regAddr, transfer.ackEn) != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    if (status == STATUS_OKAY)
    {
        /*  transmit all data */
        if (i2c_master_write(cmdHandle, transfer.data, transfer.size, transfer.ackEn) != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    return status;
}

Status_t I2CBus::masterRead(I2CTransfer_t &transfer)
{
    esp_err_t err = ESP_OK;
    Status_t status = STATUS_OKAY;

    if (transfer.data == nullptr || cmdHandle == nullptr)
    {
        status = STATUS_NULL_POINTER;
    }

    if (status == STATUS_OKAY && transfer.size == 0)
    {
        status = STATUS_OUT_OF_BOUNDS;
    }

    if (status == STATUS_OKAY)
    {
        /*  receive all data */
        err = i2c_master_read(cmdHandle, transfer.data, transfer.size, static_cast<i2c_ack_type_t>(transfer.ackType));
        if (err != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    return status;
}

Status_t I2CBus::cmdSend(void)
{
    esp_err_t err = ESP_OK;
    Status_t status = STATUS_OKAY;

    if (cmdHandle == nullptr)
    {
        status = STATUS_NULL_POINTER;
    }

    if (status == STATUS_OKAY)
    {
        /*  stop i2c command     */
        err = i2c_master_cmd_begin(port, cmdHandle, 0);
        if (err != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    return status;
}

Status_t I2CBus::cmdEnd(void)
{
    esp_err_t err = ESP_OK;
    Status_t status = STATUS_OKAY;

    if (cmdHandle == nullptr)
    {
        status = STATUS_NULL_POINTER;
    }

    if (status == STATUS_OKAY)
    {
        /*  stop i2c command     */
        err = i2c_master_stop(cmdHandle);
        if (err != ESP_OK)
        {
            status = STATUS_HAL_ERROR;
        }
    }

    return status;
}
