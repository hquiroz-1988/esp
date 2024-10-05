/**
 ********************************************************************************
 * @file    i2c_handler.h
 * @author  Hugo Quiroz
 * @date    2024-10-04 13:05:57
 * @brief   description
 ********************************************************************************
 */

#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "freertos/semphr.h"
#include "driver/i2c.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/*  
    i2c_handler_t carries an i2c command along with a TaskHandle_t
    the Task handle shall be used to notify the task that the i2c
    command is complete 
*/
typedef struct
{
    TaskHandle_t taskHdl;
    i2c_cmd_handle_t cmd;
}i2c_handler_t;


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#ifdef __cplusplus
}
#endif

#endif //I2C_HANDLER_H