/**
 *******************************************************************************
 * @file    file_name.cpp
 * @author  HQ
 * @date    2025-12-19 20:39:53
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "gpio.hpp"

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

 Gpio::Gpio(GpioPin _pin, GpioMode _mode, GpioPullup _pullup, GpioPulldown _pulldown, GpioIntrType _intrType)
 : gpioPin(_pin), mode(_mode), pullup(_pullup), pulldown(_pulldown), intrType(_intrType)
 {
    if(gpioPin > GpioPin::GPIO_PIN_NONE && gpioPin < GpioPin::NUM_GPIO_PINS)
    {
        gpioNum = static_cast<gpio_num_t>(gpioPin);
        config.pin_bit_mask = (1ULL << static_cast<gpio_num_t>(gpioPin));
        config.mode = static_cast<gpio_mode_t>(mode);
        config.pull_up_en = static_cast<gpio_pullup_t>(pullup);
        config.pull_down_en = static_cast<gpio_pulldown_t>(pulldown);
        config.intr_type = static_cast<gpio_int_type_t>(intrType);

        gpio_config(&config);
    }
    else
    {
        /* throw error */
    }
 }

Gpio::~Gpio() 
{
    
} 

Status_t Gpio::set()
{
    Status_t status = STATUS_OKAY;

    if(gpio_set_level(gpioNum, GpioState::High) != ESP_OK)
    {
        status = STATUS_ERROR;
    }

    return status;
}

Status_t Gpio::reset()
{
    Status_t status = STATUS_OKAY;

    if(gpio_set_level(gpioNum, GpioState::Low) != ESP_OK)
    {
        status = STATUS_ERROR;
    }

    return status;
}

GpioState Gpio::get()
{
    GpioState state = GpioState::Low;
    int level = gpio_get_level(gpioNum);
    if(level == 1)
    {
        state = GpioState::High;
    }
    return state;
}
