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
#include "i2c.h"
#include "gpio.hpp"
#include "typedefs.h"
#include <vector>
#include "i2c_device.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class I2CBus
{
    friend class I2CDevice;

public:
    static const int MAX_DEV_COUNT = 4;
    I2CBus(Gpio & _sda, Gpio & _scl, uint32_t i2c_clock_speed);
    virtual ~I2CBus();
    void initialize(I2CDevice * device_list);

protected:
    bool acquire(int dev_id, bool auto_cs = true);
    bool release(int dev_id, bool auto_cs = true);
    bool transmit(uint8_t *data, uint16_t size, uint32_t timeout);
    bool transceive(uint8_t *tx_data, uint8_t *rx_data, uint16_t size,
                    uint32_t timeout);
    bool isReadyToSend();

private:
    Gpio & sda;
    Gpio & scl;
    const uint32_t i2c_clk_freq;
    int cur_dev;

    I2CDevice * devices[MAX_DEV_COUNT];
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // I2C_BUS_HPP