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
#include "common.h"
#include "i2c_task.h"
#include "ads1115.h"


/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
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

#define ADS1115_WRITE                                   (ADS1115_ADDRESS & ~(ADS1115_WRITE_BIT))
#define ADS1115_READ                                   (ADS1115_ADDRESS | (ADS1115_READ_BIT))




/************************************
 * PRIVATE TYPEDEFS
 ************************************/


typedef struct
{
    uint8_t pointerReg[ADS1115_POINTER_REGISTER_SIZE];                  /* write only   */
    uint8_t conversionReg[ADS1115_CONVERSION_REGISTER_SIZE];            /*  read only   */
    ads1115ConfigRegister_t configReg;
    uint8_t loThreshReg[ADS1115_LO_THRESH_REGISTER_SIZE];               /* read/write   */
    uint8_t hiThreshReg[ADS1115_HI_THRESH_REGISTER_SIZE];               /* read/write   */
}ads1115_RegisterMap_t;



/************************************
 * STATIC VARIABLES
 ************************************/
static ads1115_RegisterMap_t ads1115CfgObj;
static const char *TAG = "ads1115";
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static errType_t read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
static errType_t write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
static errType_t queue_ads1115I2cObject( i2c_handler_t ** i2cObjPtr);

/************************************
 * STATIC FUNCTIONS
 ************************************/
static errType_t queue_ads1115I2cObject( i2c_handler_t ** i2cObjPtr)
{
    errType_t errRet = ERR_UNKNOWN;

    /* send to queue*/
    if( NULL == i2cQueueHdl)
    {
        errRet = ERR_NULL_POINTER;
    }

    if(ERR_NONE == errRet && pdTRUE != xQueueSendToBack( i2cQueueHdl, ( void *) i2cObjPtr , ( TickType_t ) 10 ))
    {
        /* failed to send to queue*/
        /* Failed to post the message, even after 10 ticks. */
        errRet = ERR_QUEUE_FAIL;
    }

    /*  Wait for i2c task to notify of completion    */
    if(ERR_NONE == errRet && 0u == ulTaskNotifyTake(pdFALSE, ( TickType_t ) 1000))
    {
        /* failed to get notification from queue within timeout, set failure*/
        errRet = ERR_NOTIFY_TIMEOUT;
    }

    return errRet;
}

static errType_t read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr)
{
    errType_t errRet = ERR_NONE;
    i2c_handler_t i2cObj;
    i2c_handler_t * i2cObjPtr = &i2cObj;

    i2cObj.cmd = i2c_cmd_link_create();
    i2cObj.taskHdl = xTaskGetCurrentTaskHandle();

    /*  check for null pointers */
    if(NULL == i2cObj.cmd)
    {
        errRet = ERR_NULL_POINTER;
    }

    /*  start i2c command     */
    if(ERR_NONE == errRet && ESP_OK != i2c_master_start(i2cObj.cmd))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /*  address ads1115 device with intention to write     */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_WRITE, I2C_ACK_CHECK_DISABLE))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }
    
    /*  write config register address to pointer register       */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_CONFIG_REGISTER, I2C_ACK_CHECK_DISABLE))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /*  address ads1115 device with intention to read    */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_READ, I2C_ACK_CHECK_DISABLE))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /* read configuration register   */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_read(i2cObj.cmd, configPtr, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_ACK))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /* i2c command stop  */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_stop(i2cObj.cmd))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    if(ERR_NONE == errRet)
    {
        /* queue commands    */
        errRet = queue_ads1115I2cObject(&i2cObjPtr);
    }

    /* delete command object */
    i2c_cmd_link_delete(i2cObj.cmd);
    
    return errRet;
}

static errType_t write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr)
{
    errType_t errRet;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    /*  address ads1115 device with intention to write     */
    i2c_master_write_byte(cmd, ADS1115_ADDRESS | ADS1115_WRITE_BIT, I2C_ACK_CHECK_DISABLE);
    
    /*  write config register address to pointer register       */
    i2c_master_write_byte(cmd, ADS1115_CONFIG_REGISTER, I2C_ACK_CHECK_DISABLE);


    /*  address ads1115 device with intention to write again    */
    i2c_master_write_byte(cmd, ADS1115_ADDRESS | ADS1115_WRITE_BIT, I2C_ACK_CHECK_DISABLE);
    
    /* write to configuration registers   */
    i2c_master_write(cmd, configPtr->data, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_ACK);

    i2c_master_stop(cmd);

    /*      send to i2c handler */
    if(ESP_OK == i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS))
    {
        errRet = ERR_NONE;
    }
    else
    {
        errRet = ERR_FAIL;
    }
    i2c_cmd_link_delete(cmd);

    return errRet;
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void init_ads1115(void)
{
    errType_t errRet = ERR_NONE;
    /* read config registers object */
    errRet = read_ads1115ConfigRegisters(&ads1115CfgObj.configReg);

    if(ERR_NONE != errRet)
    {
        ESP_LOGI(TAG, "Config Reg Read Fail: %i", errRet);   
    }
}

errType_t get_ads1115Configuration(ads1115ConfigRegister_t * configPtr)
{
    errType_t errRet = ERR_UNKNOWN;

    if(configPtr != NULL)
    {
        /*  last read config values should persist */
        memcpy(configPtr, &ads1115CfgObj.configReg, ADS1115_CONFIG_REGISTER_SIZE);

        /*   */
        errRet = ERR_NONE;
    }
    else
    {
        /*   */
        errRet = ERR_NULL_POINTER;
    }

    return errRet;
}

errType_t set_ads1115Configuration(ads1115ConfigRegister_t * configPtr)
{
    errType_t errRet = ERR_NONE;
    ads1115ConfigRegister_t tempConfig;

    if(configPtr == NULL)
    {
       /*   */
        errRet = ERR_NULL_POINTER;
    }

    if(errRet == ERR_NONE)
    {
        /*  copy config values into pointer */
        errRet = write_ads1115ConfigRegisters(configPtr);
    }

    if(errRet == ERR_NONE)
    {
        /*  read value from registers to confirm write */
        errRet = read_ads1115ConfigRegisters(&ads1115CfgObj.configReg);
    }


    if(errRet == ERR_NONE)
    {
        /* compare memory  */
        if(0 != memcmp(&ads1115CfgObj.configReg, configPtr, sizeof(ads1115ConfigRegister_t)))
        {
            errRet = ERR_FAIL;
        }
    }

    return errRet;
}