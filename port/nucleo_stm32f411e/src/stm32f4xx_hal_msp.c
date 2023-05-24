/**
 * @file stm32f4xx_hal_msp.c
 * @brief MspInit function.
 * 
 */

#include "stm32f411e_discovery.h"

/**
 * @brief Low level HAL initialization
 * 
 */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}
