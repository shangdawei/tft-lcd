/******************************************************************************
* File Name          : main.c
* Author             : xyl 
* FW Version         : V0.1
* Date               : 05/31/2009
* Description        : Main program body.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "gptip.h"
#include "uart.h"
#include "datadeal.h"
   
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	pGPTIP pGptip = &Gptip;
//	Init_Timer();
	SetupUSART();
//	Init_fpga();
//	Init_nandflash();
 

	while(1)
	{
		/*从UART的缓冲区里读取一帧命令处理*/
		if(ReadFromComBuf(pGptip) == 1)
		{
		   if(CRC_Check(pGptip) == 1)
		   {
		   		DealCmd(pGptip);
		   }
		}
	}
}



#if 0
int main(void)
{
#ifdef DEBUG
  debug();
#endif

  /* System Clocks Configuration **********************************************/
  RCC_Configuration();   


  /* Configure IO connected to LD1, LD2, LD3 and LD4 leds *********************/
  /* Enable GPIO_LED clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_LED, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_LED, &GPIO_InitStructure);

  while (1)
  {
    /* Turn off LD4 */
    GPIO_ResetBits(GPIO_LED, GPIO_Pin_9);
    /* Turn on LD1 */
    GPIO_SetBits(GPIO_LED, GPIO_Pin_6);
    /* Insert delay */
    Delay(0xAFFFF);

    /* Turn on LD2  */
    GPIO_SetBits(GPIO_LED, GPIO_Pin_7);
    /* Turn off LD1 */
    GPIO_ResetBits(GPIO_LED, GPIO_Pin_6);
    /* Insert delay */
    Delay(0xAFFFF);
      /* Turn on  LD3 */
    GPIO_SetBits(GPIO_LED, GPIO_Pin_8);
    /* Turn off LD2 */
    GPIO_ResetBits(GPIO_LED, GPIO_Pin_7);
    /* Insert delay */
    Delay(0xAFFFF);
          /* Turn on LD4 */
    GPIO_SetBits(GPIO_LED, GPIO_Pin_9);
    /* Turn off LD3 */
    GPIO_ResetBits(GPIO_LED, GPIO_Pin_8);
    /* Insert delay */
    Delay(0xAFFFF);

  }
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 	
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
}



/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
