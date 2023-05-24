/**
 * @file fsm_rx_nec.c
 * @brief NEC processing FSM main file.
 * @author Jose Manuel Bravo Pacheco 
 * @author Victor Garcia Garcia
 * @date 27/02/2023
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>

/* Ohter includes */
#include "fsm_rx_nec.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Struct to define the rx_nec FSM
*/
typedef struct
{
  fsm_t f;  /*!< NEC FSM*/
  uint16_t *p_edge_ticks; /*!< Pointer to the array containing the edge time_ticks detected by the infrared receiver*/
  uint32_t num_edges_to_read; /*!< Number of edges detected by the receiver. It is expected 68 edges for a correctly received NEC command*/
  uint32_t bits_remaining_to_read;  /*!< Number of bit remaining to read assuming that a NEC command contains 32 bits*/
  uint32_t code;  /*!< NEC code parsed*/
  bool is_repetition; /*!< To indicate if the code parsed was a repetition or not*/
} fsm_rx_nec_t;

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/**
 * @brief Enumeration with the states of the FSM_RX_NEC
 * 
 */
enum FSM_RX_NEC {
  NEC_IDLE = 0,           /*!< State of the RX_NEC waiting for data*/
  NEC_INIT,               /*!< State of the RX_NEC starting to process the data*/
  NEC_SYMBOL_SILENCE,     /*!< State when a silence has been received*/
  NEC_SYMBOL_PULSE        /*!< State when a pulse has been received*/
};

/* Private functions */
/**
 * @brief Auxiliary function to compute the time difference (in ticks) between the current and the next edge.
 *
 * @param p_edge_ticks Pointer to the array containing the the time ticks of the edges detected by the infrared receiver
 * @return Time difference in ticks
 */
static uint16_t _get_diff_ticks(uint16_t *p_edge_ticks)
{
  return *(p_edge_ticks + 1) - *(p_edge_ticks);
}

/**
 * @brief Auxiliary function to check if a given value is between two other values.
 *
 * This is used to check if a time difference is between the NEC tolerance time interval.
 *
 * @param value Value to check
 * @param min Minimum value allowed by the range (greater than or equal to).
 * @param max Maximum value allowed by the range (lower than or equal to).
 * @return true
 * @return false
 */
bool _value_in_range(uint16_t value, uint16_t min, uint16_t max)
{
  return ((value >= min) && (value <= max));
}

/* State machine input or transition functions */

/**
 * @brief Check if we are parsing the silence (low level) of the NEC prologue
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_init_silence(fsm_t* p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  uint16_t diff = _get_diff_ticks(p_fsm->p_edge_ticks);

  return _value_in_range(diff, NEC_RX_PROLOGUE_TICKS_SILENCE_MIN, NEC_RX_PROLOGUE_TICKS_SILENCE_MAX);
}

/**
 * @brief Assuming that we are parsing the silence (low level) of the NEC prologue, check if it is noise.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_init_noise(fsm_t* p_this)
{
  return !check_is_init_silence(p_this);
}

/**
 * @brief Check if we are parsing the pulse (high level) of the NEC prologue.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_prologue_pulse(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  uint16_t diff = _get_diff_ticks(p_fsm->p_edge_ticks);

  return _value_in_range(diff, NEC_RX_PROLOGUE_TICKS_PULSE_MIN, NEC_RX_PROLOGUE_TICKS_PULSE_MAX);
}

/**
 * @brief Checks if we are parsing the pulse (high level) of a NEC repetition.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_repetition_pulse(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  uint16_t diff = _get_diff_ticks(p_fsm->p_edge_ticks);

  return _value_in_range(diff, NEC_RX_REPETITION_TICKS_PULSE_MIN, NEC_RX_REPETITION_TICKS_PULSE_MAX);
}

/**
 * @brief Assuming that we are parsing the pulse (high level) of the NEC prologue, check if it is noise
 * 
 * @param p_this Pointer to an fsm_t struct that contains a fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_init_pulse_noise(fsm_t *p_this)
{
  return (!check_is_prologue_pulse(p_this) && !check_is_repetition_pulse(p_this));
}

/**
 * @brief Check if we are parsing the last symbol
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_last_symbol(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  return (p_fsm->bits_remaining_to_read == 0);
}

/**
 * @brief Check if we are parsing the silence (low level) of a NEC symbol
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_symbol_silence(fsm_t *p_this)
{
  if (check_is_last_symbol(p_this))
  {
    return false;
  }
  
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  uint16_t diff = _get_diff_ticks(p_fsm->p_edge_ticks);

  return _value_in_range(diff, NEC_RX_SYMBOL_TICKS_SILENCE_MIN, NEC_RX_SYMBOL_TICKS_SILENCE_MAX);
}

/**
 * @brief Assuming that we are parsing the silence (low level) of a NEC symbol, check if it is noise.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_symbol_silence_noise(fsm_t *p_this)
{
  if (check_is_last_symbol(p_this))
  {
    return false;
  }
  
  return !check_is_symbol_silence(p_this);
}

/**
 * @brief Check if we are parsing the pulse (high level) of a NEC symbol 0.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_symbol_0_pulse(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  uint16_t diff = _get_diff_ticks(p_fsm->p_edge_ticks);

  return _value_in_range(diff, NEC_RX_SYMBOL_0_TICKS_PULSE_MIN, NEC_RX_SYMBOL_0_TICKS_PULSE_MAX);
}

/**
 * @brief Check if we are parsing the pulse (high level) of a NEC symbol 1.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_symbol_1_pulse(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  uint16_t diff = _get_diff_ticks(p_fsm->p_edge_ticks);

  return _value_in_range(diff, NEC_RX_SYMBOL_1_TICKS_PULSE_MIN, NEC_RX_SYMBOL_1_TICKS_PULSE_MAX);
}

/**
 * @brief Assuming that we are parsing the pulse (high level) of a NEC symbol, check if it is noise.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 * @return true 
 * @return false 
 */
static bool check_is_symbol_pulse_noise (fsm_t *p_this)
{
  return (!check_is_symbol_0_pulse(p_this) && !check_is_symbol_1_pulse(p_this));
}
/* State machine output or action functions */

/**
 * @brief Move to the next time tick.
 * 
 * The effects of calling this functions are:
 * 
 * **If we are in the state NEC_INIT and we detect a noise, ignore it and move on. It was an error and the FSM comes back to the state NEC_IDLE. **If we are in the state NEC_SYMBOL_SILENCE, we are doing well. Update the information and continue. The FSM goes to the state NEC_SYMBOL_PULSE. **If we are in the state NEC_SYMBOL_PULSE and we detect a noise, ignore it and move on. It was an error and the FSM comes back to the state NEC_IDLE.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_jump_to_next_edge(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->p_edge_ticks++;
  p_fsm->num_edges_to_read--;
} 

/**
 * @brief We have detected a repetition and thus update the corresponding variables
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_repetition_starts(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm-> p_edge_ticks++;
  p_fsm->num_edges_to_read--;
  p_fsm->bits_remaining_to_read = 0;
  p_fsm->is_repetition = true;
}

/**
 * @brief We have detect that a commnand starts and thus update the corresponding variables.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_command_starts(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->p_edge_ticks++;
  p_fsm->num_edges_to_read--;
  p_fsm->bits_remaining_to_read = NEC_FRAME_BITS;
  p_fsm->is_repetition = false;
}

/**
 * @brief Reset the code and move to the next time tick
 * 
 * This happens when we are in the state IDLE_NEC and we detect the silence of a NEC prologue. The FSM goes to the state NEC_INIT. We still do not know if we are at the beggining of a command of a repetition.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_reset_and_jump_to_next_edge(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->p_edge_ticks++;
  p_fsm->num_edges_to_read--;
  p_fsm->code = 0;
}

/**
 * @brief Update the number of edges to read and move 2 time ticks ahead.
 * 
 * This happens when we are in the state NEC_IDLE and we detect a silence that is noise. The FSM remains in the state NEC_IDLE. Cause: there has been 2 edges in the infrared receiver (low and high) but the do not correspond to the beggining of a command nor a repetition. Action: move 2 edges ahead if exist (skipping the following rising edge). This might be a noise before the reception of a command.
 * 
 * Remember that in the stae NEC_IDLE we are in an event edge.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_reset_and_jump_two_edges(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->p_edge_ticks += 2;
  if (p_fsm->num_edges_to_read > 1)
  {
    p_fsm->num_edges_to_read -= 2;
  }
  p_fsm->num_edges_to_read = 0;
}

/**
 * @brief Append a bit 0 to the code parsed
 * 
 * When a NEC symbol 0 is detected we must append a binary 0 to the LEAST SIGNIFICANT Bit (LSB) of the code.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_store_bit_0 (fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->code <<= 1;
  p_fsm->code &= 0xFFFFFFFEUL;

  p_fsm->p_edge_ticks++;
  p_fsm->num_edges_to_read--;
  p_fsm->bits_remaining_to_read--;
}

/**
 * @brief Append a bit 1 to the code parsed
 * 
 * When a NEC symbol 1 is detected we must append a binary 1 to the LEAST SIGNIFICANT Bit (LSB) of the code.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_store_bit_1 (fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->code <<= 1;
  p_fsm->code |= 0x00000001UL;

  p_fsm->p_edge_ticks++;
  p_fsm->num_edges_to_read--;
  p_fsm->bits_remaining_to_read--;
}

/**
 * @brief Finish the signal processing ensuring there is no more edges to read
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
static void do_set_end(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  p_fsm->num_edges_to_read = 0;
}

/* Transition table*/
/**
 * @brief Transition table for the FSM_RX_NEC
 * 
 * \image html fsm_rx_nec.png
 * 
 */
static fsm_trans_t fsm_trans_rx_nec[] = {
  {NEC_IDLE, check_is_init_noise, NEC_IDLE, do_reset_and_jump_two_edges},
  {NEC_IDLE, check_is_init_silence, NEC_INIT, do_reset_and_jump_to_next_edge},
  {NEC_INIT, check_is_init_pulse_noise, NEC_IDLE, do_jump_to_next_edge},
  {NEC_INIT, check_is_repetition_pulse, NEC_SYMBOL_SILENCE, do_repetition_starts},
  {NEC_INIT, check_is_prologue_pulse, NEC_SYMBOL_SILENCE, do_command_starts},
  {NEC_SYMBOL_SILENCE, check_is_last_symbol, NEC_IDLE, do_set_end},
  {NEC_SYMBOL_SILENCE, check_is_symbol_silence_noise, NEC_IDLE, NULL},
  {NEC_SYMBOL_SILENCE, check_is_symbol_silence, NEC_SYMBOL_PULSE, do_jump_to_next_edge},
  {NEC_SYMBOL_PULSE, check_is_symbol_0_pulse, NEC_SYMBOL_SILENCE, do_store_bit_0},
  {NEC_SYMBOL_PULSE, check_is_symbol_1_pulse, NEC_SYMBOL_SILENCE, do_store_bit_1},
  {NEC_SYMBOL_PULSE, check_is_symbol_pulse_noise, NEC_IDLE, do_jump_to_next_edge},
  {-1, NULL, -1, NULL}
};

/* Other auxiliary functions */

/**
 * @brief Initialize a NEC processing FSM.
 *
 * This function initializes the default values of the FSM struct- The FSM process the data for any receiver and it does not contain status information of any of them.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_rx_nec_t
 */
void fsm_rx_NEC_init(fsm_t *p_this)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;
  fsm_init(p_this, fsm_trans_rx_nec);

  p_fsm->code = 0;
  p_fsm->num_edges_to_read = 0;
  p_fsm->p_edge_ticks = NULL;
  p_fsm->is_repetition = false;
}

bool fsm_rx_NEC_parse_code(fsm_t* p_this, uint16_t* p_edge_ticks, uint32_t num_edges, uint32_t* p_code)
{
  fsm_rx_nec_t *p_fsm = (fsm_rx_nec_t*) p_this;

  //p_this->current_state = NEC_IDLE;
  p_fsm->code = 0;
  p_fsm->is_repetition = false;
  p_fsm->p_edge_ticks = p_edge_ticks;
  p_fsm->num_edges_to_read = num_edges;

  while(p_fsm->num_edges_to_read > 1)
  {
    fsm_fire(p_this);
  }
  
  *p_code = p_fsm->code;
  return p_fsm->is_repetition;
}

fsm_t *fsm_rx_NEC_new()
{
  fsm_t *p_fsm = malloc(sizeof(fsm_rx_nec_t));
  fsm_rx_NEC_init(p_fsm);
  return p_fsm;
}