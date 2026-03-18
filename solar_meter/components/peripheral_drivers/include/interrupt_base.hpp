/**
 *******************************************************************************
 * @file    interrupt_base.hpp
 * @author  HQ
 * @date    2025-08-22 12:08:27
 * @brief   
 *******************************************************************************
 */

#ifndef INTERRUPT_BASE_HPP
#define INTERRUPT_BASE_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "typedefs.h"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class InterruptBase {
public:
    InterruptBase();
    virtual ~InterruptBase();
    // pure virtual Callbacks
    virtual void gpio_isr_handler(void * arg){};

    /* make setCallback virtual so that each derived class can override with their own args */
    virtual Status_t setCallback(void (*callback)(void*, void *), void* context);
    
    Status_t clearCallback();
protected:
    enum class IntType {
        gpio_isr_handler,
        NumberOfIntTypes
    };
    bool registerCallback(IntType type);
    bool removeCallback(IntType type);

    /*  add the option to have isr's call other callbacks
        protected so that inherited classes can access. 
    */
    void (*callback)(void*, void *) = nullptr;
    void* callbackContext = nullptr;

private:
    
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/
extern "C" void global_gpio_isr_handler(void *arg);


#endif // INTERRUPT_BASE_HPP