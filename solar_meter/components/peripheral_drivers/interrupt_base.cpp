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

bool InterruptBase::removeCallback(IntType type)
{
    bool success = false;

    //! TODO: static bool that tells if unregistered already

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

void global_gpio_isr_handler(void *arg)
{
    std::list<InterruptBase *>::iterator it;
    for (it = list_gpio_isr_handler.begin();
         it != list_gpio_isr_handler.end();
         ++it)
    {
        /* call every registered handler */
        (*it)->gpio_isr_handler(arg);
    }
}


Status_t InterruptBase::setCallback(void (*callback)(void*, void *), void* context)
{
    Status_t statusRet = STATUS_OKAY;
    
    if( callback != nullptr
        && context != nullptr)
    {
        this->callback = callback;
        this->callbackContext = context;
    }
    else
    {
        statusRet = STATUS_NULL_POINTER;
    }

    return statusRet;
}

Status_t InterruptBase::clearCallback()
{
    Status_t statusRet = STATUS_OKAY;

    this->callback = nullptr;
    this->callbackContext = nullptr;

    return statusRet;
}

