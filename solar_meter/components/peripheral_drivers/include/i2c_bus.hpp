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
#include "driver/i2c.h"
#include "gpio.hpp"
#include "typedefs.h"
#include <vector>
#include "i2c_device.hpp"
#include "mutex.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/

/**
 * @brief I2C transfer acknowledgment type
 */
enum class I2CTransferAckType_t 
{
    MASTER_ACK          = I2C_MASTER_ACK,                /*!< I2C ack for each byte read */
    MASTER_NACK         = I2C_MASTER_NACK,              /*!< I2C nack for each byte read */
    MASTER_LAST_NACK    = I2C_MASTER_LAST_NACK,    /*!< I2C nack for the last byte*/
    MASTER_ACK_MAX      = I2C_MASTER_ACK_MAX,
};

typedef struct
{
    uint8_t * data;
    size_t size;
    bool ackEn;
    I2CTransferAckType_t ackType;
} I2CTransfer_t;


class I2CBus
{
    friend class I2CDevice;

public:
    static const int MAX_DEV_COUNT = 4;
    I2CBus(Gpio & _sda, Gpio & _scl, i2c_port_t _port);
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
    Status_t installDriver(void);
    Status_t configureDriver(void);
    Status_t updateConfig(void);
    Status_t addDevice(I2CDevice *device);
    Status_t setClockStretching(uint32_t ticks);

protected:
    Status_t acquire(int dev_id, uint32_t timeout);
    Status_t release(int dev_id);
    Status_t transmit(I2CTransfer_t & transfer);
    Status_t receive(I2CTransfer_t & transfer);

    bool isReadyToSend();

private:
    Gpio & sda;
    Gpio & scl;
    i2c_port_t port;
    i2c_config_t conf;
    /* clock speeds above 400khz must be performed using I2C High-Speed mode */
    uint32_t clockStretching;
    uint32_t devItr;
    std::vector<I2CDevice *> devices;
    Mutex busMutex;
    int currDevID = -1;
    i2c_cmd_handle_t cmdHandle = nullptr;

    Status_t masterStart(void);
    Status_t masterWrite(I2CTransfer_t & transfer);
    Status_t masterRead(I2CTransfer_t & transfer);
    Status_t masterStop(void);
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // I2C_BUS_HPP