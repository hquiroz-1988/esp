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
#include "bus_voltage.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

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
void voltage_Task(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void init_BusVoltage(void)
{
    /*  create voltage task */
    xTaskCreate(voltage_Task, "voltage_task", 1024, NULL, 5, NULL);
}

errType_t get_filtered_voltage(float * value)
{
    errType_t retVal = UNKNOWN_ERROR;

    

    return retVal;
}

void voltage_Task(void)
{

    /* initialize voltage task */

   while (1) 
    {
        /* while i2c data exists send out data  */
        ESP_LOGI(TAG, "manage i2c\r\n");

        /* design i2c task, this might have multiple channels*/


        vTaskDelay(1000 / portTICK_RATE_MS);
    }
\
}


