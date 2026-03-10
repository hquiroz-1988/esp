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
enum class GpioPin : uint8_t
{
    GPIO_0          = GPIO_NUM_0,
    GPIO_1          = GPIO_NUM_1,
    GPIO_2          = GPIO_NUM_2,
    GPIO_3          = GPIO_NUM_3,
    GPIO_4          = GPIO_NUM_4,
    GPIO_5          = GPIO_NUM_5,
    GPIO_6          = GPIO_NUM_6,
    GPIO_7          = GPIO_NUM_7,
    GPIO_8          = GPIO_NUM_8,
    GPIO_9          = GPIO_NUM_9,
    GPIO_10         = GPIO_NUM_10,
    GPIO_11         = GPIO_NUM_11,
    GPIO_12         = GPIO_NUM_12,
    GPIO_13         = GPIO_NUM_13,
    GPIO_14         = GPIO_NUM_14,
    GPIO_15         = GPIO_NUM_15,
    GPIO_16         = GPIO_NUM_16,
    NUM_GPIO_PINS   = GPIO_NUM_MAX,

    GPIO_PIN_NONE   = 0xFF,

};

enum class GpioPullup
{
    Disable         = GPIO_PULLUP_DISABLE,
    Enable          = GPIO_PULLUP_ENABLE
};

enum class GpioPulldown
{
    Disable         = GPIO_PULLDOWN_DISABLE,
    Enable          = GPIO_PULLDOWN_ENABLE
};

enum class GpioMode
{
    Disable         = GPIO_MODE_DISABLE,
    Input           = GPIO_MODE_INPUT,
    Output          = GPIO_MODE_OUTPUT,
    OutputOpenDrain = GPIO_MODE_OUTPUT_OD
};

enum class GpioState
{
    Low             = 0,
    High            = 1
};

enum class GpioIntrType
{
    Disable         = GPIO_INTR_DISABLE,
    RisingEdge      = GPIO_INTR_POSEDGE,
    FallingEdge     = GPIO_INTR_NEGEDGE,
    AnyEdge         = GPIO_INTR_ANYEDGE,
    LowLevel        = GPIO_INTR_LOW_LEVEL,
    HighLevel       = GPIO_INTR_HIGH_LEVEL
};

class Gpio : public InterruptBase
{
    public:
        /* full constructor */
        Gpio(   GpioPin _pin, 
                GpioMode _mode, 
                GpioPullup _pullup, 
                GpioPulldown _pulldown,
                GpioIntrType _intrType);
        
        
        /* delegated constructors */
        Gpio(   GpioPin _pin, 
                GpioMode _mode, 
                GpioPullup _pullup, 
                GpioIntrType _intrType) :
        Gpio(_pin, _mode, _pullup, GpioPulldown::Disable, _intrType) {};

        Gpio(   GpioPin _pin, 
                GpioMode _mode, 
                GpioPullup _pullup ) :
        Gpio(_pin, _mode, _pullup, GpioPulldown::Disable, GpioIntrType::Disable) {};

        virtual ~Gpio();
        Status_t set();
        Status_t reset();
        GpioState get();
        GpioPin getPin() { return gpioPin; }
        GpioMode getMode() { return mode; }
        GpioPullup getPullup() { return pullup; }
        GpioPulldown getPulldown() { return pulldown; }
        void set_isr_handler(void (*handler)(GpioPin pin)) { isrHandler = handler; }

    private:
        GpioPin gpioPin;
        GpioMode mode;
        GpioPullup pullup;
        GpioPulldown pulldown;
        GpioIntrType intrType;

        gpio_num_t gpioNum;
        gpio_config_t config;

        virtual void gpio_isr_handler(void *arg) override;
        static bool isrHandlerRegistered;

        void (*isrHandler)(GpioPin pin) = nullptr;
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // GPIO_HPP