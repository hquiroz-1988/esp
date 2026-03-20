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
    uint8_t pointerReg[ADS1115_POINTER_REGISTER_SIZE];    /**< pointer register is write only     */
    ads1115ConversionRegister_t conversionReg;            /**< conversion register is read only   */
    ads1115ConfigRegister_t configReg;                    /**< config register is read and write  */
    uint8_t loThreshReg[ADS1115_LO_THRESH_REGISTER_SIZE]; /**< loThresh register is read and write   */
    uint8_t hiThreshReg[ADS1115_HI_THRESH_REGISTER_SIZE]; /**< hiThresh is read and write   */
} ads1115_RegisterMap_t;

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

Status_t ADS1115::writeConfigRegister(ADS1115_Config_t &configObj)
{
    Status_t statusRet;

    I2CTransfer_t transferObj;
    transferObj.devAddr = ADS1115_ADDRESS;
    transferObj.regAddr = ADS1115_CONFIG_REGISTER;
    transferObj.size = ADS1115_CONFIG_REGISTER_SIZE;
    transferObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    transferObj.ackType = I2CTransferAckType_t::MASTER_ACK;
    transferObj.data = new uint8_t[ADS1115_CONFIG_REGISTER_SIZE];

    statusRet = configObjToBytes(configObj, transferObj.data);

    if (statusRet == STATUS_OKAY)
    {
        statusRet = write(transferObj);
    }

    return statusRet;
}

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

Status_t ADS1115::readConversionRegister(float &value)
{
    Status_t statusRet = STATUS_OKAY;

    I2CTransfer_t transferObj;
    transferObj.devAddr = ADS1115_ADDRESS;
    transferObj.regAddr = ADS1115_CONVERSION_REGISTER;
    transferObj.size = ADS1115_CONVERSION_REGISTER_SIZE;
    transferObj.ackEn = ADS1115_ACK_CHECK_STATUS;
    transferObj.ackType = I2CTransferAckType_t::MASTER_ACK;

    statusRet = read(transferObj);

    if (statusRet == STATUS_OKAY)
    {
        /* convert bytes to float value, this will depend on the gain setting and data rate */
        //! TODO: implement conversion based on gain and data rate settings
    }

    return statusRet;
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
    /* constructor implementation*/
    //! TODO: set callback for gpio alert pin, either here or initialize function
    alertPin.setCallback(staticWrapper, this);
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
    retVal = writeConfigRegister(const_cast<ADS1115_Config_t &>(configObj));

    return retVal;
}

Status_t ADS1115::startSingleConversion(ADS1115_Config_t &configObj)
{
    Status_t retVal = STATUS_OKAY;

    /* write to address pointer register */
    retVal = setAddressPointerRegister(ADS1115_PointerRegister::Config);


    if (retVal == STATUS_OKAY)
    {
        /* Update operation status to initiate single conversion */
        configObj.opStatus = ADS1115_OperationalStatus_t::Write_StartSingleConversion;

        /* then write to configuration register */
        retVal = writeConfigRegister(configObj);
    }
    

    return retVal;
}

Status_t ADS1115::getLatestConversion(float &value)
{
    Status_t retVal = STATUS_OKAY;

    /* write to pointer register */
    retVal = setAddressPointerRegister(ADS1115_PointerRegister::Conversion);

    if (retVal == STATUS_OKAY)
    {
        /* read conversion register */
        retVal = readConversionRegister(value);
    }

    return retVal;
}

Status_t ADS1115::getAlertPinStatus(bool &pinState)
{
    Status_t retVal = STATUS_OKAY;

    /* read gpio status of pin */
    // pinState = alertPin.get();

    return retVal;
}

Status_t ADS1115::setConfiguration(ads1115ConfigRegister_t *configPtr)
{
    Status_t statusRet = STATUS_OKAY;

    return statusRet;
}

Status_t ADS1115::getLatestReading(ads1115ConversionRegister_t *regPtr)
{
    /* write conversion addy to the pointer register    */
    Status_t statusRet = STATUS_OKAY;

    return statusRet;
}

Status_t ADS1115::waitForConversionComplete(void)
{
    Status_t retVal = STATUS_OKAY;

    return retVal;
}

Status_t ADS1115::setLowThreshold(int16_t threshold)
{
    Status_t retVal = STATUS_OKAY;

    //! TODO: implement

    return retVal;
}

Status_t ADS1115::setHighThreshold(int16_t threshold)
{
    Status_t retVal = STATUS_OKAY;


    //!TODO: implement

    return retVal;
}

Status_t ADS1115::setAddressPointerRegister(ADS1115_PointerRegister reg)
{
    Status_t retVal = STATUS_OKAY;

    //!TODO: implement

    return retVal;
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