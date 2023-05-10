/**
 * @file port_system.c
 * @brief File that defines the functions that are related to the access to the specific HW of the microcontroller.
 * @author Jose Manuel Bravo Pacheco
 * @date 04/05/2023
 */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#include "port_system.h"
#include "stm32f411e_discovery.h"
#include "picovoice.h"

/* GLOBAL VARIABLES */
static volatile uint32_t msTicks = 0;     /*!< Variable to store millisecond ticks */
static volatile uint32_t microsTicks = 0; /*!< Variable to store microseconds ticks */

static uint8_t uuid[UUID_SIZE];

/* These variables are declared extern in CMSIS (system_stm32f4xx.h) */
uint32_t SystemCoreClock = HSI_VALUE;                                               /*!< Frequency of the System clock */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9}; /*!< Prescaler values for AHB bus */
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};                          /*!< Prescaler values for APB bus */

/* SYSTEM CONFIGURATION */
/**
 * @brief  Setup the microcontroller system
 *         Initialize the FPU setting, vector table location and External memory
 *         configuration.
 *
 * @note   This function is called at startup by CMSIS in startup_stm32f446xx.s.
 */
void SystemInit(void)
{
/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

#if defined(DATA_IN_ExtSRAM) || defined(DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl();
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

  /* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif                                                 /* USER_VECT_TAB_ADDRESS */
}

/**
 * @brief Configurates the system clock
 */
static void system_clock_config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // 100  MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // 100 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         // 50 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // 100 MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);

  // /* Update the SystemCoreClock global variable */
  // SystemCoreClock = SYSCLOCK_FREQ >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];

  // /* Configure the source of time base considering new system clocks settings */
  // SysTick_Config(SystemCoreClock / (1000U / TICK_FREQ_1KHZ)); /* Set Systick to 1 ms */
}

pv_status_t port_system_init()
{
  if (HAL_Init() != HAL_OK)
  {
    return PV_STATUS_INVALID_STATE;
  }
  system_clock_config();

  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  memcpy(uuid, (uint8_t *)UUID_ADDRESS, UUID_SIZE);

  // /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  // /* Configure Flash prefetch, Instruction cache, Data cache */
  // /* Instruction cache enable */
  // FLASH->ACR |= FLASH_ACR_ICEN;

  // /* Data cache enable */
  // FLASH->ACR |= FLASH_ACR_DCEN;

  // /* Prefetch cache enable */
  // FLASH->ACR |= FLASH_ACR_PRFTEN;

  // /* Set Interrupt Group Priority */
  // NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_4);

  /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
  /* Configure the SysTick IRQ priority. It must be the highest (lower number: 0)*/
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0U, 0U)); /* Tick interrupt priority */

  // /* Init the low level hardware */
  // /* Reset and clock control (RCC) */
  // RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; /* Syscfg clock enabling */

  // /* Peripheral clock enable register */
  // RCC->APB1ENR |= RCC_APB1ENR_PWREN; /* PWREN: Power interface clock enable */

  /* Configure the system clock */

  return PV_STATUS_SUCCESS;
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 *         The SystemCoreClock variable contains the core clock (HCLK), it can
 *         be used by the user application to setup the SysTick timer or configure
 *         other parameters.
 *
 * @note   Each time the core clock (HCLK) changes, this function must be called
 *         to update SystemCoreClock variable value. Otherwise, any configuration
 *         based on this variable will be incorrect.
 *
 * @note   - The system frequency computed by this function is not the real
 *           frequency in the chip. It is calculated based on the predefined
 *           constant and the selected clock source:
 *
 *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
 *
 *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
 *
 *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
 *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
 *
 *         (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
 *             16 MHz) but the real value may vary depending on the variations
 *             in voltage and temperature.
 *
 *         (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (its value
 *              depends on the application requirements), user has to ensure that HSE_VALUE
 *              is same as the real frequency of the crystal used. Otherwise, this function
 *              may have wrong result.
 *
 *         - The result of this function could be not correct when using fractional
 *           value for HSE crystal.
 *
 * @param  None
 * @retval None
 */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
  case 0x00: /* HSI used as system clock source */
    SystemCoreClock = HSI_VALUE;
    break;
  case 0x04: /* HSE used as system clock source */
    SystemCoreClock = HSE_VALUE;
    break;
  case 0x08: /* PLL used as system clock source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
       SYSCLK = PLL_VCO / PLL_P
       */
    pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
    pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

    if (pllsource != 0)
    {
      /* HSE used as PLL clock source */
      pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
    }
    else
    {
      /* HSI used as PLL clock source */
      pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
    }

    pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1) * 2;
    SystemCoreClock = pllvco / pllp;
    break;
  default:
    SystemCoreClock = HSI_VALUE;
    break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
}

void port_system_power_stop()
{
  MODIFY_REG(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS), PWR_CR_LPDS); // Select the regulator state in Stop mode: Set PDDS and LPDS bits according to PWR_Regulator value
  SCB->SCR |= ((uint32_t)SCB_SCR_SLEEPDEEP_Msk);                 // Set SLEEPDEEP bit of Cortex System Control Register
  __WFI();                                                       // Select Stop mode entry : Request Wait For Interrupt
  SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);                // Reset SLEEPDEEP bit of Cortex System Control Register
}

void port_system_sleep()
{
  port_system_systick_suspend();
  port_system_power_stop();
}

//------------------------------------------------------
// TIMER RELATED FUNCTIONS
//------------------------------------------------------
void port_system_delay_ms(uint32_t ms)
{
  uint32_t tickstart = port_system_get_millis();

  while ((port_system_get_millis() - tickstart) < ms)
  {
  }
}

void port_system_delay_until_ms(uint32_t *p_t, uint32_t ms)
{
  uint32_t until = *p_t + ms;
  uint32_t now = port_system_get_millis();
  if (until > now)
  {
    port_system_delay_ms(until - now);
  }
  *p_t = port_system_get_millis();
}

void port_system_timer5_init(uint16_t arr, uint16_t psc)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; /* TIM5 con APB1*/

  /* Temporización*/
  TIM5->CR1 = 0x0000;
  TIM5->CR1 &= ~TIM_CR1_CEN;

  TIM5->CNT = 0;
  TIM5->ARR = arr;
  TIM5->PSC = psc;

  TIM5->EGR = TIM_EGR_UG;
  TIM5->SR = TIM_SR_UIF;
  TIM5->DIER |= TIM_DIER_UIE;

  NVIC_SetPriority(TIM5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(TIM5_IRQn);

  TIM5->CR1 |= TIM_CR1_CEN;
}

void port_system_delay_micros(uint32_t micros)
{
  uint32_t tickstart = port_system_get_micros();

  while ((port_system_get_micros() - tickstart) < micros)
  {
  }
}

void port_system_systick_suspend()
{
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void port_system_systick_resume()
{
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

//------------------------------------------------------
// GPIO RELATED FUNCTIONS
//------------------------------------------------------
/**
 * @brief Configure the mode and pull of a GPIO
 *
 * @note This function performs the GPIO Port Clock Enable. It may occur that a port clock is re-enabled,
 *       it does not matter if it was already enabled. *
 * @note This function enables the AHB1 peripheral clock. After reset, the peripheral clock (used for registers
 *       read/write access) is disabled and the application software has to enable this clock before using it.
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param mode Input, output, alternate, or analog
 * @param pupd Pull-up, pull-down, or no-pull
 *
 * @retval None
 */

void port_system_gpio_config(GPIO_TypeDef *port, uint8_t pin, uint8_t mode, uint8_t pupd)
{
  /*Enables the clock of the GPIOX*/
  if (port == GPIOA)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  }
  else if (port == GPIOB)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  }
  else if (port == GPIOC)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  }

  /*Sets the PIN MODE*/
  port->MODER &= ~BASE_MASK_TO_POS(BASE_MODER_MASK, 2 * pin);
  port->MODER |= BASE_MASK_TO_POS(mode, 2 * pin);

  /*Sets the pull-up/pull-down resistance*/
  port->PUPDR &= ~BASE_MASK_TO_POS(BASE_PUPDR_MASK, 2 * pin);
  port->PUPDR |= BASE_MASK_TO_POS(pupd, 2 * pin);
}

/**
 * @brief Configure the alternate function of a GPIO
 *
 * @note The AFR register is a 2-element array representing GPIO alternate function high an low registers (GPIOx_AFRH and GPIOx_AFRL) \n
 * AFRLy: Alternate function selection for port x pin y (y = 0..7) \n
 * AFRHy: Alternate function selection for port x pin y (y = 8..15)
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param alternate Alternate function number (values from 0 to 15) according to table of the datasheet: "Table 11. Alternate function".
 *
 * @retval None
 */
void port_system_gpio_config_alternate(GPIO_TypeDef *port, uint8_t pin, uint8_t alternate)
{
  port->AFR[pin / 8] &= ~BASE_MASK_TO_POS(BASE_ALTERNATE_MASK, 4 * (pin % 8));
  port->AFR[pin / 8] |= BASE_MASK_TO_POS(alternate, 4 * (pin % 8));
}

/**
 * @brief Configure the external interruption or event of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param mode Trigger mode can be a combination (OR) of: (i) direction: rising edge (0x01), falling edge (0x02), (ii)  event request (0x04), or (iii) interrupt request (0x08).
 * @retval None
 */
void port_system_gpio_config_exti(GPIO_TypeDef *port, uint8_t pin, uint32_t mode)
{
  /*Enables system controller*/
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  /*Associate the exti with the GPIO*/
  uint8_t mascara;
  SYSCFG->EXTICR[pin / 4] &= ~BASE_MASK_TO_POS(BASE_EXTIX_MASK, 4 * (pin % 4));
  if (port == GPIOA)
  {
    mascara = 0x00;
  }
  else if (port == GPIOB)
  {
    mascara = 0x01;
  }
  else if (port == GPIOC)
  {
    mascara = 0x02;
  }
  SYSCFG->EXTICR[pin / 4] |= BASE_MASK_TO_POS(mascara, 4 * (pin % 4));

  /*Interruption trigger*/
  if ((mode & BASE_MASK_TO_POS(0x3, 0)) == TRIGGER_RISING_EDGE)
  {
    EXTI->RTSR |= BIT_POS_TO_MASK(pin); /*Rising edge*/
  }
  else if ((mode & BASE_MASK_TO_POS(0x3, 0)) == TRIGGER_FALLING_EDGE)
  {
    EXTI->FTSR |= BIT_POS_TO_MASK(pin); /*Falling edge*/
  }
  else if ((mode & BASE_MASK_TO_POS(0x3, 0)) == TRIGGER_BOTH_EDGE)
  {
    EXTI->RTSR |= BIT_POS_TO_MASK(pin); /*Rising edge*/
    EXTI->FTSR |= BIT_POS_TO_MASK(pin); /*Falling edge*/
  }

  /*Enable local interrupt and event*/
  if ((mode & BASE_MASK_TO_POS(0x3, 2)) == TRIGGER_ENABLE_INTERR_REQ)
  {
    EXTI->IMR |= BIT_POS_TO_MASK(pin);
  }
  else if ((mode & BASE_MASK_TO_POS(0x3, 2)) == TRIGGER_ENABLE_EVENT_REQ)
  {
    EXTI->EMR |= BIT_POS_TO_MASK(pin);
  }
  else if ((mode & BASE_MASK_TO_POS(0x3, 2)) == TRIGGER_ENABLE_EVENT_AND_INTERR_REQ)
  {
    EXTI->EMR |= BIT_POS_TO_MASK(pin);
    EXTI->IMR |= BIT_POS_TO_MASK(pin);
  }
}

/**
 * @brief Enable interrupts of a GPIO line (pin)
 *
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param priority Priority level (from highest priority: 0, to lowest priority: 15)
 * @param subpriority Subpriority level (from highest priority: 0, to lowest priority: 15)
 *
 * @retval None
 */
void port_system_gpio_exti_enable(uint8_t pin, uint8_t priority, uint8_t subpriority)
{
  NVIC_SetPriority(GET_PIN_IRQN(pin), NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, subpriority));
  NVIC_EnableIRQ(GET_PIN_IRQN(pin));
}

/**
 * @brief Disable interrupts of a GPIO line (pin)
 *
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @retval None
 */
void port_system_gpio_exti_disable(uint8_t pin)
{
  NVIC_DisableIRQ(GET_PIN_IRQN(pin));
}

/**
 * @brief Read the digital value of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @return `true` if the GPIO was HIGH
 * @return `false` if the GPIO was LOW
 */
bool port_system_gpio_read(GPIO_TypeDef *port, uint8_t pin)
{
  return (bool)(port->IDR & BIT_POS_TO_MASK(pin));
}

/**
 * @brief Write a digital value in a GPIO atomically
 *
 * @note You can use a +16 offset on the pin index and use the `BIT_POS_TO_MASK(pin)` macro to get the mask when you go to clear a GPIO. Otherwise, you can calculate the pin mask first and then use a 16-position left shift of the mask.
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param value Boolean value to set the GPIO to HIGH (1, `true`) or LOW (0, `false`)
 *
 * @retval None
 */
void port_system_gpio_write(GPIO_TypeDef *port, uint8_t pin, bool value)
{
  if (value)
  {
    port->BSRR = BIT_POS_TO_MASK(pin);
  }
  else
  {
    port->BSRR = BIT_POS_TO_MASK(pin) << 16;
  }
}

/**
 * @brief Toggle the value of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @retval None
 */
void port_system_gpio_toggle(GPIO_TypeDef *port, uint8_t pin)
{
  if (port_system_gpio_read(port, pin))
  {
    port_system_gpio_write(port, pin, 0);
  }
  else
  {
    port_system_gpio_write(port, pin, 1);
  }
}

void port_system_gpio_clear(GPIO_TypeDef *port)
{
  for (uint8_t i = 0; i < 16; i++)
  {
    port_system_gpio_write(port, i, 0);
  }
}

///////////////////////////////////////////////////////
/* OTHERS */
///////////////////////////////////////////////////////

const uint8_t *pv_get_uuid(void) {
    return (const uint8_t *) uuid;
}

uint32_t pv_get_uuid_size(void) {
    return UUID_SIZE;
}

///////////////////////////////////////////////////////
/* INTERRUPTION HANDLER */
///////////////////////////////////////////////////////
/**
 * @brief This function handles the System tick timer that increments the system millisecond counter (global variable).
 *
 */
void SysTick_Handler(void)
{
  msTicks++;
}

uint32_t port_system_get_millis(void)
{
  return msTicks;
}

void TIM5_IRQHandler()
{
  microsTicks++;
  TIM5->SR &= ~TIM_SR_UIF;
}

uint32_t port_system_get_micros(void)
{
  return microsTicks;
}