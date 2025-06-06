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
#include "typedefs.h"
#include "i2c_task.h"
#include "ads1115_regs.h"
#include <string.h>

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/
#define ADS1115_POINTER_REGISTER_SIZE                   (1u)
#define ADS1115_CONVERSION_REGISTER_SIZE                (2u)
#define ADS1115_CONFIG_REGISTER_SIZE                    (2u)
#define ADS1115_LO_THRESH_REGISTER_SIZE                 (2u)
#define ADS1115_HI_THRESH_REGISTER_SIZE                 (2u)

/*******************************************************************************
 * CLASSES & TYPEDEFS
*******************************************************************************/
//!TODO: consider renaming this 

typedef enum 
{
    ADS1115_OPERATION_STATUS_CONVERSION_IN_PROGRESS = 0, // Conversion in progress
    ADS1115_OPERATION_STATUS_NO_CONVERSION_IN_PROGRESS = 1,      // Conversion ready
}OperationStatus_t;

typedef enum
{
    ADS1115_MUX_AIN0_AIN1 = 0b000, // AINP = AIN0 and AINN = AIN1 (default)
    ADS1115_MUX_AIN0_AIN3 = 0b001, // AINP = AIN0 and AINN = AIN3
    ADS1115_MUX_AIN1_AIN3 = 0b010, // AINP = AIN1 and AINN = AIN3
    ADS1115_MUX_AIN2_AIN3 = 0b011, // AINP = AIN2 and AINN = AIN3
    ADS1115_MUX_AIN0_GND  = 0b100, // AINP = AIN0 and AINN = GND
    ADS1115_MUX_AIN1_GND  = 0b101, // AINP = AIN1 and AINN = GND
    ADS1115_MUX_AIN2_GND  = 0b110, // AINP = AIN2 and AINN = GND
    ADS1115_MUX_AIN3_GND  = 0b111  // AINP = AIN3 and AINN = GND
} ads1115Mux_t;


typedef union
{
    struct
    {
        uint8_t opStatus        : 1;
        uint8_t mux             : 3;
        uint8_t pga             : 3;
        uint8_t mode            : 1;
        uint8_t dataRate        : 3;   
        uint8_t compMode        : 1;
        uint8_t compPolarity    : 1;
        uint8_t compLatch       : 1;
        uint8_t compQueue       : 2;
    };
    uint8_t bytes[ADS1115_CONFIG_REGISTER_SIZE];
}ads1115ConfigRegister_t;

typedef union
{
    uint16_t value;
    uint8_t bytes[ADS1115_CONVERSION_REGISTER_SIZE];
}ads1115ConversionRegister_t;


class ADS1115
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
    ADS1115();

    /**
     * @brief Destructor for the ADS1115 class.
     * 
     * Cleans up resources used by the ADS1115 instance.
     */
    ~ADS1115();

    /**
     * @brief Tests the functionality of the ADS1115 class.
     * 
     * This method is used for debugging and verifying the class implementation.
     */
    void testClass(void);

    /**
     * @brief Retrieves the latest reading from the ADS1115 device.
     * 
     * This method fetches the most recent analog-to-digital conversion result.
     */
    retVal_t getLatestReading(ads1115ConversionRegister_t * regPtr);

    void init_ads1115(void);
    retVal_t getConfiguration(ads1115ConfigRegister_t * configPtr);
    retVal_t setConfiguration(ads1115ConfigRegister_t * configPtr);

    private:

    retVal_t read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
    retVal_t write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
    retVal_t queueWait_ads1115I2cObject( i2c_handler_t ** i2cObjPtr);

};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // ADS1115_HPP