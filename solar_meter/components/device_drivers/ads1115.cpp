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


/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/
static const char *TAG = "ads1115";
/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PROTECTED FUNCTIONS
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTIONS
 *******************************************************************************/
Status_t ADS1115::configObjToBytes(const ADS1115_Config_t &configObj, uint8_t *bytes)
{
    Status_t statusRet = STATUS_OKAY;

    if (bytes != nullptr)
    {
        /* convert config object to bytes for i2c transfer */
        bytes[0] = ((static_cast<uint8_t>(configObj.opStatus) & 0x01) << 7) |
                   ((static_cast<uint8_t>(configObj.mux) & 0x07) << 4) |
                   ((static_cast<uint8_t>(configObj.pga) & 0x07) << 1) |
                   ((static_cast<uint8_t>(configObj.mode) & 0x01) << 0);

        bytes[1] = ((static_cast<uint8_t>(configObj.dataRate) & 0x07) << 5) |
                   ((static_cast<uint8_t>(configObj.compMode) & 0x01) << 4) |
                   ((static_cast<uint8_t>(configObj.compPolarity) & 0x01) << 3) |
                   ((static_cast<uint8_t>(configObj.compLatch) & 0x01) << 2) |
                   ((static_cast<uint8_t>(configObj.compQueue) & 0x03) << 0);
    }
    else
    {
        statusRet = STATUS_NULL_POINTER;
    }

    return statusRet;
}

Status_t ADS1115::cachedRegisterToTransferObj(const ADS1115_Transfer_t & configObj, I2CTransfer_t & transferObj)
{
    Status_t retVal = STATUS_OKAY;

    /* copy the relevant fields from the cached register object to the transfer object for i2c communication */
    transferObj.devAddr = static_cast<uint8_t>(configObj.devAddr);
    transferObj.regAddr = static_cast<uint8_t>(configObj.regAddr);
    transferObj.size = static_cast<size_t>(configObj.size);
    transferObj.ackEn = configObj.ackEn;
    transferObj.ackType = configObj.ackType;
    transferObj.data = configObj.data;

    return retVal;
}


/*******************************************************************************
 * PUBLIC FUNCTIONS
 *******************************************************************************/
/*!
 * \brief initializes the ads1115 ADC Module
 *
 * Function reads the configuration registers of the ads1115,
 * these are used to poppulate the local ads1115 object.
 *
 */
ADS1115::ADS1115(Gpio &_gpio) : alertPin(_gpio)
{

    ESP_LOGI(TAG, "ADS1115 constructor called");
    /* constructor implementation*/
    if (alertPin.setCallback(staticWrapper, this) != STATUS_OKAY)
    {
        ESP_LOGE(TAG, "Failed to set callback for alert pin");
    }
    else 
    {
        ESP_LOGI(TAG, "Callback set for alert pin");
    }

    /* initialize the cached registers */
    configRegisterObj.devAddr = ADS1115_Address::Device1;
    configRegisterObj.regAddr = ADS1115_Register::Config;
    configRegisterObj.size = ADS1115_RegisterSize::Config;
    configRegisterObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    configRegisterObj.ackType = I2CTransferAckType_t::MASTER_ACK;
    configRegisterObj.data = new uint8_t[static_cast<size_t>(ADS1115_RegisterSize::Config)];

    conversionRegisterObj.devAddr = ADS1115_Address::Device1;
    conversionRegisterObj.regAddr = ADS1115_Register::Conversion;
    conversionRegisterObj.size = ADS1115_RegisterSize::Conversion;
    conversionRegisterObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    conversionRegisterObj.ackType = I2CTransferAckType_t::MASTER_ACK;
    conversionRegisterObj.data = new uint8_t[static_cast<size_t>(ADS1115_RegisterSize::Conversion)];

    loThresholdRegisterObj.devAddr = ADS1115_Address::Device1;
    loThresholdRegisterObj.regAddr = ADS1115_Register::Lo_Threshold;
    loThresholdRegisterObj.size = ADS1115_RegisterSize::Threshold;
    loThresholdRegisterObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    loThresholdRegisterObj.ackType = I2CTransferAckType_t::MASTER_ACK;
    loThresholdRegisterObj.data = new uint8_t[static_cast<size_t>(ADS1115_RegisterSize::Threshold)];

    hiThresholdRegisterObj.devAddr = ADS1115_Address::Device1;
    hiThresholdRegisterObj.regAddr = ADS1115_Register::Hi_Threshold;
    hiThresholdRegisterObj.size = ADS1115_RegisterSize::Threshold;
    hiThresholdRegisterObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    hiThresholdRegisterObj.ackType = I2CTransferAckType_t::MASTER_ACK;
    hiThresholdRegisterObj.data = new uint8_t[static_cast<size_t>(ADS1115_RegisterSize::Threshold)];

    ESP_LOGI(TAG, "ADS1115 initialization complete");
}

ADS1115::~ADS1115()
{
    // Destructor implementation
}

void ADS1115::initialize(void)
{
}

Status_t ADS1115::configure(const ADS1115_Config_t &configObj)
{
    Status_t retVal = STATUS_OKAY;

    /* write to configuration register */
    retVal = setConfigRegister(const_cast<ADS1115_Config_t &>(configObj));

    return retVal;
}

Status_t ADS1115::startSingleConversion(ADS1115_Config_t & configObj)
{
    Status_t retVal = STATUS_OKAY;

    /*  change config object to start single conversion */
    configObj.opStatus = ADS1115_OperationalStatus_t::Write_StartSingleConversion;

    /* write to configuration register to start conversion */
    retVal = setConfigRegister(configObj);

    return retVal;
}

Status_t ADS1115::getLatestConversion(float &value)
{
    Status_t retVal = STATUS_OKAY;

    /* read latest conversion */
    retVal = readConversionRegister(value);

    return retVal;
}


Status_t ADS1115::setConfigRegister(ADS1115_Config_t &configObj)
{
    Status_t statusRet;

    /* convert config object to bytes for i2c transfer */
    //!TODO: determine if config obj should be its own data structure
    configObjToBytes(configObj, configRegisterObj.data);

    /* convert cached register object to I2C transfer object */
    statusRet = cachedRegisterToTransferObj(configRegisterObj, txObj);

    if (statusRet == STATUS_OKAY)
    {
        /* write to I2C bus */
        statusRet = write(txObj);
    }

    return statusRet;
}


Status_t ADS1115::getConfigRegister(ADS1115_Config_t &configObj)
{
    Status_t statusRet;

    /* convert cached register object to I2C transfer object */
    statusRet = cachedRegisterToTransferObj(configRegisterObj, rxObj);

    if (statusRet == STATUS_OKAY)
    {
        /* read from I2C bus */
        statusRet = read(rxObj);

        if (statusRet == STATUS_OKAY)
        {
            /* convert bytes to config object */
            bytesToConfigObj(rxObj.data, configObj);
        }
    }

    return statusRet;
}


Status_t ADS1115::setLowThreshold(int16_t threshold)
{
    Status_t retVal = STATUS_OKAY;

    /* set low threshold */
    loThresholdRegisterObj.data[0] = (threshold >> 8) & 0xFF;
    loThresholdRegisterObj.data[1] = threshold & 0xFF;

    /* convert cached register to I2C transfer object */
    retVal = cachedRegisterToTransferObj(loThresholdRegisterObj, txObj); 

    if(retVal == STATUS_OKAY)
    {
        /* write to I2C bus */
        retVal = write(txObj);
    }
    
    return retVal;
}

Status_t ADS1115::getLowThreshold(int16_t &threshold)
{
    Status_t retVal = STATUS_OKAY;

    /* convert cached register object to I2C transfer object */
    retVal = cachedRegisterToTransferObj(loThresholdRegisterObj, rxObj);

    if (retVal == STATUS_OKAY)
    {
        /* read from I2C bus */
        retVal = read(rxObj);

        if (retVal == STATUS_OKAY)
        {
            /* convert bytes to threshold value */
            threshold = (static_cast<int16_t>(rxObj.data[0]) << 8) | static_cast<int16_t>(rxObj.data[1]);
        }
    }

    return retVal;
}

Status_t ADS1115::setHighThreshold(int16_t threshold)
{
    Status_t retVal = STATUS_OKAY;

    /* set high threshold */
    hiThresholdRegisterObj.data[0] = (threshold >> 8) & 0xFF;
    hiThresholdRegisterObj.data[1] = threshold & 0xFF;

    /* convert cached register to I2C transfer object */
    retVal = cachedRegisterToTransferObj(hiThresholdRegisterObj, txObj); 

    if(retVal == STATUS_OKAY)
    {
        /* write to I2C bus */
        retVal = write(txObj);
    }
    
    return retVal;
}

Status_t ADS1115::getHighThreshold(int16_t &threshold)
{
    Status_t retVal = STATUS_OKAY;

    /* convert cached register object to I2C transfer object */
    retVal = cachedRegisterToTransferObj(hiThresholdRegisterObj, rxObj);

    if (retVal == STATUS_OKAY)
    {
        /* read from I2C bus */
        retVal = read(rxObj);

        if (retVal == STATUS_OKAY)
        {
            /* convert bytes to threshold value */
            threshold = (static_cast<int16_t>(rxObj.data[0]) << 8) | static_cast<int16_t>(rxObj.data[1]);
        }
    }

    return retVal;
}

Status_t ADS1115::readConversionRegister(float &value)
{
    Status_t statusRet = STATUS_OKAY;

    I2CTransfer_t transferObj;
    transferObj.devAddr = static_cast<uint8_t>(ADS1115_Address::Device1);
    transferObj.regAddr = static_cast<uint8_t>(ADS1115_Register::Conversion);
    transferObj.size = static_cast<uint8_t>(ADS1115_RegisterSize::Conversion);
    transferObj.ackEn = static_cast<bool>(ADS1115_AckCheck::Disable);
    transferObj.ackType = I2CTransferAckType_t::MASTER_ACK;
    transferObj.data = new uint8_t[ADS1115_CONVERSION_REGISTER_SIZE];

    statusRet = read(transferObj);

    if (statusRet == STATUS_OKAY)
    {
        /* convert bytes to float value, this will depend on the gain setting and data rate */
        //! TODO: implement conversion based on gain and data rate settings
    }

    return statusRet;
}


/*******************************************************************************
 * ISR AND CALLBACK FUNCTIONS
 *******************************************************************************/

void ADS1115::staticWrapper(void *context, void *arg)
{
    //! TODO: pass notification but as argument to callback
    ADS1115 *instance = static_cast<ADS1115 *>(context);
    /* call teh alert pin ISR and pass any arg needed   */
    instance->alertPinISR(arg);
}

void ADS1115::alertPinISR(void *arg)
{
    /* perform any actions we need to take if alert pin is triggered*/

    //! TODO: do we need to clear pin or anything like that?

    /* call any callbacks that might be registered for the specific channel or fault triggered */
    //! TODO: determine if arg contributes any valuable information for the callback
    // for now assume it came from a conversion complete but if we are using alert pin for
    //  faults we need to check that configuration. In any case the most recent channel
    // should have modified the callback function so we are going to call that if
    //  it exists.
    /* if callback exists, call that here */
    if (callback != nullptr && callbackContext != nullptr)
    {
        /* call the callback and pass the context and an argument */
        callback(callbackContext, arg);
    }
    else
    {
        /* perform something if we have nullptr */
        //! TODO: add logging to this module
        // ESP_LOGE("Gpio", "Pointer to Gpio::callback is nullptr\n");
    }
}