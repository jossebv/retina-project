/**
 * @file fsm_tx.h
 * @brief Header for fsm_tx.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 08/02/2023
 */

#ifndef FSM_TX_H_
#define FSM_TX_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/* NEC transmission macros */
#define NEC_TX_TIMER_TICK_BASE_US 56.25 /*!< Time base in microseconds to create the ticks for the timer of symbols */
#define NEC_TX_PROLOGUE_TICKS_ON 9*1000/NEC_TX_TIMER_TICK_BASE_US       /*!< Number of time base ticks for prologue ON in transmission  */
#define NEC_TX_PROLOGUE_TICKS_OFF 4.5*1000/NEC_TX_TIMER_TICK_BASE_US      /*!< Number of time base ticks for prologue OFF in transmission  */
#define NEC_TX_SYM_0_TICKS_ON 562.5/NEC_TX_TIMER_TICK_BASE_US          /*!< Number of time base ticks for symbol 0 ON in transmission  */
#define NEC_TX_SYM_0_TICKS_OFF 562.5/NEC_TX_TIMER_TICK_BASE_US         /*!< Number of time base ticks for symbol 0 OFF in transmission  */
#define NEC_TX_SYM_1_TICKS_ON 562.5/NEC_TX_TIMER_TICK_BASE_US          /*!< Number of time base ticks for symbol 1 ON in transmission  */
#define NEC_TX_SYM_1_TICKS_OFF 1.6875*1000/NEC_TX_TIMER_TICK_BASE_US         /*!< Number of time base ticks for symbol 1 OFF in transmission  */
#define NEC_TX_EPILOGUE_TICKS_ON 562.5/NEC_TX_TIMER_TICK_BASE_US       /*!< Number of time base ticks for epilogue ON in transmission  */
#define NEC_TX_EPILOGUE_TICKS_OFF 3560  /*!< Number of time base ticks for epilogue OFF in transmission ~200 miliseconds */
#define NEC_PWM_FREQ_HZ 38000                /*!< PWM timer frequency in Hz */
#define NEC_PWM_DC 0.35                     /*!< PWM duty cycle 0-1  */

/* Function prototypes and explanation ----------------------------------------*/
/**
 * @brief Create a new infrared transmitter FSM
 * 
 * @param Unique infrared transmitter identifier number
 * 
 * @return fsm_t* 
 */
fsm_t* fsm_tx_new (uint8_t tx_id);

/**
 * @brief Initialize an infrared transmitter FSM
 * 
 * @param p_this Pointer to an fsm_t struct that contains fsm_tx_t
 * @param tx_id Unique infrared transmitter identifier number
 */
void fsm_tx_init (fsm_t *p_this, uint8_t tx_id);

/**
 * @brief Set the given code
 * 
 * @param p_this Pointer to an fsm_t struct that contains fsm_tx_t
 * @param code Code of the NEC command to be transmitted
 */
void fsm_tx_set_code (fsm_t *p_this, uint32_t code);

/**
 * @brief Start the process to transmit the code stored
 * 
 * @param tx_id Unique infrared transmitter identifier number
 * @param code Code of the NEC command to be transmitted
 */
void fsm_send_NEC_code (uint8_t tx_id, uint32_t code);

#endif
