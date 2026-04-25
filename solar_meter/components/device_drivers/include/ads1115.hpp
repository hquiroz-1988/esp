/**
 ********************************************************************************
 * @file    ads1115.hpp
 * @author  hugo
 * @date    2025-06-04 23:18:08
 * @brief   This module serves as an interface to the ads1115 ADC device. 
 *  The ADS1115 is a precision, low-power, 16-bit, I2Ccompatible,
 * analog-to-digital converters (ADCs). The ADS1115 device incorporate a low-drift 
 * voltage reference and an oscillator. The ADS1115 also incorporate a programmable 
 * gain amplifier (PGA) and a digital comparator. These features, along with a wide
 * operating supply range, make the ADS111x well suited for power- and 
 * space-constrained, sensormeasurement applications. The ADS111x perform 
 * conversions at data rates up to 860 samples per second (SPS). The PGA offers 
 * input ranges from ±256 mV to ±6.144 V, allowing precise large- and small-signal 
 * measurements. The ADS1115 features an input multiplexer (MUX) that allows two 
 * differential or four single-ended input measurements. Use the digital comparator
 *  in the ADS1114 and ADS1115 for under- and overvoltage detection. The ADS111x 
 * operate in either continuousconversion mode or single-shot mode. The devices
 * are automatically powered down after one conversion in single-shot mode; 
 * therefore, power consumption is significantly reduced during idle periods.
 ********************************************************************************
 */

#ifndef ADS1115_HPP
#define ADS1115_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "i2c_task.h"
#include "i2c_device.hpp"
#include "ads1115_regs.hpp"
#include "ads1115_channel.hpp"
#include "gpio.hpp"
#include <string.h>


/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/


/*******************************************************************************
 * CLASSES & TYPEDEFS
*******************************************************************************/

/* forward declaration of ADS1115Channel */
class ADS1115Channel;

class ADS1115 : public I2CDevice, public InterruptBase
{
public:
    /**
     * @class ADS1115
     * @brief A class to interface with the ADS1115 analog-to-digital converter.
     * 
     * This class provides methods to interact with the ADS1115 device, including
     * testing the class functionality and retrieving the latest reading from the device.
     */

    /**
     * @brief Constructor for the ADS1115 class.
     * 
     * Initializes the ADS1115 instance.
     */
    ADS1115(I2CBus & bus, Gpio & _gpio);
    virtual ~ADS1115();
    Status_t configure(const ADS1115_Config_t & configObj);
    Status_t startSingleConversion(ADS1115_Config_t & configObj);
    Status_t getLatestConversion(int16_t & value);
    Status_t getAlertPinStatus(bool & pinState);
    void initialize(void);
    

    /* register action functions */
    Status_t setConfigRegister(ADS1115_Config_t & configObj);
    Status_t getConfigRegister(ADS1115_Config_t & configObj);
    Status_t setLowThreshold(int16_t threshold);
    Status_t getLowThreshold(int16_t & threshold);
    Status_t setHighThreshold(int16_t threshold);
    Status_t getHighThreshold(int16_t & threshold); 
    Status_t getRawConversionRegister(uint16_t & rawValue);
    Status_t readConversionRegister(float & value);


    /* add a static wrapper function that will be based as arg to caller */
    static void staticWrapper(void* context, void * arg);
    void alertPinISR(void * arg);

    //!TODO: implement functions once they are needed
    Status_t readADC_Differential(ADS1115_Conversion_t & convObj);
    Status_t startDifferentialConversion(ADS1115_Conversion_t & convObj);
    Status_t startComparator_SingleEnded(const ADS1115_Comparator_t & compObj);
    Status_t startComparator_Differential(const ADS1115_Comparator_t & compObj);
    Status_t stopComparator(const ADS1115_Comparator_t & compObj);
    Status_t setDeviceAddress(ADS1115_Address addy);
    Status_t enableInterrupt();
    Status_t setNotificationTaskHandle(TaskHandle_t taskHdl);

    protected:

    

    private:

    Gpio & alertPin;
    
    ADS1115_Address address;

    /* cached register objects */
    ADS1115_Transfer_t configRegisterObj;
    ADS1115_Transfer_t pointerRegisterObj;
    ADS1115_Transfer_t conversionRegisterObj;
    ADS1115_Transfer_t loThresholdRegisterObj;
    ADS1115_Transfer_t hiThresholdRegisterObj;
    
    /* transfer object for interfacing to I2C Bus */
    I2CTransfer_t txObj;
    I2CTransfer_t rxObj;

    Status_t cachedRegisterToTransferObj(const ADS1115_Transfer_t & configObj, I2CTransfer_t & transferObj);


    //!TODO: determine if we still need to have this write config register function
    
    Status_t configObjToBytes(const ADS1115_Config_t & configObj, uint8_t * bytes);
    Status_t bytesToConfigObj(const uint8_t * bytes, ADS1115_Config_t & configObj);

   
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // ADS1115_HPP