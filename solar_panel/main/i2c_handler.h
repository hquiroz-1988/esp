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
typedef struct
{
    xSemaphoreHandle smphrHdl;
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