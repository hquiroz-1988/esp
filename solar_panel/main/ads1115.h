/**
 ********************************************************************************
 * @file    ads1115.h
 * @author  Hugo Quiroz
 * @date    2024-10-08 11:51:00
 * @brief   description
 ********************************************************************************
 */

#ifndef ADS1115_H
#define ADS1115_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/


/************************************
 * MACROS AND DEFINES
 ************************************/
#define ADS1115_POINTER_REGISTER_SIZE                   (1u)
#define ADS1115_CONVERSION_REGISTER_SIZE                (2u)
#define ADS1115_CONFIG_REGISTER_SIZE                    (2u)
#define ADS1115_LO_THRESH_REGISTER_SIZE                 (2u)
#define ADS1115_HI_THRESH_REGISTER_SIZE                 (2u)

/************************************
 * TYPEDEFS
 ************************************/
typedef union
{
    struct
    {
        uint8_t opStatus        : 1;
        uint8_t mux             : 3;
        uint8_t pga             : 3;
        uint8_t mode            : 1;
        uint8_t dataRate        : 3;   
        uint8_t compMode        : 1;
        uint8_t compPolarity    : 1;
        uint8_t compLatch       : 1;
        uint8_t compQueue       : 2;
    };
    uint8_t data[ADS1115_CONFIG_REGISTER_SIZE];
}ads1115ConfigRegister_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void init_ads1115(void);
errType_t get_ads1115Configuration(ads1115ConfigRegister_t * configPtr);
errType_t set_ads1115Configuration(ads1115ConfigRegister_t * configPtr);


#ifdef __cplusplus
}
#endif

#endif //ADS1115_H