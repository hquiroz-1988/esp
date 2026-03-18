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

    //!TODO: adding this here but this will actually be called whenever a conversion is requested, 
    //so that each channel can temporarily set the callback
    /* add callback for alert pin */
    ads1115.setCallback(ADS1115Channel::staticWrapper, this);

    //!TODO: this is the clear callback it must be called at the end of the conversion 
    // so as not to leave any traces back to this channel.
    ads1115.clearCallback();
}


 Status_t ADS1115Channel::startConversion(void)
{
    Status_t retVal = STATUS_OKAY;

    //!TODO: implement start of conversion

    return retVal;
}

Status_t ADS1115Channel::getConversion(float & value)
{
    Status_t retVal = STATUS_OKAY;

    //!TODO: implement get conversion

    return retVal;
}

//!TODO: change float * to reference
 Status_t ADS1115Channel::getFilteredVoltage(float * value)
{
    Status_t retVal = STATUS_UNKNOWN;

    /* grab current latch configuration */
    ADS1115CompLatch_t compConfig = configRegister.compLatch;

    /* in order to read pin, it needs to latch */
    configRegister.compLatch = ADS1115CompLatch_t::Latching;

    /* start conversion */
    retVal = ads1115.startSingleConversion();

    /* read conversion ready pin */
    bool pinState = true;
    //!TODO: should this be notify wait from isr??
    if(retVal == STATUS_OKAY)
    {
        retVal = ads1115.waitForConversionComplete();
    }

    if(retVal == STATUS_OKAY)
    {
        /* pass in self argument to read ADC */
        retVal = ads1115.readADC_SingleEnded(*this);
    }

    /* reassign original latch configuration */
    configRegister.compLatch = compConfig;
    
    if (retVal == STATUS_OKAY)
    {
        *value = conversionValue;
    }
    else
    {
        *value = 0.0f;
    }

    return retVal;
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
   //!TODO: add a log to indicate the default ISR is being called
}

