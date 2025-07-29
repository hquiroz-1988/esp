/**
 ********************************************************************************
 * @file    typedefs.h
 * @author  Hugo Quiroz
 * @date    2024-09-27 10:28:03
 * @brief   description
 ********************************************************************************
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
    STATUS_OKAY,
    STATUS_UNKNOWN,
    STATUS_OS_ERROR,
    STATUS_QUEUE_FAIL,
    STATUS_QUEUE_FULL,
    STATUS_NOTIFY_TIMEOUT,
    STATUS_NULL_POINTER,
    STATUS_REINIT_ERROR,
    STATUS_OUT_OF_BOUNDS,
    STATUS_STACK_SIZE_TOO_SMALL,
    STATUS_IN_ISR_ERROR,
    STATUS_HAL_ERROR,
    STATUS_MEMCMP_FAIL,
    
    TOTAL_STATUS_TYPES
}Status_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#ifdef __cplusplus
}
#endif

#endif //TYPEDEFS_H