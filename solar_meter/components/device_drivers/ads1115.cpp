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
typedef struct
{
    uint8_t pointerReg[ADS1115_POINTER_REGISTER_SIZE];                  /**< pointer register is write only     */
    ads1115ConversionRegister_t conversionReg;                          /**< conversion register is read only   */
    ads1115ConfigRegister_t configReg;                                  /**< config register is read and write  */
    uint8_t loThreshReg[ADS1115_LO_THRESH_REGISTER_SIZE];               /**< loThresh register is read and write   */
    uint8_t hiThreshReg[ADS1115_HI_THRESH_REGISTER_SIZE];               /**< hiThresh is read and write   */
}ads1115_RegisterMap_t;

/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/
static ads1115_RegisterMap_t ads1115CfgObj;
static const char *TAG = "ads1115";
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
Status_t ADS1115::queueWait_ads1115I2cObject( i2c_handler_t ** i2cObjPtr)
{
    Status_t errRet = STATUS_UNKNOWN;

    /*! - check i2c Queue Handle and i2c object pointer are not null*/
    if( NULL == i2cQueueHdl && NULL != i2cObjPtr)
    {
        errRet = STATUS_NULL_POINTER;
    }

    /*! - send i2c object pointer to queue  */
    if(STATUS_OKAY == errRet && pdTRUE != xQueueSendToBack( i2cQueueHdl, ( void *) i2cObjPtr , ( TickType_t ) 10 ))// TODO: replace literals with macro or const
    {
        /*! - if message fails to queue after 10 ticks, return queue fail */
        errRet = STATUS_QUEUE_FAIL;
    }

    /*! - Wait for i2c task to notify of completion    */
    if(STATUS_OKAY == errRet && 0u == ulTaskNotifyTake(pdFALSE, ( TickType_t ) 1000)) // TODO: replace literals with macro or const
    {
        /* - if i2c task fails to notify within timeout return notify timeout fail*/
        errRet = STATUS_NOTIFY_TIMEOUT;
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
 * \return Status_t - returns succces or reason for failure of the function.
 */
Status_t ADS1115::read_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr)
{
    Status_t errRet = STATUS_OKAY;

    /*! - create i2c object handler */
    i2c_handler_t i2cObj;
    i2c_handler_t * i2cObjPtr = &i2cObj;

    /*! - create command link an  */
    i2cObj.cmd = i2c_cmd_link_create();
    i2cObj.taskHdl = xTaskGetCurrentTaskHandle();

    /*! - check for null pointers */
    if(NULL == i2cObj.cmd)
    {
        errRet = STATUS_NULL_POINTER;
    }

    /*! - start i2c command     */
    if(STATUS_OKAY == errRet && ESP_OK != i2c_master_start(i2cObj.cmd))
    {
        errRet = STATUS_HAL_ERROR;
    }

    /*!  address ads1115 device with intention to write     */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_WRITE, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = STATUS_HAL_ERROR;
    }
    
    /*!  write config register address to pointer register       */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_CONFIG_REGISTER, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = STATUS_HAL_ERROR;
    }

    /*!  address ads1115 device with intention to read    */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_READ, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = STATUS_HAL_ERROR;
    }

    /*! read configuration register   */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_read(i2cObj.cmd, configPtr->bytes, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_ACK))
    {
        errRet = STATUS_HAL_ERROR;
    }

    /*! i2c command stop  */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_stop(i2cObj.cmd))
    {
        errRet = STATUS_HAL_ERROR;
    }

    if(STATUS_OKAY == errRet)
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
 * \return Status_t - returns succces or reason for failure of the function.
 */
Status_t ADS1115::write_ads1115ConfigRegisters(ads1115ConfigRegister_t * configPtr)
{
    //TODO: refactor this function to perform retval checks

    Status_t errRet;
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
        errRet = STATUS_OKAY;
    }
    else
    {
        errRet = STATUS_HAL_ERROR;
    }

    /*! - delete the i2c command handle   */
    i2c_cmd_link_delete(cmd);

    return errRet;
}


/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/
/*!
 * \brief initializes the ads1115 ADC Module
 * 
 * Function reads the configuration registers of the ads1115,
 * these are used to poppulate the local ads1115 object.
 * 
 */
ADS1115::ADS1115()
{
    
    Status_t errRet = STATUS_OKAY;
    /*! - read config registers object */
    errRet = read_ads1115ConfigRegisters(&ads1115CfgObj.configReg);

    //TODO: determing if writing a default configuration is needed

    if(STATUS_OKAY != errRet)
    {
        ESP_LOGI(TAG, "Config Reg Read Fail: %i", errRet);   
    }
}

ADS1115::~ADS1115()
{
    // Destructor implementation
}

void ADS1115::init_ads1115(void)
{
    // static ADS1115 ads1115_instance;
    // Optionally, add further initialization or configuration here if needed
}

Status_t ADS1115::getConfiguration(ads1115ConfigRegister_t * configPtr)
{
    Status_t errRet = STATUS_OKAY;

    if(configPtr != NULL)
    {
        /*  read value from registers to confirm write */
        errRet = read_ads1115ConfigRegisters(configPtr);
    }
    else
    {
        /*   */
        errRet = STATUS_NULL_POINTER;
    }

    return errRet;
}


/*!
 * \brief Set the ads1115Configuration object
 * 
 * \param configPtr 
 * \return Status_t 
 */
Status_t ADS1115::setConfiguration(ads1115ConfigRegister_t * configPtr)
{
    Status_t errRet = STATUS_OKAY;

    if(configPtr == NULL)
    {
       /*   */
        errRet = STATUS_NULL_POINTER;
    }

    if(errRet == STATUS_OKAY)
    {
        /*  copy config values into pointer */
        errRet = write_ads1115ConfigRegisters(configPtr);
    }

    if(errRet == STATUS_OKAY)
    {
        /*  read value from registers to confirm write */
        errRet = read_ads1115ConfigRegisters(&ads1115CfgObj.configReg);
    }


    if(errRet == STATUS_OKAY)
    {
        /* compare memory  */
        if(0 != memcmp(&ads1115CfgObj.configReg, configPtr, sizeof(ads1115ConfigRegister_t)))
        {
            errRet = STATUS_MEMCMP_FAIL;
        }
    }

    return errRet;
}

//TODO: document function
Status_t ADS1115::getLatestReading(ads1115ConversionRegister_t * regPtr)
{
    /* write conversion addy to the pointer register    */
    Status_t errRet = STATUS_OKAY;
    i2c_handler_t i2cObj;
    i2c_handler_t * i2cObjPtr = &i2cObj;

    i2cObj.cmd = i2c_cmd_link_create();
    i2cObj.taskHdl = xTaskGetCurrentTaskHandle();

    /*  check for null pointers */
    if(NULL == i2cObj.cmd)
    {
        errRet = STATUS_NULL_POINTER;
    }

    /*  start i2c command     */
    if(STATUS_OKAY == errRet && ESP_OK != i2c_master_start(i2cObj.cmd))
    {
        errRet = STATUS_HAL_ERROR;
    }

    /*  address ads1115 device with intention to write     */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_WRITE, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = STATUS_HAL_ERROR;
    }
    
    /*  write conversion register address to pointer register       */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_CONVERSION_REGISTER, ADS1115_ACK_CHECK_STATUS))
    {
        errRet = STATUS_HAL_ERROR;
    }

    /* read conversion register    */


    /* i2c command stop  */
    if( STATUS_OKAY == errRet && 
        ESP_OK != i2c_master_stop(i2cObj.cmd))
    {
        errRet = STATUS_HAL_ERROR;
    }

    if(STATUS_OKAY == errRet)
    {
        /* queue commands    */
        errRet = queueWait_ads1115I2cObject(&i2cObjPtr);
    }

    /* delete command object */
    i2c_cmd_link_delete(i2cObj.cmd);

    return errRet;
}


