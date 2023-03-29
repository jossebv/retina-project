/**
 * @file fsm_tx.c
 * @brief Infrared transmitter FSM main file.
 * @author Jose Manuel Bravo Pacheco   
 * @author Victor Garcia Garcia
 * @date 22/02/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_tx.h"
#include <stdlib.h>
#include "port_tx.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Struct to define the TX FSM
*/
typedef struct{
    fsm_t f;    /*!< Infrared transmitter fsm*/
    uint32_t code;  /*!< NEC code to be sent*/
    uint8_t tx_id;  /*!< Transmitter ID. Must be unique*/
}fsm_tx_t;

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum{
    WAIT_TX = 0
};

/* NEC private functions */
/**
 * @brief Wait for some ticks. Each tick if the symbol timer lasts for 562.5 microseconds.
 * 
 * @param ticks Number of ticks to wait
 */
static void _wait_for_tx_ticks(uint32_t ticks)
{
    uint32_t tickstart = port_tx_tmr_get_tick();
    while(port_tx_tmr_get_tick()-tickstart < ticks)
    {
    }
}

/**
 * @brief Send a PWM burst and wait for silence
 * 
 * @param tx_id Unique transmitter identifier number
 * @param ticks_ON Number of ticks to wait for an active PWM burst 
 * @param ticks_OFF Number of ticks to wait for a silence
 */
static void _send_NEC_burst (uint8_t tx_id, uint32_t ticks_ON, uint32_t ticks_OFF)
{
    port_tx_pwm_timer_set(tx_id,true);
    _wait_for_tx_ticks(ticks_ON);
    port_tx_pwm_timer_set(tx_id,false);
    _wait_for_tx_ticks(ticks_OFF);
}

/* State machine input or transition functions */
/**
 * @brief Check if it has been received a code other than 0x00
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_tx_t.
 * @return true 
 * @return false 
 */
static bool check_tx_start (fsm_t *p_this)
{
    fsm_tx_t *p_fsm = (fsm_tx_t*) p_this;
    uint32_t code = p_fsm->code;
    return (code != 0x00);
}

/* State machine output or action functions */
/**
 * @brief Start the transmission of the received NEC code
 * 
 * @param p_this 
 */
static void do_tx_start(fsm_t *p_this)
{
    fsm_tx_t *p_fsm = (fsm_tx_t*) p_this;
    fsm_send_NEC_code(p_fsm->tx_id, p_fsm->code);
    p_fsm->code = 0x00;
}

static fsm_trans_t fsm_trans_tx[] = {
    {WAIT_TX, check_tx_start, WAIT_TX, do_tx_start},
    {-1,NULL,-1,NULL},
};

/* Other auxiliary functions */
void fsm_tx_set_code(fsm_t *p_this, uint32_t code)
{
    fsm_tx_t *p_fsm = (fsm_tx_t*) p_this;
    if(code != 0x00)
    {
        p_fsm->code = code;
    }
}

void fsm_send_NEC_code(uint8_t tx_id, uint32_t code)
{
    uint32_t bit_mask = 0x80000000; /*Mask on bit 32*/
    port_tx_symbol_tmr_start();

    _send_NEC_burst(tx_id, NEC_TX_PROLOGUE_TICKS_ON, NEC_TX_PROLOGUE_TICKS_OFF);
    do
    {
        if(code & bit_mask) /* Send a 1*/
        {
            _send_NEC_burst(tx_id, NEC_TX_SYM_1_TICKS_ON, NEC_TX_SYM_1_TICKS_OFF);
        }
        else    /* Send a 0*/
        {
            _send_NEC_burst(tx_id, NEC_TX_SYM_0_TICKS_ON, NEC_TX_SYM_0_TICKS_OFF);
        }
        bit_mask >>= 1;
    } while (bit_mask>0);
    _send_NEC_burst(tx_id, NEC_TX_EPILOGUE_TICKS_ON, NEC_TX_EPILOGUE_TICKS_OFF);    //Envio epilogo
    port_tx_symbol_tmr_stop();
}

fsm_t *fsm_tx_new(uint8_t tx_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_tx_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_tx_init(p_fsm, tx_id);
    return p_fsm;
}

void fsm_tx_init(fsm_t *p_this, uint8_t tx_id)
{
    fsm_tx_t *p_fsm = (fsm_tx_t *)(p_this);
    fsm_init(p_this, fsm_trans_tx);

    p_fsm->tx_id = tx_id;
    p_fsm->code = 0x00;
    port_tx_init(tx_id,false);
}


