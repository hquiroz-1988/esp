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
    MASTER_ACK       = I2C_MASTER_ACK,             /*!< I2C ack for each byte read */
    MASTER_NACK      = I2C_MASTER_NACK,           /*!< I2C nack for each byte read */
    MASTER_LAST_NACK = I2C_MASTER_LAST_NACK, /*!< I2C nack for the last byte*/
    MASTER_ACK_MAX   = I2C_MASTER_ACK_MAX,
};

struct I2CTransfer_t
{
    uint8_t *data = nullptr;
    size_t size = 0;
    uint8_t devAddr = 0;
    uint8_t regAddr = 0;
    bool ackEn = false;
    I2CTransferAckType_t ackType = I2CTransferAckType_t::MASTER_NACK;
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // I2C_TYPEDEFS_HPP