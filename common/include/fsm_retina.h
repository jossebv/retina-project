/**
 * @file fsm_retina.h
 * @brief Header for fsm_retina.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date fecha
 */
#ifndef FSM_RETINA_H_
#define FSM_RETINA_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Other includes */
#include "fsm.h"
#include "port_lcd.h"

/* Function prototypes and explanation ---------------------------------------*/
/**
 * @brief Create a new RETINA FSM
 * 
 * This FSM is the mains state machine of the Retina system that governs the interaction between the otrher state machines of the system: button, trasmitter ans receiver FSM.
 * 
 *  -The FSM starts in the WAIT RX state and checks if the button has been pressed for more than button_press_time_ms ms. If so, it goes to the WAIT_TX state to wait to transmit.
 *  -When in the WAIT TX state, it checks if the button has been pressed for more than button_press_time_ms ms. If so, it goes to the WAIT RX state to wait to receive.
 *  -Being in WAIT RX or WAIT TX, if no machine has any activity (neither the button's FSM, nor the transmitter's, nor the receiver's), it goes to low power mode SLEEP RX or SLEEP TX, respectively.
 *  -While in SLEEP RX or SLEEP TX, it checks that no machine has activity, and, by means of an autotransition, it goes to sleep. These autotransitions are used to avoid staying awake by any interruption from other elements than ours, or the debugger.
 *  -When waking up by one of our system elements, being in SLEEP RX or SLEEP TX, it will always switch (check_true()) to WAIT RX or WAIT TX, respectively.
 * 
 * @param p_fsm_button User button fsm
 * @param button_press_time_ms Duration in ms of the button press to change between transmitter and receiver modes.
 * @param p_fsm_tx Infrared transmitter FSM
 * @return fsm_t* 
 */
fsm_t *fsm_retina_new(fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx);

/**
 * @brief Initialize the infrared transmitter FSM
 * 
 * This function initializes the default values of the FSM struct. On the basic version of Retina, it does not have HW (such as status LEDs, as in example); in thtat case, this fuction must call to the port to initialize the HW associated
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t.
 * @param p_fsm_button Pointer to an fsm_t struct that contains a fsm_button_t
 * @param button_press_time_ms Duration in ms of the button press to change between transmitter and receiver modes.
 * @param p_fms_tx Pointer to an fsm_t struct that contains an fsm_tx_t
 */
void fsm_retina_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t button_press_time_ms, fsm_t *p_fms_tx, fsm_t *p_fsm_rx);

#endif

