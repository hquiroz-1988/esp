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
constexpr uint8_t ADS1115_MAX_CHANNELS = 0x48; // Default I2C address for ADS1115

#define ADS1115_POINTER_REGISTER_SIZE                   (1u)
#define ADS1115_CONVERSION_REGISTER_SIZE                (2u)
#define ADS1115_CONFIG_REGISTER_SIZE                    (2u)
#define ADS1115_LO_THRESH_REGISTER_SIZE                 (2u)
#define ADS1115_HI_THRESH_REGISTER_SIZE                 (2u)

/*******************************************************************************
 * CLASSES & TYPEDEFS
*******************************************************************************/
//!TODO: to be deleted once ADS1115 is refactored/fully implemented
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

enum class OperationStatus_t : uint8_t
{
    ConversionInProgress = 0, // Conversion in progress
    NoConversionInProgress = 1 // Conversion ready
};

enum class ADS1115Mux_t : uint8_t
{
    AIN0_AIN1 = 0b000, // AINP = AIN0 and AINN = AIN1 (default)
    AIN0_AIN3 = 0b001, // AINP = AIN0 and AINN = AIN3
    AIN1_AIN3 = 0b010, // AINP = AIN1 and AINN = AIN3
    AIN2_AIN3 = 0b011, // AINP = AIN2 and AINN = AIN3
    AIN0_GND  = 0b100, // AINP = AIN0 and AINN = GND
    AIN1_GND  = 0b101, // AINP = AIN1 and AINN = GND
    AIN2_GND  = 0b110, // AINP = AIN2 and AINN = GND
    AIN3_GND  = 0b111  // AINP = AIN3 and AINN = GND
};

enum class ADS1115PGA_t : uint8_t
{
    FSR_6_144V = 0b000, // ±6.144V
    FSR_4_096V = 0b001, // ±4.096V
    FSR_2_048V = 0b010, // ±2.048V (default)
    FSR_1_024V = 0b011, // ±1.024V
    FSR_0_512V = 0b100, // ±0.512V
    FSR_0_256V_1 = 0b101, // ±0.256V
    FSR_0_256V_2 = 0b110, // ±0.256V
    FSR_0_256V_3 = 0b111  // ±0.256V
};

enum class ADS1115Mode_t : uint8_t
{
    Continuous = 0b0, // Continuous-conversion mode
    SingleShot = 0b1  // Single-shot mode or power-down state (default)
};

enum class ADS1115DataRate_t : uint8_t
{
    SPS_8   = 0b000, // 8 samples per second
    SPS_16  = 0b001, // 16 samples per second
    SPS_32  = 0b010, // 32 samples per second
    SPS_64  = 0b011, // 64 samples per second
    SPS_128 = 0b100, // 128 samples per second (default)
    SPS_250 = 0b101, // 250 samples per second
    SPS_475 = 0b110, // 475 samples per second
    SPS_860 = 0b111  // 860 samples per second
};

enum class ADS1115CompMode_t : uint8_t
{
    Traditional = 0b0, // Traditional comparator (default)
    Window      = 0b1  // Window comparator
};

enum class ADS1115CompPolarity_t : uint8_t
{
    ActiveLow  = 0b0, // Comparator output is active low (default)
    ActiveHigh = 0b1  // Comparator output is active high
};

enum class ADS1115CompLatch_t : uint8_t
{
    NonLatching = 0b0, // ALERT/RDY pin does not latch when asserted (default)
    Latching    = 0b1  // ALERT/RDY pin remains latched until conversion data are read or SMBus alert response
};

enum class ADS1115CompQueue_t : uint8_t
{
    AssertAfterOneConversion   = 0b00, // Assert ALERT/RDY after one conversion
    AssertAfterTwoConversions  = 0b01, // Assert ALERT/RDY after two conversions
    AssertAfterFourConversions = 0b10, // Assert ALERT/RDY after four conversions
    DisableComparator          = 0b11  // Disable comparator, ALERT/RDY pin high-impedance (default)
};

struct ADS1115_Config
{
    OperationStatus_t opStatus;
    ADS1115Mux_t mux;
    ADS1115PGA_t pga;
    ADS1115Mode_t mode;
    ADS1115DataRate_t dataRate;
    ADS1115CompMode_t compMode;
    ADS1115CompPolarity_t compPolarity;
    ADS1115CompLatch_t compLatch;
    ADS1115CompQueue_t compQueue;
};



using ADS1115_Config_t = struct ADS1115_Config;

enum class Conversion_t
{
    SingleEnded,
    Differential
};

enum class Channel_t
{
    AIN0,
    AIN1,
    AIN2,
    AIN3,
    AIN0_AIN1,
    AIN2_AIN3
};

struct ADS1115_Conversion
{
    Conversion_t type;
    uint16_t value;
    ADS1115Mux_t channel;
};

using ADS1115_Conversion_t = struct ADS1115_Conversion;


struct ADS1115_Comparator
{
    uint16_t lowThreshold;
    uint16_t highThreshold;
    Channel_t channel;
};

using ADS1115_Comparator_t = struct ADS1115_Comparator;

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
     * @brief Configures the ADS1115 device with the specified settings.
     *
     * This method applies the provided configuration object to the ADS1115 device.
     * Use this to set up the input multiplexer, gain, mode, data rate, and comparator options.
     *
     * @param configObj Reference to an ADS1115_Config_t structure containing the desired configuration.
     * @return Status_t Returns the status of the configuration operation.
     */
    Status_t configure(const ADS1115_Config_t & configObj);


    /**
     * @brief Performs a single-ended ADC measurement using the provided conversion object.
     *
     * This method initiates a single-ended analog-to-digital conversion on the ADS1115 device.
     * The conversion result and channel information are stored in the supplied ADS1115_Conversion_t object.
     *
     * @param convObj Reference to an ADS1115_Conversion_t structure to hold the conversion result and channel.
     * @return Status_t Returns the status of the read operation.
     */
    Status_t readADC_SingleEnded(ADS1115_Conversion_t & convObj);


    /**
     * @brief Performs a differential ADC measurement using the provided conversion object.
     *
     * This method initiates a differential analog-to-digital conversion on the ADS1115 device.
     * The conversion result and channel information are stored in the supplied ADS1115_Conversion_t object.
     *
     * @param convObj Reference to an ADS1115_Conversion_t structure to hold the conversion result and channel.
     * @return Status_t Returns the status of the read operation.
     */
    Status_t readADC_Differential(ADS1115_Conversion_t & convObj);

    /**
     * @brief Starts a single-ended comparator operation on the ADS1115.
     *
     * This method configures the ADS1115 to compare the selected analog input channel
     * against the threshold values provided in the ADS1115_Comparator_t object.
     * The comparator will trigger according to the configuration set in the device.
     *
     * @param compObj Reference to an ADS1115_Comparator_t structure containing the channel and threshold values.
     * @return Status_t Returns the status of the comparator start operation.
     */
    Status_t startComparator_SingleEnded(const ADS1115_Comparator_t & compObj);

    /**
     * @brief Starts a differential comparator operation on the ADS1115.
     *
     * This method configures the ADS1115 to compare the selected differential input pair
     * against the threshold values provided in the ADS1115_Comparator_t object.
     * The comparator will trigger according to the configuration set in the device.
     *
     * @param compObj Reference to an ADS1115_Comparator_t structure containing the channel and threshold values.
     * @return Status_t Returns the status of the comparator start operation.
     */
    Status_t startComparator_Differential(const ADS1115_Comparator_t & compObj);

    /**
     * @brief Stops the comparator operation on the ADS1115 device for the specified comparator object.
     *
     * This method disables the comparator functionality for the provided ADS1115_Comparator_t object,
     * halting any ongoing comparator-triggered conversions or alerts for the specified channel.
     * Use this to return the ADS1115 to normal ADC operation after comparator use.
     *
     * @param compObj Reference to an ADS1115_Comparator_t structure associated with the comparator to stop.
     * @return Status_t Returns the status of the stop operation.
     */
    Status_t stopComparator(const ADS1115_Comparator_t & compObj);




    /**
     * @brief Retrieves the latest reading from the ADS1115 device.
     * 
     * This method fetches the most recent analog-to-digital conversion result.
     */
    Status_t getLatestReading(ads1115ConversionRegister_t * regPtr);

    void init_ads1115(void);
    Status_t getConfiguration(ads1115ConfigRegister_t * configPtr);
    Status_t setConfiguration(ads1115ConfigRegister_t * configPtr);

    private:

    Status_t read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
    Status_t write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
    Status_t queueWait_ads1115I2cObject( i2c_handler_t ** i2cObjPtr);

};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // ADS1115_HPP