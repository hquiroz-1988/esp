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
static const char *TAG = "ads1115_channel";
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

ADS1115Channel::~ADS1115Channel()
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
    int16_t rawValue = 0;

    /* get conversion value from ADS1115 */
    retVal = ads1115.getLatestConversion(rawValue);
    ESP_LOGI(TAG, "Raw conversion value: %d", rawValue);

    if (retVal == STATUS_OKAY)
    {
        value = valueToScaledValue(rawValue);
        ESP_LOGI(TAG, "Scaled conversion value: %d mV", static_cast<int>(value * 1000.0f));
    }
    else
    {
        value = 0.0f; // Set value to a default value in case of an error
        ESP_LOGI(TAG, "Failed to get conversion value, status: %d", static_cast<int>(retVal));
    }

    return retVal;
}

float ADS1115Channel::valueToScaledValue(int16_t rawValue)
{
    const int16_t rawCode = rawValue;
    float fullScaleVoltage = 2.048f;

    switch (channelConfig.pga)
    {
    case ADS1115PGA_t::FSR_6_144V:
        fullScaleVoltage = 6.144f;
        break;
    case ADS1115PGA_t::FSR_4_096V:
        fullScaleVoltage = 4.096f;
        break;
    case ADS1115PGA_t::FSR_2_048V:
        fullScaleVoltage = 2.048f;
        break;
    case ADS1115PGA_t::FSR_1_024V:
        fullScaleVoltage = 1.024f;
        break;
    case ADS1115PGA_t::FSR_0_512V:
        fullScaleVoltage = 0.512f;
        break;
    case ADS1115PGA_t::FSR_0_256V_1:
    case ADS1115PGA_t::FSR_0_256V_2:
    case ADS1115PGA_t::FSR_0_256V_3:
        fullScaleVoltage = 0.256f;
        break;
    default:
        fullScaleVoltage = 2.048f;
        break;
    }

    return (static_cast<float>(rawCode) * fullScaleVoltage) / 32768.0f;
}

Status_t ADS1115Channel::setLowThreshold(int16_t value)
{
    Status_t retStatus = STATUS_OKAY;

    /* update local threshold */
    lowThreshold = value;

    /* update the threshold in the ADS1115 */
    ads1115.setLowThreshold(value);

    return retStatus;
}

Status_t ADS1115Channel::setHighThreshold(int16_t value)
{
    Status_t retStatus = STATUS_OKAY;

    /* update local threshold */
    highThreshold = value;

    /* update the threshold in the ADS1115 */
    ads1115.setHighThreshold(value);

    return retStatus;
}

Status_t ADS1115Channel::setCallback(void (*callback)(void*, uint32_t), void* context, uint32_t value)
{
    Status_t retVal = STATUS_OKAY;

    if (callback == nullptr || context == nullptr)
    {
        return STATUS_NULL_POINTER;
    }

    /* set the callback and context for this channel */
    this->callback2 = callback;
    this->callbackContext = context;
    this->callbackValue = value;

    /* route ADS1115 alert ISR into this channel callback */
    retVal = ads1115.setCallback(ADS1115Channel::staticWrapper, this);

    return retVal;
}

Status_t ADS1115Channel::clearCallback()
{
    callback2 = nullptr;
    callbackValue = 0;
    InterruptBase::clearCallback();
    return ads1115.clearCallback();
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

