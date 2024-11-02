/**
 ********************************************************************************
 * @file    ads1115.c
 * @author  Hugo Quiroz
 * @date    2024-10-08 11:50:19
 * @brief   This module serves as an interface to the ads1115 ADC device. 
 *  The ADS1115 is a precision, low-power, 16-bit, I2Ccompatible,
 * analog-to-digital converters (ADCs). The ADS1115 device incorporate a low-drift 
 * voltage reference and an oscillator. The ADS1115 also incorporate a programmable 
 * gain amplifier (PGA) and a digital comparator. These features, along with a wide
 * operating supply range, make the ADS111x well suited for power- and 
 * space-constrained, sensormeasurement applications. The ADS111x perform 
 * conversions at data rates up to 860 samples per second (SPS). The PGA offers 
 * input ranges from ±256 mV to ±6.144 V, allowing precise large- and small-signal 
 * measurements. The ADS1115 features an input multiplexer (MUX) that allows two 
 * differential or four single-ended input measurements. Use the digital comparator
 *  in the ADS1114 and ADS1115 for under- and overvoltage detection. The ADS111x 
 * operate in either continuousconversion mode or single-shot mode. The devices
 * are automatically powered down after one conversion in single-shot mode; 
 * therefore, power consumption is significantly reduced during idle periods.
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "common.h"
#include "i2c_task.h"
#include "ads1115.h"
#include <string.h>


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
#define ADS1115_ACK_CHECK_STATUS                        (I2C_ACK_CHECK_DISABLE)//TODO: enable checking when ready to connect to device

#define ADS1115_WRITE                                   (ADS1115_ADDRESS & ~(ADS1115_WRITE_BIT))
#define ADS1115_READ                                    (ADS1115_ADDRESS | (ADS1115_READ_BIT))




/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/*!
 * \brief a typedef struct that contains the register map for the ads1115
 * 
 * This typedef struct contains all read and write registers for the
 * in the ads1115.
 */
typedef struct
{
    uint8_t pointerReg[ADS1115_POINTER_REGISTER_SIZE];                  /**< pointer register is write only     */
    ads1115ConversionRegister_t conversionReg;                          /**< conversion register is read only   */
    ads1115ConfigRegister_t configReg;                                  /**< config register is read and write  */
    uint8_t loThreshReg[ADS1115_LO_THRESH_REGISTER_SIZE];               /**< loThresh register is read and write   */
    uint8_t hiThreshReg[ADS1115_HI_THRESH_REGISTER_SIZE];               /**< hiThresh is read and write   */
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
static retVal_t read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
static retVal_t write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr);
static retVal_t queueWait_ads1115I2cObject( i2c_handler_t ** i2cObjPtr);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/*!
 * \brief Queues i2c object and waits for its return
 *  
 * Function takes a pointer to an i2c object pointer so that it can be sent to
 * the i2c task queue, function then waits for a notification from the queue to
 * continue.
 *
 * \param i2cObjPtr -Even if there is only one possible unified theory. it is just a
 *               set of rules and equations.
 * \return return value is NO_ERROR, NULL_POINTER, QUEUE_FAIL, or NOTIFY_TIMEOUT.
 */
static retVal_t queueWait_ads1115I2cObject( i2c_handler_t ** i2cObjPtr)
{
    retVal_t errRet = ERR_UNKNOWN;

    /*! - check i2c Queue Handle and i2c object pointer are not null*/
    if( NULL == i2cQueueHdl && NULL != i2cObjPtr)
    {
        errRet = ERR_NULL_POINTER;
    }

    /*! - send i2c object pointer to queue  */
    if(ERR_NONE == errRet && pdTRUE != xQueueSendToBack( i2cQueueHdl, ( void *) i2cObjPtr , ( TickType_t ) 10 ))// TODO: replace literals with macro or const
    {
        /*! - if message fails to queue after 10 ticks, return queue fail */
        errRet = ERR_QUEUE_FAIL;
    }

    /*! - Wait for i2c task to notify of completion    */
    if(ERR_NONE == errRet && 0u == ulTaskNotifyTake(pdFALSE, ( TickType_t ) 1000)) // TODO: replace literals with macro or const
    {
        /* - if i2c task fails to notify within timeout return notify timeout fail*/
        errRet = ERR_NOTIFY_TIMEOUT;
    }

    return errRet;
}

/*!
 * \brief reads ads1115 configuration registers
 * 
 * Function creates an i2c handler object, builds the the i2c command
 * then queues and waits for the command then deletes the object.
 * 
 * \param configPtr - pointer to configuration register struct that will be poppulated with
 * configuration register data after read.
 * \return retVal_t - returns succces or reason for failure of the function.
 */
static retVal_t read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr)
{
    retVal_t errRet = ERR_NONE;

    /*! - create i2c object handler */
    i2c_handler_t i2cObj;
    i2c_handler_t * i2cObjPtr = &i2cObj;

    /*! - create command link an  */
    i2cObj.cmd = i2c_cmd_link_create();
    i2cObj.taskHdl = xTaskGetCurrentTaskHandle();

    /*! - check for null pointers */
    if(NULL == i2cObj.cmd)
    {
        errRet = ERR_NULL_POINTER;
    }

    /*! - start i2c command     */
    if(ERR_NONE == errRet && ESP_OK != i2c_master_start(i2cObj.cmd))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /*!  address ads1115 device with intention to write     */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_WRITE, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }
    
    /*!  write config register address to pointer register       */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_CONFIG_REGISTER, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /*!  address ads1115 device with intention to read    */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_READ, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /*! read configuration register   */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_read(i2cObj.cmd, configPtr->bytes, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_ACK))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /*! i2c command stop  */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_stop(i2cObj.cmd))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    if(ERR_NONE == errRet)
    {
        /*! queue commands    */
        errRet = queueWait_ads1115I2cObject(&i2cObjPtr);
    }

    /*! delete command object */
    i2c_cmd_link_delete(i2cObj.cmd);
    
    return errRet;
}

/*!
 * \brief writes to the ads1115 configuration registers
 * 
 * This function uses config register pointer passed as argument to
 * write to ads1115 config registers.
 * 
 * \param configPtr - pointer to configuration register that contains the ads1115
 * configuration to be written to the ads1115.
 * \return retVal_t - returns succces or reason for failure of the function.
 */
static retVal_t write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr)
{
    //TODO: refactor this function to perform retval checks

    retVal_t errRet;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    /*! - address ads1115 device with intention to write */
    i2c_master_write_byte(cmd, ADS1115_ADDRESS | ADS1115_WRITE_BIT, ADS1115_ACK_CHECK_STATUS);
    
    /*! - write config register address to pointer register       */
    i2c_master_write_byte(cmd, ADS1115_CONFIG_REGISTER, ADS1115_ACK_CHECK_STATUS);


    /*! - address ads1115 device with intention to write again    */
    i2c_master_write_byte(cmd, ADS1115_ADDRESS | ADS1115_WRITE_BIT, ADS1115_ACK_CHECK_STATUS);
    
    /*! - write to configuration registers   */
    i2c_master_write(cmd, configPtr->bytes, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_ACK);

    i2c_master_stop(cmd);

    /*! - send to i2c handler */
    if(ESP_OK == i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS))
    {
        errRet = ERR_NONE;
    }
    else
    {
        errRet = ERR_FAIL;
    }

    /*! - delete the i2c command handle   */
    i2c_cmd_link_delete(cmd);

    return errRet;
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/*!
 * \brief initializes the ads1115 ADC Module
 * 
 * Function reads the configuration registers of the ads1115,
 * these are used to poppulate the local ads1115 object.
 * 
 */
void init_ads1115(void)
{
    retVal_t errRet = ERR_NONE;
    /*! - read config registers object */
    errRet = read_ads1115ConfigRegisters(&ads1115CfgObj.configReg);

    //TODO: determing if writing a default configuration is needed

    if(ERR_NONE != errRet)
    {
        ESP_LOGI(TAG, "Config Reg Read Fail: %i", errRet);   
    }
}

/*!
 * \brief Get the ads1115Configuration object
 * 
 * \param configPtr - pointer to configuration object that will be be poppulated
 * with the current ads1115 configuration. 
 * \return retVal_t - returns succces or reason for failure
 */
retVal_t get_ads1115Configuration(ads1115ConfigRegister_t * configPtr)
{
    retVal_t errRet = ERR_UNKNOWN;

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

/*!
 * \brief Set the ads1115Configuration object
 * 
 * \param configPtr 
 * \return retVal_t 
 */
retVal_t set_ads1115Configuration(ads1115ConfigRegister_t * configPtr)
{
    retVal_t errRet = ERR_NONE;

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

//TODO: document function
retVal_t get_ads1115LatestConversionRegister(ads1115ConversionRegister_t * regPtr)
{
    /* write conversion addy to the pointer register    */
    retVal_t errRet = ERR_NONE;
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
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_WRITE, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }
    
    /*  write conversion register address to pointer register       */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_CONVERSION_REGISTER, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    /* read conversion register    */


    /* i2c command stop  */
    if( ERR_NONE == errRet && 
        ESP_OK != i2c_master_stop(i2cObj.cmd))
    {
        errRet = ERR_I2C_CMD_FAIL;
    }

    if(ERR_NONE == errRet)
    {
        /* queue commands    */
        errRet = queueWait_ads1115I2cObject(&i2cObjPtr);
    }

    /* delete command object */
    i2c_cmd_link_delete(i2cObj.cmd);

    return errRet;
}