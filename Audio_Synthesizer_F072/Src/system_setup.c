/**
 * system_setup.c
 * 
 * This file contains the implementations for functions that set up the STM32's peripherals.
 * 
 * Authors:  Kenneth Gordon, Bryant Watson, Hayoung Im, and Adrian Sucahyo
 * Date:  March 25, 2025
 */

#include "main.h"
#include <stm32f0xx_hal.h>

/**
 * @brief Enable the GPIOA Clock using the RCC.
 * @param None
 * @retval None
 */
void HAL_RCC_GPIOA_CLK_Enable(void)
{
  RCC->AHBENR &= ~(RCC_AHBENR_GPIOAEN);
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

/**
 * @brief Enable the GPIOB Clock using the RCC.
 * @param None
 * @retval None
 */
void HAL_RCC_GPIOB_CLK_Enable(void)
{
  RCC->AHBENR &= ~(RCC_AHBENR_GPIOBEN);
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
}

/**
 * @brief Enable the GPIOC Clock using the RCC.
 * @param None
 * @retval None
 */
void HAL_RCC_GPIOC_CLK_Enable(void)
{
  RCC->AHBENR &= ~(RCC_AHBENR_GPIOCEN);
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

/**
 * @brief Enable the RCC Clock for TIM2
 */
void HAL_RCC_TIM2_CLK_Enable()
{
  RCC->APB1ENR &= ~(RCC_APB1ENR_TIM2EN);
  RCC->APB1ENR |=  (RCC_APB1ENR_TIM2EN);
}

/**
 * @brief Enable the RCC Clock for TIM3
 */
void HAL_RCC_TIM3_CLK_Enable()
{
  RCC->APB1ENR &= ~(RCC_APB1ENR_TIM3EN);
  RCC->APB1ENR |=  (RCC_APB1ENR_TIM3EN);
}

/**
 * @brief Enable the RCC clock for the USART1 peripheral.
 * @param None
 * @retval None
 */
void HAL_RCC_USART1_CLK_Enable(void)
{
  RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN);
  RCC->APB2ENR &=   RCC_APB2ENR_USART1EN;
}

/**
 * @brief Enable the RCC clock for the USART2 peripheral.
 * @param None
 * @retval None
 */
void HAL_RCC_USART2_CLK_Enable(void)
{
  RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN);
  RCC->APB1ENR |=   RCC_APB1ENR_USART2EN;
}

/**
 * @brief Enable the RCC clock for the USART3 peripheral.
 * @param None
 * @retval None
 */
void HAL_RCC_USART3_CLK_Enable(void)
{
  RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN);
  RCC->APB1ENR |=   RCC_APB1ENR_USART3EN;
}

/**
 * @brief Enable the RCC clock for the USART4 peripheral.
 * @param None
 * @retval None
 */
void HAL_RCC_USART4_CLK_Enable(void)
{
  RCC->APB1ENR &= ~(RCC_APB1ENR_USART4EN);
  RCC->APB1ENR |=   RCC_APB1ENR_USART4EN;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
    /* STM32F0xx HAL library initialization:
    - Configure the Flash prefetch
    - Systick timer is configured by default as source of time base, but user
        can eventually implement his proper time base source (a general purpose
        timer for example or other time source), keeping in mind that Time base
        duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
        handled in milliseconds basis.
    - Low Level Initialization
    */
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Select HSI48 Oscillator as PLL source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler();
  }
}

#ifdef  USE_FULL_ASSERT
  /**
    * @brief  Reports the name of the source file and the source line number
    *         where the assert_param error has occurred.
    * @param  file: pointer to the source file name
    * @param  line: assert_param error line source number
    * @retval None
    */
  void assert_failed(uint8_t* file, uint32_t line)
  {
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
  }
#endif

/* These implement libc system calls that a referenced but not used, which causes a linker error. */
void _close(void) { Error_Handler(); }
void _lseek(void) { Error_Handler(); }
void _read(void) { Error_Handler(); }
void _write(void) { Error_Handler(); }