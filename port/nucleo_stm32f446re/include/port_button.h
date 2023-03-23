/**
 * @file port_button.h
 * @brief Header for port_button.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 28/02/2023
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define BUTTONS_ROW 2

#define BUTTON_0_ID 0   /*!< Button 0 ID*/
#define BUTTON_0_GPIO GPIOC /*!< Button 0 Port to which it is connected*/
#define BUTTON_0_PIN 13 /*!< Button 0 pin to which it is connected*/
#define BUTTON_0_DEBOUNCE_TIME_MS 150 /*!< Debounce time for button 0*/

#define BUTTON_1_ID 1
#define BUTTON_1_GPIO GPIOC
#define BUTTON_1_PIN 10
#define BUTTON_1_DEBOUNCE_TIME_MS 150

#define BUTTON_2_ID 2
#define BUTTON_2_GPIO GPIOC
#define BUTTON_2_PIN 11
#define BUTTON_2_DEBOUNCE_TIME_MS 150

#define BUTTON_3_ID 3
#define BUTTON_3_GPIO GPIOA
#define BUTTON_3_PIN 1
#define BUTTON_3_DEBOUNCE_TIME_MS 150

#define BUTTON_4_ID 4
#define BUTTON_4_GPIO GPIOA
#define BUTTON_4_PIN 14
#define BUTTON_4_DEBOUNCE_TIME_MS 150


/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Configure the HW specifications of a given button
 * 
 * @param button_id Button ID. This index is used to select the element of the buttons_arr[] array
 */
void port_button_init (uint32_t button_id);

/**
 * @brief Return the status of the button (pressed or not)
 * 
 * @param button_id Button ID. The index is used to select the element of the buttons_arr[] array
 * 
 * @return true If the button has been pressed
 * @return false If the button has not been pressed
 */
bool port_button_is_pressed (uint32_t button_id);

/**
 * @brief Return the count of the System tick in milliseconds
 * 
 * @return uint32_t 
 */
uint32_t port_button_get_tick();

/**
 * @brief Retrieves the direction of the pressed_flag of a button
 * 
 * This function is used when receiving an interruption caused by the button.
 * 
 * @param button_id 
 * @return true 
 * @return false 
 */
bool* port_button_get_p_flag(uint8_t button_id);

/**
 * @brief Handles the button interruption when happening on an external interruption handler
 * 
 * @param button_id 
 */
void port_button_interruption_handler(uint8_t button_id);

#endif
