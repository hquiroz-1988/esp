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
#include "typedefs.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/** @brief  Initializes Bus Voltage module including
 *  task and variables
 *
 *  @param void 
 *  @return void 
 */
void init_BusVoltage(void);

/** @brief  Returns the filtered voltage from the
 *  bus voltage module.
 *
 *  @param value - pointer to a float value that will return
 *  the voltage value
 *  @return errType_t - returns error type or success
 */
errType_t get_filtered_voltage(float * value);


#ifdef __cplusplus
}
#endif

#endif //BUS_VOLTAGE_H