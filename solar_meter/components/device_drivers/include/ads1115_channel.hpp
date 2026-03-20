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

class ADS1115Channel : public InterruptBase
{
    public:
    ADS1115Channel(ADS1115 & _ads1115, ADS1115Mux_t _channel);
    virtual ~ADS1115Channel();
    
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


    virtual void alertPinISR(void * arg);

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
     * @return Status_t - returns error type or success
     */
    Status_t getHighThreshold(int16_t & value) const;

    /* overload set callback function to pass in more parameters */
    void setCallback(void (*callback)(void*, uint32_t), void* context, uint32_t value);

    /* static wrapper to be called by interrupt handler from ADS1115 */
    static void staticWrapper(void* context, void * arg);

    protected:
    ADS1115 & ads1115;
    ADS1115_Config_t channelConfig;
    ADS1115Mux_t channel;
    int16_t conversionValue;
    int16_t lowThreshold;
    int16_t highThreshold;

    //!TODO: there must be a better to create a new callback template....todo
    void (*callback2)(void*, uint32_t);
    uint32_t callbackValue;

    private:

    
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/



#endif // ADS1115_CHANNEL_HPP