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
BusVoltage::BusVoltage(PowerMonitor & _pm, ADS1115 & _ads1115) : pm(_pm), ADS1115Channel(_ads1115)
{
    // Constructor implementation
}

Status_t BusVoltage::init(void)
{

    /* for configuration we are not trying to trigger a conversion+ */
    configRegister.opStatus = ADS1115_OperationalStatus_t::Write_No_Effect;
    /* busvoltage measurement will be single ended at AIN0  */
    configRegister.mux = ADS1115Mux_t::AIN0_GND;
    /*  range 2.048 will be used since device VDD will be 3.3V, 2.048 is the
        next largest value    */
    configRegister.pga = ADS1115PGA_t::FSR_2_048V;
    /* all measurements are requested not automatic, continuous would drain power */
    configRegister.mode = ADS1115Mode_t::SingleShot;
    /*  sampling rate will be high but actual sampling will be much lower rate
        this is to save power, lower device sampling rates require device to be on much
        longer hence consuming more power */
    configRegister.dataRate = ADS1115DataRate_t::SPS_860;
    /*  comparator mode might need to window when measuring battery voltage to ensure
        batteries stay above and below thresholds    */
    configRegister.compMode = ADS1115CompMode_t::Window;
    /* comparator polarity will be active low */
    configRegister.compPolarity = ADS1115CompPolarity_t::ActiveLow;
    /* comparator is latching, in case we want to poll signal   */
    configRegister.compLatch = ADS1115CompLatch_t::Latching;
    /* asserting alert/rdy pin after four coversions to filter out sporadic measurements */
    configRegister.compQueue = ADS1115CompQueue_t::AssertAfterFourConversions;

    /* configure ADS1115 module */
    Status_t retVal = ads1115.configure(configRegister);

    /*  set low and high thresholds for bus voltage, these will be 
        set to not use the comparator but instad used to alert when
        a sample is ready this is done by setting low high threshold value to most
        negative value possible, thresholds will be set at conversion request since
        different channels might actually use the threshold values  */
    lowThreshold = ADS1115_CONVERSION_COMPLETE_LO;
    highThreshold = ADS1115_CONVERSION_COMPLETE_HI;

    /* init conversion object */
    type = Conversion_t::SingleEnded;
    channel = ADS1115Mux_t::AIN0_GND;
    conversionValue = 0;

    return retVal;
}



/*******************************************************************************
 * INTERRUPT SERVICE ROUTINES
 *******************************************************************************/

 void BusVoltage::runAlertISR(void * arg)
 {
    // Handle the alert interrupt, e.g., notify a task or set a flag
    // This is a placeholder implementation
    ESP_LOGI(TAG, "Alert ISR triggered");
    //!TODO: call power monitor task notify
    pm.notifyFromISR();
 }