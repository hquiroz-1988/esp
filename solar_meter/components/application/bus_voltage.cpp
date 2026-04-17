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

}

#include "bus_voltage.hpp"
#include "esp_log.h"

/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/
static const char *TAG = "BusVoltage";

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
BusVoltage::BusVoltage(ADS1115 & _ads1115, ADS1115Mux_t _channel) 
    : ADS1115Channel(_ads1115, _channel)
{
    // Constructor implementation
    if(initialize() != STATUS_OKAY)
    {
        /* throw error */
        ESP_LOGE(TAG, "Failed to initialize Bus Voltage channel");
    }
}

BusVoltage::~BusVoltage()
{
    // Destructor implementation
}

Status_t BusVoltage::initialize(void)
{
    /* check that pm is not nullptr */
    Status_t retVal = STATUS_OKAY;

    /* configure ADS1115 module */
    if(retVal == STATUS_OKAY)
    {
        /* for configuration we are not trying to trigger a conversion+ */
        channelConfig.opStatus = ADS1115_OperationalStatus_t::Write_No_Effect;
        /* busvoltage measurement will be single ended at AIN0  */
        channelConfig.mux = ADS1115Mux_t::AIN0_GND;
        /*  range 2.048 will be used since device VDD will be 3.3V, 2.048 is the
            next largest value    */
        channelConfig.pga = ADS1115PGA_t::FSR_2_048V;
        /* all measurements are requested not automatic, continuous would drain power */
        channelConfig.mode = ADS1115Mode_t::SingleShot;
        /*  sampling rate will be high but actual sampling will be much lower rate
            this is to save power, lower device sampling rates require device to be on much
            longer hence consuming more power */
        channelConfig.dataRate = ADS1115DataRate_t::SPS_860;
        /*  comparator mode might need to window when measuring battery voltage to ensure
            batteries stay above and below thresholds    */
        channelConfig.compMode = ADS1115CompMode_t::Window;
        /* comparator polarity will be active low */
        channelConfig.compPolarity = ADS1115CompPolarity_t::ActiveLow;
        /* comparator is latching, in case we want to poll signal   */
        channelConfig.compLatch = ADS1115CompLatch_t::Latching;
        /* asserting alert/rdy pin after four coversions to filter out sporadic measurements */
        channelConfig.compQueue = ADS1115CompQueue_t::AssertAfterFourConversions;
        retVal = ads1115.configure(channelConfig);
    }

    /*  set low and high thresholds for bus voltage, these will be 
        set to not use the comparator but instad used to alert when
        a sample is ready this is done by setting low high threshold value to most
        negative value possible, thresholds will be set at conversion request since
        different channels might actually use the threshold values  */
    if(retVal == STATUS_OKAY)
    {
        retVal = setLowThreshold(static_cast<int16_t>(ADS1115_ThresholdValues::Max));
    }

    if(retVal == STATUS_OKAY)
    {
        retVal = setHighThreshold(static_cast<int16_t>(ADS1115_ThresholdValues::Min));
    }

    return retVal;
}



/*******************************************************************************
 * INTERRUPT SERVICE ROUTINES
 *******************************************************************************/
