/**
 ********************************************************************************
 * @file    i2c_task.h
 * @author  Hugo Quiroz
 * @date    2024-09-06 15:42:20
 * @brief   description
 ********************************************************************************
 */

#ifndef I2C_TASK_H
#define I2C_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

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

/** @brief  Initializes i2c handler module including
 *  task and variables
 *
 *  @param void 
 *  @return void 
 */
void init_i2cHandler(void);


#ifdef __cplusplus
}
#endif

#endif //I2C_TASK_H