/**
 * @file port_tx.c
 * @brief Portable functions to interact with the infrared transmitter FSM library.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 09/02/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "port_tx.h"
#include "fsm_tx.h"

/* Defines --------------------------------------------------------------------*/
#define ALT_FUNC1_TIM2 0x01U /*!< TIM2 Alternate Function mapping */
#define TIM_AS_PWM1_MASK 0x0060

/* IMPORTANT
The timer symbol is the same for all the TX, so it is not in the structure of TX. It has been decided to be the TIM1. It is like a systick but faster.
*/

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Struct to define the HW dependencies of a TX
*/
typedef struct{
  GPIO_TypeDef *p_port; /*!< GPIO where the transmitter is connected*/
  uint8_t pin;   /*!< Pin/line where the infrared transmitter is connected*/
  uint8_t alt_func; /*!< Alternate function value according to the Alternate function table of the datasheet*/
}port_tx_hw_t;
/* Global variables ------------------------------------------------------------*/
static volatile uint32_t symbol_tick;

static port_tx_hw_t transmitters_arr [] ={
  [IR_TX_0_ID] = {.p_port = IR_TX_0_GPIO, .pin=IR_TX_0_PIN, .alt_func = ALT_FUNC1_TIM2},
};

/* Infrared transmitter private functions */
/**
 * @brief Setup the symbol clock (0.56 ms)
 * 
 */
static void _timer_symbol_setup()
{
  /* Enable clock source */
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

  /* Disables counter register and enables preload register*/
  TIM1->CR1 &= ~TIM_CR1_CEN;
  TIM1->CR1 |= TIM_CR1_ARPE;

  /* Reset TIM1 counter*/
  TIM1->CNT = 0x0;

  /*Set the prescaler*/
  uint64_t psc = (SystemCoreClock/1000000*NEC_TX_TIMER_TICK_BASE_US)/((65535+1)) - 1;
  TIM1->PSC = psc; 

  /*Set the ARR value*/
  uint64_t arr = (SystemCoreClock*NEC_TX_TIMER_TICK_BASE_US)/(1000000*(TIM1->PSC+1)) - 1;
  TIM1->ARR = arr;

  /*Generate event update*/
  TIM1->EGR = TIM_EGR_UG;

  /*Cleanup interruptions*/
  TIM1->SR = ~TIM_SR_UIF;

  /*Enable interruptions*/
  TIM1->DIER |= TIM_DIER_UIE;

  /* Finally, set the priority and enable interruptions globally. */
  NVIC_SetPriority(TIM1_UP_TIM10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0)); /* Priority 1, sub-priority 0 */
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);                                                          /* Enable interrupt */
}

/**
 * @brief Set up the pwm clock (38 kHz clock)
 * 
 * @param tx_id Transmitter ID. If we have more than one transmitter, this value allows us to know which one of the transmitters we want to configure.
 */
static void _timer_pwm_setup(uint32_t tx_id)
{
  /*Enable TIM2 clock*/
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  /*Clock source (not necessary as it is default)*/
  TIM2->CR1 = 0x0000;

  /*Disable counter register and enable autoreload preload*/
  TIM2->CR1 &= ~TIM_CR1_CEN;
  TIM2->CR1 |= TIM_CR1_ARPE;

  /*Reset the timer count*/
  TIM2->CNT = 0;

  /*Set the value of the prescaler*/
  uint32_t psc = ceil(SystemCoreClock/NEC_PWM_FREQ_HZ/65536) - 1;
  TIM2->PSC = psc;

  /*Set the value of the autoreload register*/
  uint64_t arr = (SystemCoreClock/NEC_PWM_FREQ_HZ)/(TIM2->PSC+1) - 1;
  TIM2->ARR = arr;    //Values for PWM period

  /*Generate an update event*/
  TIM2->EGR = TIM_EGR_UG;

  /*Disable output commpare of CH3*/
  TIM2->CCER &= ~TIM_CCER_CC3E;

  /*Set both mode PWM 1 and enable preload register of CH3*/
  TIM2->CCMR2 |= TIM_AS_PWM1_MASK;
  TIM2->CCMR2 |= TIM_CCMR2_OC3PE;

  /*Set the pulse width of CH3*/
  uint32_t ccr3 = NEC_PWM_DC * (TIM2->ARR+1);
  TIM2->CCR3 = ccr3;  
}

/* Public functions */
void port_tx_init(uint8_t tx_id, bool status)
{
  GPIO_TypeDef *port = transmitters_arr[tx_id].p_port;
  uint8_t pin = transmitters_arr[tx_id].pin;
  uint8_t alternate = transmitters_arr[tx_id].alt_func;

  port_system_gpio_config(port, pin, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PDOWN);
  port_system_gpio_config_alternate(port, pin, alternate);
  _timer_symbol_setup();
  _timer_pwm_setup(tx_id);
  port_tx_pwm_timer_set(tx_id, status);
}

/* In order to make the academic effort of configuring the PWM, the values: timer, channel and masks are hardcoded and this function is not generic for any timer/channel. It is not the best way, but it is as it. */
void port_tx_pwm_timer_set(uint8_t tx_id, bool status)
{
  if(status)
  {
    TIM2->CCER |= TIM_CCER_CC3E;
    TIM2->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    TIM2->CCER &= ~TIM_CCER_CC3E;
    TIM2->CR1 &= ~TIM_CR1_CEN;
  }
}

void port_tx_symbol_tmr_start()
{
  TIM1->CNT = 0;
  TIM1->CR1 |= TIM_CR1_CEN;
  symbol_tick = 0;
}

void port_tx_symbol_tmr_stop()
{
  TIM1->CR1 &= ~TIM_CR1_CEN;
}

uint32_t port_tx_tmr_get_tick()
{
  return symbol_tick;
}

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------

void TIM1_UP_TIM10_IRQHandler(void)
{
  /*Check if the interruption was caused by TIM1*/
  if(TIM1->SR & 0x1){ 
    TIM1->SR &= ~TIM_SR_UIF;
    symbol_tick++;
  }
}