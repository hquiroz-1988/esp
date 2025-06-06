/**
 ********************************************************************************
 * @file    ads1115.hpp
 * @author  hugo
 * @date    2025-06-04 23:18:08
 * @brief   
 ********************************************************************************
 */

#ifndef ADS1115_HPP
#define ADS1115_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "typedefs.h"

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
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // ADS1115_HPP