/**
 *******************************************************************************
 * @file    i2c_typedefs.hpp
 * @author  HQ
 * @date    2026-03-10 23:40:27
 * @brief   
 *******************************************************************************
 */

#ifndef I2C_TYPEDEFS_HPP
#define I2C_TYPEDEFS_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "typedefs.h"
#include "driver/i2c.h"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/

/**
 * @brief I2C transfer acknowledgment type
 */
enum class I2CTransferAckType_t
{
    MASTER_ACK = I2C_MASTER_ACK,             /*!< I2C ack for each byte read */
    MASTER_NACK = I2C_MASTER_NACK,           /*!< I2C nack for each byte read */
    MASTER_LAST_NACK = I2C_MASTER_LAST_NACK, /*!< I2C nack for the last byte*/
    MASTER_ACK_MAX = I2C_MASTER_ACK_MAX,
};

typedef struct
{
    uint8_t *data;
    size_t size;
    uint8_t devAddr;
    uint8_t regAddr;
    bool ackEn;
    I2CTransferAckType_t ackType;
} I2CTransfer_t;

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // I2C_TYPEDEFS_HPP