/**
 ********************************************************************************
 * @file    bus_voltage.c
 * @author  Hugo Quiroz
 * @date    2024-09-27 10:23:37
 * @brief   description
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "bus_voltage.hpp"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
static const char *TAG = "bus voltage";

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
static void voltage_Task(void *arg);

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void voltage_Task(void *arg)
{

    /* initialize voltage task */

   while (1) 
    {
        /* while i2c data exists send out data  */
        ESP_LOGI(TAG, "bus voltage task\r\n");

        /* design i2c task, this might have multiple channels*/


        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void init_BusVoltage(void)
{
    /*  create voltage task */
    xTaskCreate(voltage_Task, "voltage_task", 1024, NULL, 5, NULL);
}

Status_t get_filtered_voltage(float * value)
{
    Status_t retVal = STATUS_UNKNOWN;

    

    return retVal;
}




