/**
 *******************************************************************************
 * @file    interrupt_base.hpp
 * @author  HQ
 * @date    2025-08-22 12:08:27
 * @brief   
 *******************************************************************************
 */

#ifndef INTERRUPT_BASE_HPP
#define INTERRUPT_BASE_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "typedefs.h"
#include <list>

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class InterruptBase {
public:
    // pure virtual Callbacks
    virtual void HAL_TIM_PeriodElapsedCallback(void * arg){};
    virtual void HAL_GPIO_EXTI_Callback(void * arg){};

protected:
    enum class IntType {
        HAL_TIM_IC_CaptureCallback,
        HAL_TIM_PeriodElapsedCallback,
        HAL_GPIO_EXTI_Callback,
        HAL_ADC_ConvCpltCallback,
        HAL_TIM_PWM_PulseFinishedCallback,
        HAL_TIM_OC_DelayElapsedCallback,
        HAL_UART_RxHalfCpltCallback,
        HAL_UART_RxCpltCallback,
        HAL_UART_TxCpltCallback,
        HAL_SPI_TxHalfCpltCallback,
        HAL_SPI_TxCpltCallback,
        HAL_UART_ErrorCallback
    };
    bool registerCallback(IntType type);
    bool removeCallback(IntType type);
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // INTERRUPT_BASE_HPP