/**
 ********************************************************************************
 * @file    i2c_task.c
 * @author  Hugo Quiroz
 * @date    2024-09-06 15:40:43
 * @brief   This module handles the i2c management using an RTOS task.
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c_task.h"
#include "i2c_handler.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
static const char *TAG = "solar panel";

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
i2c_handler_t cmdObj;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/** @brief  I2C task that runs periodically
 * 
 * Will pop queue values 
 * 
 *  @param void 
 *  @return void 
 */
void i2c_Task(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void init_i2cHandler(void)
{
    /*  create i2c task */
    xTaskCreate(i2c_Task, "i2c_task", 1024, NULL, 5, NULL);

    /* create or initialize i2c object queue */
    
}

void i2c_Task(void)
{
    /* initialize i2c task */
    
    while (1) 
    {
        /* while i2c data exists send out data  */
        ESP_LOGI(TAG, "manage i2c\r\n");

        /* design i2c task, this might have multiple channels*/


        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}