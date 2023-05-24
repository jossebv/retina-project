/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 23/02/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_button.h"
#include "port_button.h"
#include "port_system.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Struct to define the FSM of a button
*/
typedef struct
{
    fsm_t f;    /*!< Button FSM */
    uint32_t debounce_time; /*!< Button debounce time*/
    uint32_t next_timeout;  /*!< Next timeout for the anti-debounce in ms*/
    uint32_t tick_pressed; /*!< Number of ticks when the button was pressed*/
    uint32_t duration;  /*!< How much time the button has been pressed*/
    uint32_t button_id; /*!< Button ID. Must be unique*/
}fsm_button_t;
/* Defines and enums ----------------------------------------------------------*/

/* Enums */
enum {
    BUTTON_RELEASED = 0,
    BUTTON_RELEASED_WAIT,
    BUTTON_PRESSED,
    BUTTON_PRESSED_WAIT
};
/* State machine input or transition functions */

/**
 * @brief Check if the button has been pressed
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 * @return true  
 * @return false 
 */
static bool check_button_pressed (fsm_t *p_this)
{
    fsm_button_t *p_fms = (fsm_button_t*) p_this;
    return port_button_is_pressed(p_fms->button_id);
}

/**
 * @brief Checks if the button has been released
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 * @return true 
 * @return false 
 */
static bool check_button_released(fsm_t *p_this)
{
    fsm_button_t *p_fms = (fsm_button_t*) p_this;
    return !port_button_is_pressed(p_fms->button_id);
}

/**
 * @brief Check if the debounce-time has passed
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 * @return true 
 * @return false 
 */
static bool check_timeout(fsm_t *p_this)
{
    fsm_button_t *p_button = (fsm_button_t*) p_this;
    uint32_t tick = port_button_get_tick();
    if(tick >= p_button->next_timeout)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* State machine output or action functions */
/**
 * @brief Store the duration of the button press.
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 */
static void do_set_duration (fsm_t *p_this)
{   
    fsm_button_t *p_button = (fsm_button_t*) p_this;
    uint32_t tick = port_button_get_tick();
    p_button->duration = (tick-p_button->tick_pressed);
    p_button->next_timeout = (tick+p_button->debounce_time);
}

/**
 * @brief Store the system tick when the button was pressed
 * 
 * @param p_this Pointer to an fsm_t struct that contains an fsm_button_t
 */
static void do_store_tick_pressed(fsm_t *p_this)
{
    fsm_button_t *p_button = (fsm_button_t*) p_this;
    uint32_t tick = port_button_get_tick();
    p_button->tick_pressed = tick;
    p_button->next_timeout = (tick+p_button->debounce_time);
}

/**
 * @brief Transition table for the button FSM
 * 
 * \image html fsm_button.png
 */
static fsm_trans_t fsm_trans_button[] = {
    {BUTTON_RELEASED, check_button_pressed, BUTTON_PRESSED_WAIT, do_store_tick_pressed},
    {BUTTON_PRESSED_WAIT, check_timeout, BUTTON_PRESSED, NULL},
    {BUTTON_PRESSED, check_button_released, BUTTON_RELEASED_WAIT, do_set_duration},
    {BUTTON_RELEASED_WAIT, check_timeout, BUTTON_RELEASED, NULL},
    {-1,NULL,-1,NULL}
};

/* Other auxiliary functions */


fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm, debounce_time, button_id);
    return p_fsm;
}

void fsm_button_init(fsm_t *p_this, uint32_t debounce_time, uint32_t button_id)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    fsm_init(p_this, fsm_trans_button);


    p_fsm->debounce_time = debounce_time;
    p_fsm->button_id = button_id;
    p_fsm->tick_pressed = 0;
    p_fsm->duration = 0;
    port_button_init(button_id);
}

/**
 * @brief Get the duration of the last button press
 * 
 * @param p_this Pointer to a fsm_t struct that contains an fsm_button_t
 * @return uint32_t 
 */
uint32_t fsm_button_get_duration(fsm_t *p_this)
{
    fsm_button_t *p_button = (fsm_button_t*) p_this;
    return p_button->duration;
}

void fsm_button_reset_duration(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t*) p_this;
    p_fsm->duration=0;
}

bool fsm_button_check_activity(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t*) p_this;

    return (p_fsm->f.current_state != BUTTON_RELEASED);
}
