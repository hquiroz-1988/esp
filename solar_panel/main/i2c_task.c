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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c_task.h"

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

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void i2c_task(void)
{
    /* initialize i2c task */

    
    while (1) 
    {
        /* while i2c data exists send out data  */
        ESP_LOGI(TAG, "manage i2c\r\n");

        // ESP_LOGI(TAG, "adc read fast:\r\n");

        // if (ESP_OK == adc_read_fast(adc_data, 100)) {
        //     for (x = 0; x < 100; x++) {
        //         printf("%d\n", adc_data[x]);
        //     }
        // }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}