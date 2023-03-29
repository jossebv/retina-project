/**
 * @file fsm_rx.h
 * @brief Header for fsm_rx.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 15/02/2023
 */

#ifndef FSM_RX_H_
#define FSM_RX_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Function prototypes and explanation ----------------------------------------*/
/**
 * @brief Create a new infrared receiver FSM
 *
 * The infrared reception module indeed manages 2 FSMs. (i) The first one (`fsm_trans_rx_nec`) controls the reception of infrared pulses and stores the times where the changes on the GPIO occur. (ii) The second one (`fsm_trans_rx_nec`) parses the data received (an array of timestamps) to extract the NEC command. This second FSM that decodes the NEC protocol. Refer to `fsm_rx_NEC_new()` for further information about this FSM.
 *
 * At start and reset, the code value must be '0x00'. A value of '0x00' means that it has not been received any new code. The Retina FSM is the one which stores and retains the last code until a new one is received.
 *
 * @attention The user is required to reset the code value once it has been read. Otherwise, this value may be misinterpreted by the Retina FSM. In such a case we would be interpreting the code constantly. The function `fsm_rx_reset_code()` resets the code when its called.
 *
 * The FSM contains information of the receiver ID. This ID is a unique identifier that is managed by the user in the `port`. That is where the user provides identifiers and HW information for all the receivers on his system. The FSM does not have to know anything of the underlying HW.
 *
 * @param rx_id Unique infrared receiver identifier number
 *
 * @return A pointer to the infrared receiver FSM
 */
fsm_t *fsm_rx_new(uint8_t rx_id);

/**
 * @brief Initialize an infrared receiver FSM.
 * 
 * This function initializes the default values of the FSM struct and calls to the port to initialize the HW of associated to the given ID.
 * 
 * @note The main program and the port are the only ones that manage and stablish the ID. The infrared receiver FSM acts as a man-in-the-middle but this library does not handles the receivers IDs.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @param rx_id Unique infrared receiver identifier number.
 */
void fsm_rx_init (fsm_t *p_this, uint8_t rx_id);

/**
 * @brief Retrieve the code parse (if any)
 * 
 * @param p_this Pointer to an fsm struct that contains an fsm_rx_t
 * @return uint32_t 
 */
uint32_t fsm_rx_get_code (fsm_t *p_this);

/**
 * @brief Retrieve if the code received is a repetition or not
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @return true 
 * @return false 
 */
bool fsm_rx_get_repetition (fsm_t *p_this);

/**
 * @brief Retrieve if the code received is noise (an error), or not.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t.
 * @return true 
 * @return false 
 */
bool fsm_rx_get_error_code (fsm_t *p_this);

/**
 * @brief Set the status of the infrared receiver (on or off).
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @param status true to turn the infrared receiver FSM ON. false to turn the infrared receiver FSM OFF.
 */
void fsm_rx_set_rx_status (fsm_t *p_this, bool status);

/**
 * @brief Reset the code and reception status values.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t.
 */
void fsm_rx_reset_code (fsm_t *p_this);

#endif
