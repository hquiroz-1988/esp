/**
 *******************************************************************************
 * @file    ads1115_channel.hpp
 * @author  HQ
 * @date    2025-08-22 11:58:43
 * @brief   
 *******************************************************************************
 */

#ifndef ADS1115_CHANNEL_HPP
#define ADS1115_CHANNEL_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "ads1115_regs.hpp"
#include "ads1115.hpp"
/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class ADS1115;

class ADS1115Channel
{
    public:
    ADS1115Channel(ADS1115 & _ads1115);
    virtual ~ADS1115Channel();

    /** @brief  Initializes ADS1115 module including registers, thresholds,
     *  and other necessary configurations.
     *
     *  @param void 
     *  @return void 
     */
    virtual Status_t init(void);    

    /**
     * @brief  Starts an ADS1115 conversion and returns to caller, non-blocking.
     * 
     * @param void
     * @return Status_t - returns error type or success
     */
    virtual Status_t startConversion(void);

    /** @brief  Fetches latest conversion from ADS1115 device.
     *
     *  @param value - pointer to a float value that will return
     *  the voltage value
     *  @return Status_t - returns error type or success
     */
    virtual Status_t getConversion(float & value);

    /** @brief  Starts, waits, and returns for ADS1115 conversion, blocking.
     *
     *  @param value - pointer to a float value that will return
     *  the voltage value
     *  @return Status_t - returns error type or success
     */
    virtual Status_t getFilteredVoltage(float * value);
    //!TODO: change to startAndWaitForConversion(float & value)

     /**
     * @brief  Runs the alert ISR for the ADS1115 device.
     * 
     * @param arg - pointer to any arguments needed for the ISR
     * @return void
     */
    virtual void runAlertISR(void * arg);

    /**
     * @brief Sets the low threshold value of the ADS1115 channel.
     * 
     * @param value - low threshold value to set
     * @return Status_t - returns error type or success
     */
    Status_t setLowThreshold(int16_t value);

    /**
     * @brief Gets low threshold value of the ADS1115 channel.
     * 
     * @param value - reference to store the low threshold value
     * @return Status_t - returns error type or success
     */
    Status_t getLowThreshold(int16_t & value) const;

    /**
     * @brief Sets the high threshold value of the ADS1115 channel.
     * 
     * @param value - high threshold value to set
     * @return Status_t - returns error type or success
     */
    Status_t setHighThreshold(int16_t value);

    /**
     * @brief Gets high threshold value of the ADS1115 channel.
     * 
     * @param value - reference to store the high threshold value
     * @return Status_t - high threshold value
     */
    Status_t getHighThreshold(int16_t & value) const;

    private:
    

    protected:
    ADS1115 & ads1115;
    ADS1115_PointerRegister addressPtrRegister;
    ADS1115_Config_t configRegister;
    Conversion_t type;
    ADS1115Mux_t channel;
    int16_t conversionValue;
    int16_t lowThreshold;
    int16_t highThreshold;
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/



#endif // ADS1115_CHANNEL_HPP