/**
 * @file port_buzzer.h
 * @author Jose Manuel Bravo
 * @brief Header of port_buzzer.c
 * @version 0.1
 * @date 2023-05-17
 * 
 */


/* Includes */
#include <stdint.h>
#include "port_system.h"

/* Defines */
#define BUZZER_0_ID 0   /*!< Identification number for BUZZER system */
#define BUZZER_0_PORT GPIOB /*!< Port to which the buzzer 0 is connected*/
#define BUZZER_0_PIN 1  /*!< Pin to which the buzzer 0 is connected*/

/**
 * @brief Function to init the HW of the buzzer
 * 
 * @param buzzer_id Identifier of the buzzer that will play the sound
 */
void port_buzzer_init(uint8_t buzzer_id);

/**
 * @brief Plays a sound with the active buzzer connected
 * 
 * @param buzzer_id Identifier of the buzzer that will play the sound
 */
void port_buzzer_play(uint8_t buzzer_id);