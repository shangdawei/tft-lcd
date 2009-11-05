/******************** U-easytech ********************
* File Name          : platform_config.h
* Author             : u-easytech
* Version            : V2.0
* Date               : 05/26/2008
* Description        : Evaluation board specific configuration file.
********************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (PRO_STM32) &&  !defined (STM32_EK)
 //#define USE_STM3210B_EVAL
 #define STM32_EK
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef    PRO_STM32
  #define GPIO_LED                   GPIOC    
  #define RCC_APB2Periph_GPIO_LED    RCC_APB2Periph_GPIOC
  
#elif defined STM32_EK
  #define GPIO_LED                   GPIOF    
  #define RCC_APB2Periph_GPIO_LED    RCC_APB2Periph_GPIOF
#endif /* USE_STM3210B_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
