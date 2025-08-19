/**
 *******************************************************************************
 * @file    bus_voltage.cpp
 * @author  HQ
 * @date    2025-08-18 22:22:21
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/

extern "C" 
{
    #include "esp_log.h"
}

#include "bus_voltage.hpp"


/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/
static const char *TAG = "bus voltage";

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
BusVoltage::BusVoltage(ADS1115 & _ads1115) : ads1115(_ads1115)
{
    // Constructor implementation
}

Status_t BusVoltage::init(void)
{
    /* create configuration object   */
    ADS1115_Config_t configObj;

    //!TODO: verify all these values are goooood
    configObj.opStatus = OperationStatus_t::NoConversionInProgress;
    configObj.mux = ADS1115Mux_t::AIN0_AIN1;
    configObj.pga = ADS1115PGA_t::FSR_2_048V;
    configObj.mode = ADS1115Mode_t::Continuous;
    configObj.dataRate = ADS1115DataRate_t::SPS_860;
    configObj.compMode = ADS1115CompMode_t::Traditional;
    configObj.compPolarity = ADS1115CompPolarity_t::ActiveHigh;
    configObj.compLatch = ADS1115CompLatch_t::NonLatching;
    configObj.compQueue = ADS1115CompQueue_t::AssertAfterFourConversions;

    /* configure ADS1115 module */
    Status_t retVal = ads1115.configure(configObj);

    /* init conversion object */
    convObj.type = Conversion_t::SingleEnded;
    convObj.channel = ADS1115Mux_t::AIN0_AIN1;
    convObj.value = 0.0f;


    return retVal;
}

Status_t BusVoltage::getFilteredVoltage(float & value)
{
    Status_t retVal = STATUS_UNKNOWN;

    retVal = ads1115.readADC_SingleEnded(convObj);

    if (retVal == STATUS_OKAY)
    {
        value = convObj.value;
    }
    else
    {
        value = 0.0f;
    }

    return retVal;
}
