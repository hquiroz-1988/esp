/**
 ********************************************************************************
 * @file    ads1115.cpp
 * @author  hugo
 * @date    2025-06-04 23:22:04
 * @brief   
 ********************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
extern "C" 
{
    #include "esp_log.h"
    #include "ads1115.h"
}

#include "ads1115.hpp"

/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS
 *******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTIONS
 *******************************************************************************/

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


/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/

ADS1115::ADS1115()
{
    // Constructor implementation
}

ADS1115::~ADS1115()
{
    // Destructor implementation
}

retVal_t ADS1115::getConfiguration(ads1115ConfigRegister_t * configPtr)
{
    retVal_t errRet = ERR_NONE;

    if(configPtr != NULL)
    {
        /*  read value from registers to confirm write */
        errRet = read_ads1115ConfigRegisters(configPtr);
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
retVal_t ADS1115::setConfiguration(ads1115ConfigRegister_t * configPtr)
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
retVal_t ADS1115::getLatestReading(ads1115ConversionRegister_t * regPtr)
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


