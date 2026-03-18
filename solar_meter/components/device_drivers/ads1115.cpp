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
    Status_t statusRet = STATUS_UNKNOWN;

    /*! - check i2c Queue Handle and i2c object pointer are not null*/
    if( NULL == i2cQueueHdl && NULL != i2cObjPtr)
    {
        statusRet = STATUS_NULL_POINTER;
    }

    /*! - send i2c object pointer to queue  */
    if(STATUS_OKAY == statusRet && pdTRUE != xQueueSendToBack( i2cQueueHdl, ( void *) i2cObjPtr , ( TickType_t ) 10 ))// TODO: replace literals with macro or const
    {
        /*! - if message fails to queue after 10 ticks, return queue fail */
        statusRet = STATUS_QUEUE_FAIL;
    }

    /*! - Wait for i2c task to notify of completion    */
    if(STATUS_OKAY == statusRet && 0u == ulTaskNotifyTake(pdFALSE, ( TickType_t ) 1000)) // TODO: replace literals with macro or const
    {
        /* - if i2c task fails to notify within timeout return notify timeout fail*/
        statusRet = STATUS_NOTIFY_TIMEOUT;
    }

    return statusRet;
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
    Status_t statusRet = STATUS_OKAY;

    /*! - create i2c object handler */
    i2c_handler_t i2cObj;
    i2c_handler_t * i2cObjPtr = &i2cObj;

    /*! - create command link an  */
    i2cObj.cmd = i2c_cmd_link_create();
    i2cObj.taskHdl = xTaskGetCurrentTaskHandle();

    /*! - check for null pointers */
    if(NULL == i2cObj.cmd)
    {
        statusRet = STATUS_NULL_POINTER;
    }

    /*! - start i2c command     */
    if(STATUS_OKAY == statusRet && ESP_OK != i2c_master_start(i2cObj.cmd))
    {
        statusRet = STATUS_HAL_ERROR;
    }

    /*!  address ads1115 device with intention to write     */
    if( STATUS_OKAY == statusRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_WRITE, ADS1115_ACK_CHECK_STATUS))
    {
        statusRet = STATUS_HAL_ERROR;
    }
    
    /*!  write config register address to pointer register       */
    if( STATUS_OKAY == statusRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_CONFIG_REGISTER, ADS1115_ACK_CHECK_STATUS))
    {
        statusRet = STATUS_HAL_ERROR;
    }

    /*!  address ads1115 device with intention to read    */
    if( STATUS_OKAY == statusRet && 
        ESP_OK != i2c_master_write_byte(i2cObj.cmd, ADS1115_READ, ADS1115_ACK_CHECK_STATUS))
    {
        statusRet = STATUS_HAL_ERROR;
    }

    /*! read configuration register   */
    if( STATUS_OKAY == statusRet && 
        ESP_OK != i2c_master_read(i2cObj.cmd, configPtr->bytes, ADS1115_CONFIG_REGISTER_SIZE, I2C_MASTER_ACK))
    {
        statusRet = STATUS_HAL_ERROR;
    }

    /*! i2c command stop  */
    if( STATUS_OKAY == statusRet && 
        ESP_OK != i2c_master_stop(i2cObj.cmd))
    {
        statusRet = STATUS_HAL_ERROR;
    }

    if(STATUS_OKAY == statusRet)
    {
        /*! queue commands    */
        statusRet = queueWait_ads1115I2cObject(&i2cObjPtr);
    }

    /*! delete command object */
    i2c_cmd_link_delete(i2cObj.cmd);
    
    return statusRet;
}



Status_t ADS1115::writeConfigRegister(ADS1115_Config_t & configObj)
{
    Status_t statusRet;

    I2CTransfer_t transferObj;
    transferObj.devAddr = ADS1115_ADDRESS;
    transferObj.regAddr = ADS1115_CONFIG_REGISTER;
    transferObj.size = ADS1115_CONFIG_REGISTER_SIZE;
    transferObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    transferObj.ackType = I2CTransferAckType_t::MASTER_ACK;

    statusRet = configObjToBytes(configObj, transferObj.data);

    if(statusRet == STATUS_OKAY)
    {
        statusRet = write(transferObj);
    }

    return statusRet;
}

Status_t ADS1115::configObjToBytes(const ADS1115_Config_t & configObj, uint8_t * bytes)
{
    Status_t statusRet = STATUS_OKAY;

    if(bytes != nullptr)
    {
        /* convert config object to bytes for i2c transfer */
        bytes[0] = ( (static_cast<uint8_t>(configObj.opStatus) & 0x01) << 7 ) |
                    ( (static_cast<uint8_t>(configObj.mux) & 0x07) << 4 ) |
                    ( (static_cast<uint8_t>(configObj.pga) & 0x07) << 1 ) |
                    ( (static_cast<uint8_t>(configObj.mode) & 0x01) << 0 );

        bytes[1] = ( (static_cast<uint8_t>(configObj.dataRate) & 0x07) << 5 ) |
                    ( (static_cast<uint8_t>(configObj.compMode) & 0x01) << 4 ) |
                    ( (static_cast<uint8_t>(configObj.compPolarity) & 0x01) << 3 ) |
                    ( (static_cast<uint8_t>(configObj.compLatch) & 0x01) << 2 ) |
                    ( (static_cast<uint8_t>(configObj.compQueue) & 0x03) << 0 );
    }
    else
    {
        statusRet = STATUS_NULL_POINTER;
    }

    return statusRet;
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
ADS1115::ADS1115(Gpio & _gpio) : alertPin(_gpio)
{
    /* constructor implementation*/
    //!TODO: set callback for gpio alert pin, either here or initialize function
    alertPin.setCallback(staticWrapper, this);
}

ADS1115::~ADS1115()
{
    // Destructor implementation
}

void ADS1115::initialize(void)
{
    Status_t statusRet = STATUS_OKAY;

    /* set configuration */
    transferObj.devAddr = ADS1115_ADDRESS;
    transferObj.regAddr = ADS1115_CONFIG_REGISTER;
    transferObj.data = ads1115CfgObj.configReg.bytes;
    transferObj.size = ADS1115_CONFIG_REGISTER_SIZE;
    transferObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    transferObj.ackType = I2CTransferAckType_t::MASTER_ACK;

    statusRet = write(transferObj);

    /* read back configuration to verify */
    //!TODO: add code to read back and verify configuration

    /* 
        register the ISR callback here, since we want the ISR to have accesss to
        ADS1115 context.
     */
    registerCallback(IntType::gpio_isr_handler);
}

Status_t ADS1115::getConfiguration(ads1115ConfigRegister_t * configPtr)
{
    Status_t statusRet = STATUS_OKAY;

    if(configPtr != NULL)
    {
        /*  read value from registers to confirm write */
        statusRet = read_ads1115ConfigRegisters(configPtr);
    }
    else
    {
        /*   */
        statusRet = STATUS_NULL_POINTER;
    }

    return statusRet;
}


Status_t ADS1115::startSingleConversion(ADS1115_Config_t & configObj)
{
    Status_t retVal = STATUS_OKAY;

    /* write to address pointer register */
    setAddressPointerRegister(ADS1115_PointerRegister::Config);

    /* Update operation status to initiate single conversion */
    configObj.opStatus = ADS1115_OperationalStatus_t::Write_StartSingleConversion;

    /* then write to configuration register */
    writeConfigRegister(configObj);

    /* we might want to install the ISR callback to the channel here */

    return retVal;
}

Status_t ADS1115::getAlertPinStatus(bool & pinState)
{
    Status_t retVal = STATUS_OKAY;

    /* read gpio status of pin */
    // pinState = alertPin.get();

    return retVal;
}




/*!
 * \brief Set the ads1115Configuration object
 * 
 * \param configPtr 
 * \return Status_t 
 */
Status_t ADS1115::setConfiguration(ads1115ConfigRegister_t * configPtr)
{
    Status_t statusRet = STATUS_OKAY;

    if(configPtr == NULL)
    {
       /*   */
        statusRet = STATUS_NULL_POINTER;
    }

    if(statusRet == STATUS_OKAY)
    {
        /*  copy config values into pointer */
        statusRet = writeConfigRegister(configPtr);
    }

    if(statusRet == STATUS_OKAY)
    {
        /*  read value from registers to confirm write */
        statusRet = read_ads1115ConfigRegisters(&ads1115CfgObj.configReg);
    }


    if(statusRet == STATUS_OKAY)
    {
        /* compare memory  */
        if(0 != memcmp(&ads1115CfgObj.configReg, configPtr, sizeof(ads1115ConfigRegister_t)))
        {
            statusRet = STATUS_MEMCMP_FAIL;
        }
    }

    return statusRet;
}

Status_t ADS1115::getLatestReading(ads1115ConversionRegister_t * regPtr)
{
    /* write conversion addy to the pointer register    */
    Status_t statusRet = STATUS_OKAY;


    return statusRet;
}

Status_t ADS1115::setNotificationTaskHandle(TaskHandle_t taskHdl)
{
    Status_t retVal = STATUS_OKAY;

    if(taskHdl != nullptr)
    {
        this->notificationTaskHandle = taskHdl;
    }
    else
    {
        retVal = STATUS_NULL_POINTER;
    }

    return retVal;
}

Status_t ADS1115::waitForConversionComplete(void)
{
    Status_t retVal = STATUS_OKAY;

    /* wait for conversion complete notification from gpio isr handler */
    if(xTaskNotifyWait(0, ADS1115_NOTIFY_CONVERSION_COMPLETE, NULL, portMAX_DELAY) != pdTRUE)
    {
        retVal = STATUS_TIMEOUT;
    }

    return retVal;
}


void ADS1115::staticWrapper(void* context, void * arg) 
{
    //!TODO: pass notification but as argument to callback
    ADS1115 * instance = static_cast<ADS1115*>(context);
    /* call teh alert pin ISR and pass any arg needed   */
    instance->alertPinISR(arg);
}

void ADS1115::alertPinISR(void *arg)
{
    /* perform any actions we need to take if alert pin is triggered*/

    //!TODO: do we need to clear pin or anything like that?

    /* call any callbacks that might be registered for the specific channel or fault triggered */
    //!TODO: determine if arg contributes any valuable information for the callback
    //for now assume it came from a conversion complete but if we are using alert pin for
    // faults we need to check that configuration. In any case the most recent channel 
    //should have modified the callback function so we are going to call that if 
    // it exists.
    /* if callback exists, call that here */
    if(callback != nullptr
        && callbackContext != nullptr)
    {
        /* call the callback and pass the context and an argument */
        callback(callbackContext, arg);
    }
    else
    {
        /* perform something if we have nullptr */
        //!TODO: add logging to this module
        // ESP_LOGE("Gpio", "Pointer to Gpio::callback is nullptr\n");
    }
    
}