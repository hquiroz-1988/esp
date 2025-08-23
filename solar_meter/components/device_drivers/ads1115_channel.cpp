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

 Status_t ADS1115Channel::startConversion(void)
{
    Status_t retVal = STATUS_UNKNOWN;

    if (retVal == STATUS_OKAY)
    {
        /* pass in self argument to start ADC conversion */
        retVal = ads1115.startSingleConversion(*this);
    }

    return retVal;
}

Status_t ADS1115Channel::getConversion(float & value)
{
    Status_t retVal = STATUS_UNKNOWN;

    if (retVal == STATUS_OKAY)
    {
        /* pass in self argument to read ADC */
        retVal = ads1115.readADC_SingleEnded(*this);
    }

    if (retVal == STATUS_OKAY)
    {
        value = conversionValue;
    }
    else
    {
        value = 0.0f;
    }

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
    retVal = ads1115.startSingleConversion(*this);

    /* read conversion ready pin */
    while (ads1115.read() == true)
    {
        //!TODO: add a timeout
        // Wait for conversion to complete
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
