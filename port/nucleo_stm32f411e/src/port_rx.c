/**
 * @file port_rx.c
 * @brief Portable functions to interact with the infrared receiver FSM library.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 27/02/2023
 * */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <string.h> /* To use memset */

/* Other includes */
#include "port_rx.h"
#include "port_button.h"
#include "port_system.h"
#include "fsm_rx_nec.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of an infrared receiver.
 */
typedef struct
{
  GPIO_TypeDef *p_port; /*!< GPIO where the infrared transmitter is connected*/
  uint8_t pin;  /*!< Pin/line where the infrared transmitter is connected*/
  uint16_t edge_ticks [NEC_FRAME_EDGES];  /*!< Array to store the time ticks of the edges detected by the infrared receiver. It size must be larger or equal than the number of expected edges of the NEC protocol*/
  uint16_t edge_idx;  /*!< Index to go though the edge_ticks array*/
} port_rx_hw_t;

/* Global variables ------------------------------------------------------------*/
/**
 * @brief Array of elements that represents the HW characteristics of the infrared receivers.
 */
static port_rx_hw_t receivers_arr[] = {
    [IR_RX_0_ID] = {.p_port = IR_RX_0_GPIO, .pin = IR_RX_0_PIN},
};

/* Infrared receiver private functions */
/**
 * @brief Set the elements of the array of time ticks to '0' and init the index to '0' as well.
 *
 * @param rx_id Receiver ID. This index is used to select the element of the `receivers_arr[]` array.
 */
static void _reset_edge_ticks_idx(uint8_t rx_id)
{
  memset(receivers_arr[rx_id].edge_ticks, 0, sizeof(uint16_t) * NEC_FRAME_EDGES);
  receivers_arr[rx_id].edge_idx = 0;
}

/**
 * @brief Store the time tick of the last edge detected. This funtion is called by the ISR aftes an interruption of the GPIO.
 * 
 * @param rx_id Receiver ID. This index is used to select the element if the receivers_arr[] array.
 */
static void _store_edge_tick(uint8_t rx_id)
{
  uint16_t edge_idx = receivers_arr[rx_id].edge_idx;

  bool value = port_system_gpio_read(receivers_arr[rx_id].p_port, receivers_arr[rx_id].pin);  //if one-> rising edge

  if(value != edge_idx%2){
    return;
  }

  if(edge_idx < NEC_FRAME_EDGES)
  {
    uint16_t tick = (uint16_t)(TIM3->CNT);
    receivers_arr[rx_id].edge_ticks[edge_idx] = tick;
    receivers_arr[rx_id].edge_idx = edge_idx + 1;
  }
}

/**
 * @brief Configure the timer tick. This timer sets the basis for tick counting for checking received NEC symbols.
 * 
 * This timer does not generate interruptions. It is like the Systick, but faster. The Systick has a period of 1 ms which is to large for the identification of NEC symbols. That is why we need another counter.
 * 
 * With this timer we are not interested very much on the value of the ARR register but on the PSC register. We will set up the ARR register to its maximum value to ensure that each increment of the prescaler corresponds to NEC_RX_TIMER_TICK_BASE_US. On the other hand, we set the PSC so that each increment of this register represents NEC_RX_TIMER_TICK_BASE_US.
 */
void _timer_rx_setup()
{
  /*Enable clock source*/
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  /*Disables counter register and enables preload register*/
  TIM3->CR1 &= ~TIM_CR1_CEN;
  TIM3->CR1 |= TIM_CR1_ARPE;

  /*Resets the timer count*/
  TIM3->CNT = 0;

  /*Set the autorreload register*/
  TIM3->ARR = ARR_MAX_VALUE;

  /*Set the prescaler register*/
  TIM3->PSC = SystemCoreClock/(2*1000000)*NEC_RX_TIMER_TICK_BASE_US - 1;

  /*Update event*/
  TIM3->EGR = TIM_EGR_UG;
}

void port_rx_init(uint8_t rx_id)
{
  GPIO_TypeDef *p_port = receivers_arr[rx_id].p_port;
  uint8_t pin = receivers_arr[rx_id].pin;

  _timer_rx_setup();

  port_system_gpio_config(p_port, pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
  port_system_gpio_config_exti(p_port, pin, 0x0b);
  port_system_gpio_exti_disable(pin);
  //port_system_gpio_exti_enable(pin, 2, 0);
  _reset_edge_ticks_idx(rx_id);
}

void port_rx_en(uint8_t rx_id, bool interr_en)
{
  uint8_t pin = receivers_arr[rx_id].pin;

  _reset_edge_ticks_idx(rx_id);
  if(interr_en)
  {
    port_system_gpio_exti_enable(pin, 2, 0);
  }
  else
  {
    port_system_gpio_exti_disable(pin);
  }
}

/**
 * @brief Enable the tick count timer
 * 
 */
void port_rx_tmr_start()
{
  TIM3->CNT = 0;
  TIM3->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief Disable the tick count timer
 * 
 */
void port_rx_tmr_stop()
{
  TIM3->CR1 &= ~TIM_CR1_CEN;
}

uint32_t port_rx_get_num_edges(uint8_t rx_id)
{
  return receivers_arr[rx_id].edge_idx;
}

uint16_t *port_rx_get_buffer_edges(uint8_t rx_id)
{
  return (uint16_t *)(&(receivers_arr[rx_id].edge_ticks));
}

void port_rx_clean_buffer(uint8_t rx_id)
{
  _reset_edge_ticks_idx(rx_id);
}

/* Interruption Rutine Handler 9-5*/
void EXTI9_5_IRQHandler(void)
{
  port_system_systick_resume();
  /*Interruption on pin PA8*/
  if (EXTI->PR & BIT_POS_TO_MASK(receivers_arr[IR_RX_0_ID].pin))
  {
    EXTI->PR |= BIT_POS_TO_MASK(receivers_arr[IR_RX_0_ID].pin);
    _store_edge_tick(IR_RX_0_ID);
  }  
}
