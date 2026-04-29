/**
 ********************************************************************************
 * @file    ina219_regs.hpp
 * @author  Hugo Quiroz
 * @date    2026-04-29
 * @brief   INA219 register definitions
 *          Reference: INA219 Datasheet (SBOS448G, Texas Instruments)
 ********************************************************************************
 */

#ifndef INA219_REGS_HPP
#define INA219_REGS_HPP

/************************************
 * INCLUDES
 ************************************/
#include "typedefs.h"
#include "i2c_typedefs.hpp"

/************************************
 * CONSTANTS
 ************************************/

// Configuration register reset value
constexpr uint16_t INA219_CONFIG_RESET_VALUE            = 0x399Fu;

// Configuration register bit positions
constexpr uint8_t  INA219_RST_CFG_BIT                  = 15u;  // Reset bit
constexpr uint8_t  INA219_BRNG_CFG_BIT                 = 13u;  // Bus voltage range
constexpr uint8_t  INA219_PG_CFG_BITS                  = 11u;  // PGA gain (bits 12:11)
constexpr uint8_t  INA219_BADC_CFG_BITS                = 7u;   // Bus ADC resolution/averaging (bits 10:7)
constexpr uint8_t  INA219_SADC_CFG_BITS                = 3u;   // Shunt ADC resolution/averaging (bits 6:3)
constexpr uint8_t  INA219_MODE_CFG_BITS                = 0u;   // Operating mode (bits 2:0)

// Configuration register bit masks
constexpr uint8_t  INA219_RST_CFG_MASK                 = 1u;
constexpr uint8_t  INA219_BRNG_CFG_MASK                = 1u;
constexpr uint8_t  INA219_PG_CFG_MASK                  = 3u;   // 2-bit field
constexpr uint8_t  INA219_BADC_CFG_MASK                = 0xFu; // 4-bit field
constexpr uint8_t  INA219_SADC_CFG_MASK                = 0xFu; // 4-bit field
constexpr uint8_t  INA219_MODE_CFG_MASK                = 7u;   // 3-bit field

// Bus Voltage register bit positions
constexpr uint8_t  INA219_CNVR_BIT                     = 1u;   // Conversion Ready flag
constexpr uint8_t  INA219_OVF_BIT                      = 0u;   // Math Overflow flag
constexpr uint8_t  INA219_BD_SHIFT                     = 3u;   // Bus voltage data starts at bit 3

// Physical LSB values
constexpr uint8_t  INA219_SHUNT_VOLTAGE_LSB_UV         = 10u;  // 10 µV per LSB
constexpr uint8_t  INA219_BUS_VOLTAGE_LSB_MV           = 4u;   // 4 mV per LSB

// ACK check (I2C_ACK_CHECK_DISABLE / I2C_ACK_CHECK_ENABLE defined in ads1115_regs.hpp)
constexpr bool     INA219_ACK_CHECK_STATUS              = false;

/************************************
 * TYPEDEFS
 ************************************/

// Device address is set by A1 and A0 pins (GND, VS, SDA, or SCL)
enum class INA219_Address : uint8_t
{
    Device1 = 0x40, // A1 = GND, A0 = GND (default)
    Device2 = 0x41, // A1 = GND, A0 = VS
    Device3 = 0x42, // A1 = GND, A0 = SDA
    Device4 = 0x43, // A1 = GND, A0 = SCL
    Device5 = 0x44, // A1 = VS,  A0 = GND
    Device6 = 0x45, // A1 = VS,  A0 = VS
    Device7 = 0x46, // A1 = VS,  A0 = SDA
    Device8 = 0x47  // A1 = VS,  A0 = SCL
};

enum class INA219_Register : uint8_t
{
    Config       = 0x00, // Configuration register (R/W, reset = 399Fh)
    ShuntVoltage = 0x01, // Shunt voltage register (R)
    BusVoltage   = 0x02, // Bus voltage register (R)
    Power        = 0x03, // Power register (R, reset = 0000h)
    Current      = 0x04, // Current register (R, reset = 0000h)
    Calibration  = 0x05  // Calibration register (R/W, reset = 0000h)
};

enum class INA219_RegisterSize : uint8_t
{
    Pointer      = 1,
    Config       = 2,
    ShuntVoltage = 2,
    BusVoltage   = 2,
    Power        = 2,
    Current      = 2,
    Calibration  = 2
};

// Bus Voltage Range (BRNG, bit 13)
enum class INA219_BusVoltageRange_t : uint8_t
{
    FSR_16V = 0b0, // 16V full-scale range
    FSR_32V = 0b1  // 32V full-scale range (default)
};

// PGA Gain and Range (PG, bits 12:11)
enum class INA219_PGAGain_t : uint8_t
{
    Gain_1_40mV  = 0b00, // Gain = /1,  range = ±40 mV
    Gain_2_80mV  = 0b01, // Gain = /2,  range = ±80 mV
    Gain_4_160mV = 0b10, // Gain = /4,  range = ±160 mV
    Gain_8_320mV = 0b11  // Gain = /8,  range = ±320 mV (default)
};

// ADC Resolution/Averaging (BADC and SADC, 4-bit fields)
enum class INA219_ADCResolution_t : uint8_t
{
    Res_9bit          = 0b0000, // 9-bit,   84 µs conversion time
    Res_10bit         = 0b0001, // 10-bit,  148 µs conversion time
    Res_11bit         = 0b0010, // 11-bit,  276 µs conversion time
    Res_12bit         = 0b0011, // 12-bit,  532 µs conversion time (default)
    Res_12bit_2samp   = 0b1001, // 12-bit,  2 samples,   1.06 ms
    Res_12bit_4samp   = 0b1010, // 12-bit,  4 samples,   2.13 ms
    Res_12bit_8samp   = 0b1011, // 12-bit,  8 samples,   4.26 ms
    Res_12bit_16samp  = 0b1100, // 12-bit,  16 samples,  8.51 ms
    Res_12bit_32samp  = 0b1101, // 12-bit,  32 samples,  17.02 ms
    Res_12bit_64samp  = 0b1110, // 12-bit,  64 samples,  34.05 ms
    Res_12bit_128samp = 0b1111  // 12-bit,  128 samples, 68.10 ms
};

// Operating Mode (MODE, bits 2:0)
enum class INA219_OperatingMode_t : uint8_t
{
    PowerDown              = 0b000, // Power-down
    ShuntVoltage_Triggered = 0b001, // Shunt voltage, triggered
    BusVoltage_Triggered   = 0b010, // Bus voltage, triggered
    ShuntBus_Triggered     = 0b011, // Shunt and bus, triggered
    ADC_Off                = 0b100, // ADC off (disabled)
    ShuntVoltage_Cont      = 0b101, // Shunt voltage, continuous
    BusVoltage_Cont        = 0b110, // Bus voltage, continuous
    ShuntBus_Cont          = 0b111  // Shunt and bus, continuous (default)
};

struct INA219_Config
{
    INA219_BusVoltageRange_t  busVoltageRange;
    INA219_PGAGain_t          pgaGain;
    INA219_ADCResolution_t    busADC;
    INA219_ADCResolution_t    shuntADC;
    INA219_OperatingMode_t    mode;
};
using INA219_Config_t = struct INA219_Config;

typedef struct
{
    uint8_t              *data;
    INA219_RegisterSize   size;
    INA219_Address        devAddr;
    INA219_Register       regAddr;
    bool                  ackEn;
    I2CTransferAckType_t  ackType;
} INA219_Transfer_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

#endif // INA219_REGS_HPP
