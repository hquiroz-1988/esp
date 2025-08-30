/**
 ********************************************************************************
 * @file    ads1115_regs.h
 * @author  Hugo Quiroz
 * @date    2024-10-08 11:51:00
 * @brief   description
 ********************************************************************************
 */

#ifndef ADS1115_REGS_H
#define ADS1115_REGS_H


/************************************
 * INCLUDES
 ************************************/
#include "typedefs.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define GND_ADDR_PIN                                    (0x48)              /* ADDR PIN connected to GND Pin */
#define VDD_ADDR_PIN                                    (0x49)              /* ADDR PIN connected to VDD Pin */
#define SDA_ADDR_PIN                                    (0x4A)              /* ADDR PIN connected to SDA Pin */
#define SCL_ADDR_PIN                                    (0x4B)              /* ADDR PIN connected to SCL Pin */
#define ADS1115_ADDRESS_SHIFT                           (1u)
#define ADS1115_ADDRESS_MASK                            (0x7)
#define ADS1115_ADDRESS                                 ((GND_ADDR_PIN & ADS1115_ADDRESS_MASK) << ADS1115_ADDRESS_SHIFT)

#define ADS1115_WRITE_BIT                               (0x0)
#define ADS1115_READ_BIT                                (0x1)

#define ADS1115_CONVERSION_REGISTER                     (0x00)
#define ADS1115_CONFIG_REGISTER                         (0x01)
#define ADS1115_LO_THRESH_REGISTER                      (0x02)
#define ADS1115_HI_THRESH_REGISTER                      (0x03)

#define ADS1115_POINTER_REGISTER_SIZE                   (1u)
#define ADS1115_CONVERSION_REGISTER_SIZE                (2u)
#define ADS1115_CONFIG_REGISTER_SIZE                    (2u)
#define ADS1115_LO_THRESH_REGISTER_SIZE                 (2u)
#define ADS1115_HI_THRESH_REGISTER_SIZE                 (2u)

#define ADS1115_OS_CFG_BIT                              (15u)
#define ADS1115_MUX_CFG_BITS                            (12u)
#define ADS1115_PGA_CFG_BITS                            (9u)
#define ADS1115_MODE_CFG_BIT                            (8u)
#define ADS1115_DR_CFG_BIT                              (5u)
#define ADS1115_COMP_MODE_CFG_BIT                       (4u)
#define ADS1115_COMP_POL_CFG_BIT                        (3u)
#define ADS1115_COMP_LATCH_CFG_BIT                      (2u)
#define ADS1115_COMP_QUEUE_CFG_BIT                      (0u)

#define ADS1115_OS_CFG_MASK                             (1u)
#define ADS1115_MUX_CFG_MASK                            (3u)
#define ADS1115_PGA_CFG_MASK                            (3u)
#define ADS1115_MODE_CFG_MASK                           (1u)
#define ADS1115_DR_CFG_MASK                             (3u)
#define ADS1115_COMP_MODE_CFG_MASK                      (1u)
#define ADS1115_COMP_POL_CFG_MASK                       (1u)
#define ADS1115_COMP_LATCH_CFG_MASK                     (1u)
#define ADS1115_COMP_QUEUE_CFG_MASK                     (2u)

#define I2C_ACK_CHECK_DISABLE                           (false)
#define I2C_ACK_CHECK_ENABLE                            (true)
#define ADS1115_ACK_CHECK_STATUS                        (I2C_ACK_CHECK_DISABLE)//TODO: enable checking when ready to connect to device

#define ADS1115_WRITE                                   (ADS1115_ADDRESS & ~(ADS1115_WRITE_BIT))
#define ADS1115_READ                                    (ADS1115_ADDRESS | (ADS1115_READ_BIT))


/************************************
 * TYPEDEFS
 ************************************/
// Device address depends on which pin is connected: GND, VDD, SDA, or SCL
// To use a specific address, connect the ADDR pin of the ADS1115 to the corresponding pin.
enum class ADS1115_Address : uint8_t
{
    Device1 = (0b1001000 << 1), // ADDR connected to GND
    Device2 = (0b1001001 << 1), // ADDR connected to VDD
    Device3 = (0b1001010 << 1), // ADDR connected to SDA
    Device4 = (0b1001011 << 1)  // ADDR connected to SCL
};

enum class ADS1115_PointerRegister : uint8_t
{
    Conversion    = 0b00, // Conversion register
    Config        = 0b01, // Config register
    Lo_Threshold  = 0b10, // Lo_thresh register
    Hi_Threshold  = 0b11  // Hi_thresh register
};

enum class ADS1115_OperationalStatus_t : uint8_t
{
    Read_ConversionInProgress = 0,      // Conversion in progress
    Read_NoConversionInProgress = 1,    // Conversion ready
    Write_No_Effect = 0,                // Dont perform an action
    Write_StartSingleConversion = 1     // Start a Single Conversion
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
    ADS1115_OperationalStatus_t opStatus;
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


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#endif //ADS1115_REGS_H