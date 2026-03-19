/**
 *******************************************************************************
 * @file    ads1115_channel.cpp
 * @author  HQ
 * @date    2025-08-22 16:14:15
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "ads1115_channel.hpp"

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
ADS1115Channel::ADS1115Channel(ADS1115 & _ads1115, ADS1115Mux_t _channel) 
    : ads1115(_ads1115), channel(_channel), lowThreshold(0), highThreshold(0)
{

}


 Status_t ADS1115Channel::startConversion(void)
{
    Status_t retVal = STATUS_OKAY;

    /* start single conversion */
    ads1115.startSingleConversion(this->channelConfig);

    return retVal;
}

Status_t ADS1115Channel::getConversion(float & value)
{
    Status_t retVal = STATUS_OKAY;

    /* get conversion value from ADS1115 */
    ads1115.getLatestConversion(value);

    return retVal;
}

//!TODO: change float * to reference
 Status_t ADS1115Channel::getFilteredVoltage(float * value)
{
    Status_t retVal = STATUS_UNKNOWN;


    return retVal;
}

void ADS1115Channel::setCallback(void (*callback)(void*, uint32_t), void* context, uint32_t value)
{
    /* set the callback and context for this channel */
    this->callback2 = callback;
    this->callbackContext = context;
    this->callbackValue = value;

    /* register the callback with the base class */
    registerCallback(IntType::gpio_isr_handler);
}

void ADS1115Channel::staticWrapper(void* context, void * arg) 
{
    ADS1115Channel * instance = static_cast<ADS1115Channel*>(context);
    instance->alertPinISR(arg);
}


void ADS1115Channel::alertPinISR(void *arg)
{
    /*
        this is the default callback for any ADS1115 channel, we expect each channel instance 
        to override this instance and create a more meaningful callback.
    */
    if (    callback2 != nullptr
         && callbackContext != nullptr)
    {
        callback2(callbackContext, callbackValue);
    }
    else
    {
        /* perform something if we have nullptr */
        //!TODO: add logging
        // ESP_LOGE("ADS1115Channel","Pointer to callback2 is nullptr\n");
    }
}

