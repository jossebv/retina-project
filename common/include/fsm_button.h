/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 23/02/2023
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Create a new button FSM. 
 * 
 * @attention When used, the button duration must be reset by using the fsm_button_reset_duration() function.
 * 
 * @param debounce_time Anti-debounce time in milliseconds
 * @param button_id Unique button identifier number
 * @return fsm_t* 
 */
fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id);

/**
 * @brief Initialize a button FSM. 
 *
 * @note The FSM acts as intermediary with the button_id, it does not uses it at all 
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 * @param debounce_time Anti-debounce time in milliseconds
 * @param button_id Unique button identifier number
 */
void fsm_button_init (fsm_t *p_this, uint32_t debounce_time, uint32_t button_id);

/**
 * @brief Return the duration of the last button press
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_button_t
 * @return uint32_t 
 */
uint32_t fsm_button_get_duration (fsm_t *p_this);

/**
 * @brief Reset the duration of the last button press.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 */
void fsm_button_reset_duration (fsm_t *p_this);

/**
 * @brief Check if the button FSM is active or not
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t structure
 * 
 * @return true
 * @return false
*/
bool fsm_button_check_activity(fsm_t *p_this);

#endif
