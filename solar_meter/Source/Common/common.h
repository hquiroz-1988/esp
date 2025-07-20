/**
 ********************************************************************************
 * @file    common.h
 * @author  Hugo Quiroz
 * @date    2024-10-09 19:26:54
 * @brief   description
 ********************************************************************************
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "freertos/FreeRTOS.h"
#include "typedefs.h"
#include "esp_log.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define ESP_MIN_PRIOITY (0)
#define ESP_LOW_PRIORITY (1)
#define ESP_NORMAL_PRIORITY (2)
#define ESP_HIGH_PRIORITY (3)
#define ESP_MAX_PRIORITY (configMAX_PRIORITIES - 1)

/************************************
 * TYPEDEFS
 ************************************/
#ifdef UNIT_TEST
bool FOREVER(void);
bool CHECK_POINTER_VALID(void *ptr);
#else
#define FOREVER() (1)
#define CHECK_POINTER_VALID(ptr) ((ptr) != nullptr)
#endif


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#ifdef __cplusplus
}
#endif

#endif //COMMON_H