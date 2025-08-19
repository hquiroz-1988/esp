/**
 ********************************************************************************
 * @file    bus_voltage.h
 * @author  Hugo Quiroz
 * @date    2024-09-27 10:24:18
 * @brief   description
 ********************************************************************************
 */

#ifndef BUS_VOLTAGE_H
#define BUS_VOLTAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
extern "C" 
{

}

#include "typedefs.h"
#include "ads1115.hpp"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
class BusVoltage
{
public:
    BusVoltage(ADS1115 & _ads1115);
    ~BusVoltage() = default;

    /** @brief  Initializes Bus Voltage module including
     *  task and variables
     *
     *  @param void 
     *  @return void 
     */
    Status_t init(void);

    /** @brief  Returns the filtered voltage from the
     *  bus voltage module.
     *
     *  @param value - pointer to a float value that will return
     *  the voltage value
     *  @return Status_t - returns error type or success
     */
    Status_t getFilteredVoltage(float & value);

    private:
    // Add private members if needed
    ADS1115 & ads1115;
    ADS1115_Conversion_t convObj;
};

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

#ifdef __cplusplus
}
#endif

#endif //BUS_VOLTAGE_H