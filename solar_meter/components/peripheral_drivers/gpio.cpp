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

/*******************************************************************************
 * PRIVATE TYPEDEFS
 *******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/
bool Gpio::isrHandlerRegistered = false;
static const char *TAG = "gpio";

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
    if( (gpioPin >= GpioPin::GPIO_0) && (gpioPin < GpioPin::NUM_GPIO_PINS) )
    {
        gpioNum = static_cast<gpio_num_t>(gpioPin);
        config.pin_bit_mask = (1ULL << static_cast<gpio_num_t>(gpioPin));
        config.mode = static_cast<gpio_mode_t>(mode);
        config.pull_up_en = static_cast<gpio_pullup_t>(pullup);
        config.pull_down_en = static_cast<gpio_pulldown_t>(pulldown);
        config.intr_type = static_cast<gpio_int_type_t>(intrType);

        if(gpio_config(&config) != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to configure gpio");
        }   
        else
        {
            ESP_LOGI(TAG, "Gpio configured");
        }

        if(config.intr_type != GPIO_INTR_DISABLE)
        {
            ESP_LOGI(TAG, "Gpio interrupt type is not disable");
            /* install gpio isr service if it has not been done already  */
            if(!isrHandlerRegistered)
            {
                esp_err_t isrServiceStatus = gpio_install_isr_service(0);
                if(isrServiceStatus != ESP_OK && isrServiceStatus != ESP_ERR_INVALID_STATE)
                {
                    ESP_LOGE(TAG, "Failed to install gpio isr service");
                }
                else
                {
                    ESP_LOGI(TAG, "Gpio isr service installed");
                    isrHandlerRegistered = true;
                }
            }
            
            /* register gpio for this pin  */
            if(gpio_isr_handler_add(gpioNum, global_gpio_isr_handler, (void *) gpioNum) != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to register isr handler for this pin");
            }
            else
            {
                ESP_LOGI(TAG, "Gpio isr handler registered for this pin");

                if (!registerCallback(IntType::gpio_isr_handler))
                {
                    ESP_LOGE(TAG, "Failed to register GPIO callback dispatcher");
                }
            }
            
        }

        ESP_LOGI(TAG, "Gpio Init Complete. Pin: %d, Mode: %d, Pullup: %d, Pulldown: %d, IntrType: %d\n", 
                static_cast<uint8_t>(gpioPin), 
                static_cast<uint8_t>(mode), 
                static_cast<uint8_t>(pullup), 
                static_cast<uint8_t>(pulldown), 
                static_cast<uint8_t>(intrType));
    }
    else
    {
        /* throw error */
        ESP_LOGE(TAG, "Invalid GPIO pin number: %d\n", static_cast<uint8_t>(gpioPin));
    }
 }

Gpio::~Gpio() 
{
    
} 

Status_t Gpio::set()
{
    Status_t status = STATUS_OKAY;

    if(gpio_set_level(gpioNum, (uint32_t)GpioState::High) != ESP_OK)
    {
        status = STATUS_HAL_ERROR;
    }

    return status;
}

Status_t Gpio::reset()
{
    Status_t status = STATUS_OKAY;

    if(gpio_set_level(gpioNum, (uint32_t)GpioState::Low) != ESP_OK)
    {
        status = STATUS_HAL_ERROR;
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


void Gpio::gpio_isr_handler(void *arg)
{
    /*
        converting arg from void * to uint32_t big is a big no-no but 
        this is what the lib requires
    */
    uint32_t gpio = (uint32_t) arg;
    if (gpio == gpioNum)
    {
        /* perform any gpio related duties here */
        //!TODO: check if we need to clear interrupt or any
        //other administrative tasks related to gpio interrupt handling

        /* if callback exists, call that here */
        if(callback != nullptr
           && callbackContext != nullptr)
        {   
            /* call the callback and pass the context and an argument */
            callback(callbackContext, arg);
        }
        else
        {
            /* perform something if we have nullptr */
            //!TODO: add logging to this module
            // ESP_LOGE("Gpio", "Pointer to Gpio::callback is nullptr\n");
        }
    }
}