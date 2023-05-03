/**
 * @file fsm_rx.c
 * @brief Infrared receiver FSM main file.
 * @author Jose Manuel Bravo Pacheco 
 * @author Victor Garcia Garcia
 * @date 27/02/2023
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>

/* Other includes */
#include "fsm_rx.h"
#include "fsm_rx_nec.h"
#include "port_system.h"
#include "port_rx.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Struct to define the RX FSM
*/
typedef struct
{
  fsm_t f;  /*!< Infrared receiver FSM*/
  fsm_t *p_fsm_rx_nec;  /*!< Pointer to the FSM to parse NEC protocol codes*/
  uint32_t message_timeout_ms;  /*!< Time in milliseconds after which, if no edge is detected, the processing process begins*/
  uint32_t last_tick; /*!< Time-tick when of last edge detected*/
  uint32_t num_edges_detected;  /*!< Number of edges detected*/
  uint32_t code;  /*!< Code received once it is parsed*/
  bool is_repetition; /*!< Indicate if the received code is a repetition code or not*/
  bool is_error;  /*!< Indicate if the received code is an error or not*/
  bool status;  /*!< Indicate if the infrared receiver must be turned on, or off*/
  uint8_t rx_id;  /*!< Receiver ID. Must be unique*/
} fsm_rx_t;

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_RX {
  OFF_RX = 0,
  IDLE_RX,
  WAIT_RX
};

/* State machine input or transition functions */

/**
 * @brief Check if the infrared receiver must be turned ON.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @return true 
 * @return false 
 */
static bool check_on_rx (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;
  
  return p_fsm->status;
}

/**
 * @brief Check if the infrared receiver must be turned OFF.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @return true 
 * @return false 
 */
static bool check_off_rx (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  return !p_fsm->status;
}

/**
 * @brief Check if there has been a change in the GPIO connected to the infrared receiver
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @return true 
 * @return false 
 */
static bool check_edge_detection (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  uint32_t edges = port_rx_get_num_edges(p_fsm->rx_id);
  return edges!=p_fsm->num_edges_detected;
}

/**
 * @brief Checks if a timeout has elapsed without detecting any change in infrared reception.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 * @return true 
 * @return false 
 */
static bool check_timeout (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  return (port_system_get_millis()-p_fsm->last_tick > p_fsm->message_timeout_ms);
}

/* State machine output or action functions */

/**
 * @brief Start the infrared reception system (when the system changes to receiver mode)
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_t
 */
static void do_rx_start (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  p_fsm->p_fsm_rx_nec = fsm_rx_NEC_new();
  port_rx_tmr_start();

  p_fsm->num_edges_detected = 0;

  port_rx_clean_buffer(p_fsm->rx_id);
  port_rx_en(p_fsm->rx_id, true);
}

/**
 * @brief Stop the infrared reception system (when the system changes to tramsmitter mode).
 * 
 * @param p_this Pointer to an fsm_t struct that containd an fsm_rx_t.
 */
static void do_rx_stop (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  port_rx_tmr_stop();
  port_rx_en(p_fsm->rx_id, false);
  fsm_destroy(p_fsm->p_fsm_rx_nec);
}

/**
 * @brief Transcribes the received code information using the time_ticks of the edges detected by the infrared receiver.
 * 
 * @param p_this Pointer to an fsm_t struct that containd an fsm_rx_t.
 */
static void do_store_data (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  uint16_t* edge_ticks = port_rx_get_buffer_edges(p_fsm->rx_id);
  uint32_t num_edges = port_rx_get_num_edges(p_fsm->rx_id);

  p_fsm->is_repetition = fsm_rx_NEC_parse_code(p_fsm->p_fsm_rx_nec, edge_ticks, num_edges, &p_fsm->code);

  p_fsm->is_error = (p_fsm->code == 0x00 && !p_fsm->is_repetition);

  p_fsm->num_edges_detected = 0;
  port_rx_clean_buffer(p_fsm->rx_id);
}

/**
 * @brief Update the time of the last tick and the number of edges detected
 * 
 * @param p_this Pointer to an fsm_t struct that containd an fsm_rx_t.
 */
static void do_update_len_and_timeout (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  p_fsm->last_tick = port_system_get_millis();
  p_fsm->num_edges_detected = port_rx_get_num_edges(p_fsm->rx_id);
}

/* Transition table*/
static fsm_trans_t fsm_trans_rx[] = {
  {OFF_RX, check_on_rx, IDLE_RX, do_rx_start},
  {IDLE_RX, check_off_rx, OFF_RX, do_rx_stop},
  {IDLE_RX, check_edge_detection, WAIT_RX, do_update_len_and_timeout},
  {WAIT_RX, check_edge_detection, WAIT_RX, do_update_len_and_timeout},
  {WAIT_RX, check_timeout, IDLE_RX, do_store_data},
  {-1, NULL, -1, NULL}
};


/* Other auxiliary functions */

void fsm_rx_set_rx_status (fsm_t *p_this, bool status)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  p_fsm->status = status;
}

uint32_t fsm_rx_get_code (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  return p_fsm->code;
}

bool fsm_rx_get_repetition (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  return p_fsm->is_repetition;
}

bool fsm_rx_get_error_code (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  return p_fsm->is_error;
}

void fsm_rx_reset_code (fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;

  p_fsm->code = 0x00;
  p_fsm->is_repetition = false;
  p_fsm->is_error = false;
}

void fsm_rx_init(fsm_t *p_this, uint8_t rx_id)
{
  fsm_rx_t *p_fsm = (fsm_rx_t*) p_this;
  fsm_init(p_this, fsm_trans_rx);

  p_fsm->rx_id = rx_id;
  p_fsm->code = 0;
  p_fsm->num_edges_detected = 0;
  p_fsm->last_tick = 0;
  p_fsm->is_repetition = false;
  p_fsm->is_error = false;
  p_fsm->status = false;  //¿Primero apagada?
  p_fsm->message_timeout_ms = NEC_MESSAGE_TIMEOUT_US/1000;
  p_fsm->f.current_state = OFF_RX;

  port_rx_init(rx_id);
}

fsm_t *fsm_rx_new(uint8_t rx_id)
{
  fsm_t *p_fsm = malloc(sizeof(fsm_rx_t));
  fsm_rx_init(p_fsm, rx_id);
  return p_fsm;
}

bool fsm_rx_check_activity(fsm_t* p_this)
{
  fsm_rx_t* p_fsm = (fsm_rx_t*) p_this;

  return (p_fsm->f.current_state == WAIT_RX);
}