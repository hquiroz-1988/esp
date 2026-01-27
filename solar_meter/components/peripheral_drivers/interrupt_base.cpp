/**
 *******************************************************************************
 * @file    interrupt_base.cpp
 * @author  HQ
 * @date    2026-01-26 22:49:14
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
extern "C" 
{
    #include "driver/gpio.h"
}

#include "interrupt_base.hpp"
#include <list>


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
// GLOBAL list of registered objects that would like to receive callbacks
static std::list<InterruptBase *> list_gpio_isr_handler;

/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *******************************************************************************/
void global_gpio_isr_handler(void *arg);


/*******************************************************************************
 * STATIC FUNCTIONS
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/
InterruptBase::InterruptBase()
{
    // Constructor implementation
}

InterruptBase::~InterruptBase()
{
    // Destructor implementation
}

bool InterruptBase::registerCallback(IntType type) 
{
    bool success = false;

    

    switch (type) 
    {
        case IntType::gpio_isr_handler:
        {

            //!TODO: must install service and addd here

            /* if callback is not registered then register it */
            //!TODO: static bool that tells if registered already
            gpio_isr_register(global_gpio_isr_handler, NULL, 0, NULL);


            list_gpio_isr_handler.push_back(this);
            success = true;
            break;
        }
        default:
        {
            break;
        }
    }
    return success;
}

bool InterruptBase::removeCallback(IntType type) {
    bool success = false;

    //!TODO: static bool that tells if unregistered already


    switch (type) 
    {
        case IntType::gpio_isr_handler:
        {
            list_gpio_isr_handler.remove(this);
            success = true;
            break;
        }
        default:
        {
            break;
        }
    }
    return success;
}

// GLOBAL functions for actual callbacks from HAL Interrupt handler from
// stm32g4xx_hal.h


extern "C" void global_gpio_isr_handler(void *arg)
{
    std::list<InterruptBase *>::iterator it;
    for (it = list_gpio_isr_handler.begin();
         it != list_gpio_isr_handler.end(); 
         ++it) 
    {
        (*it)->gpio_isr_handler(arg);
    }
}
