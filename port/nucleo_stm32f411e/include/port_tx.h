/**
 * @file port_tx.h
 * @brief Header for port_tx.c file.
 * @author Jose Manuel Bravo Pacheco  
 * @author Victor Garcia Garcia
 * @date 09/02/2023
 */

#ifndef PORT_TX_H_
#define PORT_TX_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_lcd.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define IR_TX_0_ID 0    /*!< Infrared transmitter identifier*/
#define IR_TX_0_GPIO GPIOA   /*!< Infrared transmitter GPIO port*/
#define IR_TX_0_PIN 1     /*!< Infrared transmitter GPIO pin*/

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Configure the HW specifications of a given infrared trasmitter
 * 
 * @param tx_id Transmitter ID. This index is used to select the element of the transmitters_arr[] array.
 * @param status To indeicate if PWM starts, or not, from the beginning
 */
void port_tx_init (uint8_t tx_id, bool status);

/**
 * @brief Set the PWM ON of OFF
 * 
 * @param tx_id Transmitter ID. This index is used to select the element of the transmitters_arr[] array 
 * @param status To indicate if PWM starts, or not, from the beginning 
 */
void port_tx_pwm_timer_set(uint8_t tx_id, bool status);

/**
 * @brief Starts the symbol timer and reset the count of ticks
 * 
 */
void port_tx_symbol_tmr_start();

/**
 * @brief Stop the symbol timer
 * 
 */
void port_tx_symbol_tmr_stop();

/**
 * @brief Get the count of the symbol ticks
 * 
 * @return uint32_t
 */
uint32_t port_tx_tmr_get_tick();

#endif