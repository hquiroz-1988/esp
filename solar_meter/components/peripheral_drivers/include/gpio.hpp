/**
 *******************************************************************************
 * @file    gpio.hpp
 * @author  HQ
 * @date    2025-08-22 12:05:58
 * @brief   
 *******************************************************************************
 */

#ifndef GPIO_HPP
#define GPIO_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "typedefs.h"
#include "interrupt_base.hpp"
#include "driver/gpio.h"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
enum class GpioPin
{
    GPIO_PIN_NONE,
    GPIO_0 = 0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    GPIO_10,
    GPIO_11,
    GPIO_12,
    GPIO_13,
    GPIO_14,
    GPIO_15
};

enum class GpioPullup
{
    Disable,
    Enable
};

enum class GpioPulldown
{
    Disable,
    Enable
};

enum class GpioMode
{
    Disable,
    Input,
    Output,
    OutputOpenDrain
};

class Gpio
{
    public:
        Gpio(GpioPin _pin, GpioMode _mode, GpioPullup _pullup = GpioPullup::Disable, GpioPulldown _pulldown = GpioPulldown::Disable);
        virtual ~Gpio();
        void set();
        void reset();
        bool read();
        gpio_num_t getPin();
        gpio_mode_t getMode();
        gpio_pullup_t getPullup();

    private:
        GpioPin gpioPin;
        gpio_num_t pin;
        GpioMode mode;
        GpioPullup pullup;
        GpioPulldown pulldown;
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/

#endif // GPIO_HPP