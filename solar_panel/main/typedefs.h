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

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
    ERR_NONE,
    ERR_UNKNOWN,
    ERR_FAIL,
    ERR_NULL_POINTER,
    ERR_OUT_OF_BOUNDS,

    TOTAL_ERR_TYPES
}errType_t;

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