/**
 ********************************************************************************
 * @file    ads1115.c
 * @author  Hugo Quiroz
 * @date    2024-10-08 11:50:19
 * @brief   description
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "i2c_handler.h"
#include "ads1115.h"
#include "typedefs.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define GND_ADDR_PIN                            (0x90)              /* ADDR PIN connected to GND Pin */
#define VDD_ADDR_PIN                            (0x91)              /* ADDR PIN connected to VDD Pin */
#define SDA_ADDR_PIN                            (0x92)              /* ADDR PIN connected to SDA Pin */
#define SCL_ADDR_PIN                            (0x93)              /* ADDR PIN connected to SCL Pin */
#define ADS1115_ADDRESS                         (GND_ADDR_PIN)      /* select one of the above addresses */
    


#define ADS1115_POINTER_REGISTER_SIZE            (1u)
#define ADS1115_CONVERSION_REGISTER              (0x00)
#define ADS1115_CONVERSION_REGISTER_SIZE         (2u)
#define ADS1115_CONFIG_REGISTER                  (0x01)
#define ADS1115_CONFIG_REGISTER_SIZE             (2u)
#define ADS1115_LO_THRESH_REGISTER               (0x02)
#define ADS1115_LO_THRESH_REGISTER_SIZE          (2u)
#define ADS1115_HI_THRESH_REGISTER               (0x03)
#define ADS1115_HI_THRESH_REGISTER_SIZE          (2u)

#define ADS1115_OS_CFG_BIT                   (15u)
#define ADS1115_MUX_CFG_BITS                 (12u)
#define ADS1115_PGA_CFG_BITS                 (9u)
#define ADS1115_MODE_CFG_BIT                 (8u)
#define ADS1115_DR_CFG_BIT                   (5u)
#define ADS1115_COMP_MODE_CFG_BIT            (4u)
#define ADS1115_COMP_POL_CFG_BIT             (3u)
#define ADS1115_COMP_LATCH_CFG_BIT           (2u)
#define ADS1115_COMP_QUEUE_CFG_BIT           (0u)

#define ADS1115_OS_CFG_MASK                  (1u)
#define ADS1115_MUX_CFG_MASK                 (3u)
#define ADS1115_PGA_CFG_MASK                 (3u)
#define ADS1115_MODE_CFG_MASK                (1u)
#define ADS1115_DR_CFG_MASK                  (3u)
#define ADS1115_COMP_MODE_CFG_MASK           (1u)
#define ADS1115_COMP_POL_CFG_MASK            (1u)
#define ADS1115_COMP_LATCH_CFG_MASK          (1u)
#define ADS1115_COMP_QUEUE_CFG_MASK          (2u)




/************************************
 * PRIVATE TYPEDEFS
 ************************************/

typedef struct
{
    uint8_t pointerReg[ADS1115_POINTER_REGISTER_SIZE];                 /* write only */
    uint8_t conversionReg[ADS1115_CONVERSION_REGISTER_SIZE];               /*  read only*/
    uint8_t configReg[ADS1115_CONFIG_REGISTER_SIZE];                   /* read/write   */
    uint8_t loThreshReg[ADS1115_LO_THRESH_REGISTER_SIZE];                 /* read/write   */
    uint8_t hiThreshReg[ADS1115_HI_THRESH_REGISTER_SIZE];                 /* read/write   */
}ads1115_Config_t;

/************************************
 * STATIC VARIABLES
 ************************************/
static ads1115_Config_t ads1115CfgObj;
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void read_ads1115Registers(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void read_ads1115Registers(void)
{
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    /*  queue up a write command     */
    i2c_master_write_byte(cmd, ADS1115_ADDRESS | I2C_MASTER_WRITE, I2C_ACK_CHECK_DIS);
    /*  write config register address to pointer register       */
    i2c_master_write_byte(cmd, ADS1115_CONFIG_REGISTER, I2C_ACK_CHECK_DIS);
    i2c_master_read(cmd, ads1115CfgObj.configReg, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void init_ads1115(void)
{

    /* read and poppulate config registers object */
    read_ads1115Registers();

    /* init adc in single shot mode by default */

}

errType_t get_ads1115ChannelValue(uint8_t ch, float *value)
{
    errType_t errRet = ERR_FAIL;



    return errRet;
}

errType_t reset_ads1115(void)
{

}