/**
 * @file fsm_retina_v2.h
 * @brief Header for fsm_retina_v2.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 22/02/2023
 */
#ifndef FSM_RETINA_V2_H_
#define FSM_RETINA_V2_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Other includes */
#include "fsm.h"
#include "port_lcd.h"


/* Defines */
#define NUMBER_BUTTONS 3    /*!< Number of buttons of the controller (3 min: on_button, change_button, and generic button)*/
#define NUMBER_CONTROLLERS 5    /*!< Number of controllers to store*/
#define CONTROLLERS_INITIALLY_AV 2   /*!< Number controllers initially available*/

#define BUTTON_0_ID 0       /*!< Identification for button 0*/

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
 * @param button_arr Array with the pointers to fsm_button_t structs.
 * @param button_press_time Duration in ms of the button press to change between transmitter and receiver modes.
 * @param p_fsm_tx Pointer to an fsm_t struct that contains an fsm_tx_t.
 * @param p_fsm_rx Pointer to an fsm_t struct that contains an fsm_rx_t.
 * @param rgb_id Identifier number of the board rgb.
 * @return fsm_t* 
 */
fsm_t *fsm_retina_new_v2(fsm_t* button_arr[], uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id);

/**
 * @brief Initialize the infrared transmitter FSM
 * 
 * This function initializes the default values of the FSM struct. On the basic version of Retina, it does not have HW (such as status LEDs, as in example); in thtat case, this fuction must call to the port to initialize the HW associated
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t.
 * @param button_arr Pointer to an array with pointers to fsm_button_t structs.
 * @param button_press_time_ms Duration in ms of the button press to change between transmitter and receiver modes.
 * @param p_fsm_tx Pointer to an fsm_t struct that contains an fsm_tx_t.
 * @param p_fsm_rx Pointer to an fsm-t struct that contains an fsm_rx_t.
 * @param rgb_id Identifier number of the board rgb.
 */
void fsm_retina_init_v2(fsm_t *p_this, fsm_t* button_arr[], uint32_t button_press_time_ms, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id);

#endif

