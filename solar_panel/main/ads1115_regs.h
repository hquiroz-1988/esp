/**
 ********************************************************************************
 * @file    ads1115_regs.h
 * @author  Hugo Quiroz
 * @date    2024-10-08 11:51:00
 * @brief   description
 ********************************************************************************
 */

#ifndef ADS1115_REGS_H
#define ADS1115_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "typedefs.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define GND_ADDR_PIN                                    (0x48)              /* ADDR PIN connected to GND Pin */
#define VDD_ADDR_PIN                                    (0x49)              /* ADDR PIN connected to VDD Pin */
#define SDA_ADDR_PIN                                    (0x4A)              /* ADDR PIN connected to SDA Pin */
#define SCL_ADDR_PIN                                    (0x4B)              /* ADDR PIN connected to SCL Pin */
#define ADS1115_ADDRESS_SHIFT                           (1u)
#define ADS1115_ADDRESS_MASK                            (0x7)
#define ADS1115_ADDRESS                                 ((GND_ADDR_PIN & ADS1115_ADDRESS_MASK) << ADS1115_ADDRESS_SHIFT)

#define ADS1115_WRITE_BIT                               (0x0)
#define ADS1115_READ_BIT                                (0x1)

#define ADS1115_CONVERSION_REGISTER                     (0x00)
#define ADS1115_CONFIG_REGISTER                         (0x01)
#define ADS1115_LO_THRESH_REGISTER                      (0x02)
#define ADS1115_HI_THRESH_REGISTER                      (0x03)

#define ADS1115_POINTER_REGISTER_SIZE                   (1u)
#define ADS1115_CONVERSION_REGISTER_SIZE                (2u)
#define ADS1115_CONFIG_REGISTER_SIZE                    (2u)
#define ADS1115_LO_THRESH_REGISTER_SIZE                 (2u)
#define ADS1115_HI_THRESH_REGISTER_SIZE                 (2u)

#define ADS1115_OS_CFG_BIT                              (15u)
#define ADS1115_MUX_CFG_BITS                            (12u)
#define ADS1115_PGA_CFG_BITS                            (9u)
#define ADS1115_MODE_CFG_BIT                            (8u)
#define ADS1115_DR_CFG_BIT                              (5u)
#define ADS1115_COMP_MODE_CFG_BIT                       (4u)
#define ADS1115_COMP_POL_CFG_BIT                        (3u)
#define ADS1115_COMP_LATCH_CFG_BIT                      (2u)
#define ADS1115_COMP_QUEUE_CFG_BIT                      (0u)

#define ADS1115_OS_CFG_MASK                             (1u)
#define ADS1115_MUX_CFG_MASK                            (3u)
#define ADS1115_PGA_CFG_MASK                            (3u)
#define ADS1115_MODE_CFG_MASK                           (1u)
#define ADS1115_DR_CFG_MASK                             (3u)
#define ADS1115_COMP_MODE_CFG_MASK                      (1u)
#define ADS1115_COMP_POL_CFG_MASK                       (1u)
#define ADS1115_COMP_LATCH_CFG_MASK                     (1u)
#define ADS1115_COMP_QUEUE_CFG_MASK                     (2u)

#define I2C_ACK_CHECK_DISABLE                           (false)
#define I2C_ACK_CHECK_ENABLE                            (true)
#define ADS1115_ACK_CHECK_STATUS                        (I2C_ACK_CHECK_DISABLE)//TODO: enable checking when ready to connect to device

#define ADS1115_WRITE                                   (ADS1115_ADDRESS & ~(ADS1115_WRITE_BIT))
#define ADS1115_READ                                    (ADS1115_ADDRESS | (ADS1115_READ_BIT))


/************************************
 * TYPEDEFS
 ************************************/


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#ifdef __cplusplus
}
#endif

#endif //ADS1115_REGS_H