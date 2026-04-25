/**
 *******************************************************************************
 * @file    i2c_bus.hpp
 * @author  HQ
 * @date    2025-08-31 22:40:46
 * @brief
 *******************************************************************************
 */

#ifndef I2C_BUS_HPP
#define I2C_BUS_HPP

/*******************************************************************************
 * INCLUDES
 *******************************************************************************/
#include "i2c_typedefs.hpp"
#include "gpio.hpp"
#include "i2c_device.hpp"
#include "mutex.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
 *******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
 *******************************************************************************/

/* forward declaration of I2C Device*/
class I2CDevice;

class I2CBus
{
    friend class I2CDevice;

public:
    static const int MAX_DEV_COUNT = 4;
    I2CBus(Gpio &_sda, Gpio &_scl, i2c_port_t _port);
    virtual ~I2CBus();
    /**
     * @brief Initializes the I2C bus with the current configuration.
     *
     * This method installs and initializes the i2c driver, and subsequent
     * calls to this method will result in error.
     *
     * @return Status_t
     */
    Status_t initialize(void);
    Status_t addDevice(I2CDevice *device);
    // Status_t setClockStretching(uint32_t ticks);
    // Status_t updateConfig(void);

private:
    Gpio &sda;
    Gpio &scl;
    i2c_port_t port;
    i2c_config_t conf;
    /* clock speeds above 400khz must be performed using I2C High-Speed mode */
    uint32_t clockStretching;
    uint32_t devItr;
    I2CDevice *devices[MAX_DEV_COUNT];
    Mutex busMutex;
    int currDevID = -1;
    i2c_cmd_handle_t cmdHandle = nullptr;

    Status_t acquire(int dev_id, uint32_t timeout);
    Status_t release(int dev_id);
    Status_t write(I2CTransfer_t &transfer);
    Status_t read(I2CTransfer_t &transfer);

    Status_t installDriver(void);
    Status_t configureDriver(void);
    Status_t createLink(void);
    Status_t deleteLink(void);
    Status_t cmdStart(void);
    Status_t cmdBuildWrite(I2CTransfer_t &transfer);
    Status_t cmdBuildRead(I2CTransfer_t &transfer);
    Status_t masterRead(I2CTransfer_t &transfer);
    Status_t cmdSend(void);
    Status_t cmdEnd(void);
};

/*******************************************************************************
 * EXPORTED VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *******************************************************************************/

#endif // I2C_BUS_HPP