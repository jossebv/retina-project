/**
 * @file fsm_retina.c
 * @brief Retina FSM main file.
 * @author Jose Manuel Bravo Pacheco
 * @author VictorGarcia Garcia
 * @date 22/02/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "commands.h"
#include "fsm_retina.h"
#include "fsm_button.h"
#include "fsm_tx.h"
#include "fsm_rx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define COMMANDS_MEMORY_SIZE 3 /*!< Number of NEC commands stored in the memory of the system Retina */

/* Global variables */
uint32_t color_mem = 0x0; /*!< Memory space with the last color when turned off the leds */
bool rgb_state = false;

/* Enums */
enum
{
    WAIT_TX = 0, /*!< State to wait in transmission mode */
    WAIT_RX,     /*!< State to wait in receiver mode*/
    SLEEP_TX,    /*!< State to be sleeping in transmission mode*/
    SLEEP_RX,    /*!< State to be sleeping in reception mode*/
};

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the Retina FSM.
 */
typedef struct
{
    fsm_t f;                                     /*!< Retina FSM  */
    fsm_t *p_fsm_button;                         /*!< Pointer to the FSM of the user button*/
    fsm_t *p_fsm_tx;                             /*!< Pointer to the FSM of the infrared transmitter*/
    fsm_t *p_fsm_rx;                             /*!< Pointer to the FSM of the infrared receiver*/
    uint32_t long_button_press_ms;               /*!< Duration of the button press to change between transmitter and receiver modes*/
    uint32_t tx_codes_arr[COMMANDS_MEMORY_SIZE]; /*!< Array to store in the memory of the system a number of codes to send in a loop*/
    uint8_t tx_codes_index;                      /*!< Index to go through the elements of the tx_codes_arr*/
    uint32_t rx_code;                            /*!< Code received to be processed*/
    uint8_t rgb_id;                              /*!< Identifier of the RGB LED*/
} fsm_retina_t;

/* Private functions */
/**
 * @brief Process the color from the code that is received
 *
 * @param rgb_id Identification of the RGB LED
 * @param code Code received
 */
void _process_rgb_code(uint8_t rgb_id, uint32_t code)
{   
    if (code == MY_ON_BUTTON)
    {
        if (rgb_state)
        {
            port_rgb_set_color(rgb_id, LOW, LOW, LOW);
            rgb_state = !rgb_state;
            return;
        }

        if (color_mem == 0x0)
        {
            port_rgb_set_color(rgb_id, HIGH, HIGH, HIGH);
        }
        else
        {
            _process_rgb_code(rgb_id, color_mem);
        }
        rgb_state = !rgb_state;
        return;
    }
    else if (code == MY_OFF_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, LOW, LOW);
        return;
    }

    if (code == MY_RED_BUTTON)
    {
        port_rgb_set_color(rgb_id, HIGH, LOW, LOW);
    }
    else if (code == MY_GREEN_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, HIGH, LOW);
    }
    else if (code == MY_BLUE_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, LOW, HIGH);
    }
    else if (code == LIL_WHITE_BUTTON)
    {
        port_rgb_set_color(rgb_id, HIGH, HIGH, HIGH);
    }
    color_mem = code;
}

/* State machine input or transition functions */
/**
 * @brief Check if the button has been pressed fast to send a new command
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_short_pressed(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint32_t duration = fsm_button_get_duration(p_fsm->p_fsm_button);
    return (duration < p_fsm->long_button_press_ms && duration > 0);
}

/**
 * @brief Check if the button has been pressed long to change between transmitter and receiver modes.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_long_pressed(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint32_t duration = fsm_button_get_duration(p_fsm->p_fsm_button);
    return (duration > p_fsm->long_button_press_ms && duration != 0);
}

/**
 * @brief Check if there is a new code in the infrared receiver FSM.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_code(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint32_t code = fsm_rx_get_code(p_fsm->p_fsm_rx);

    if (code != 0x00)
    {
        p_fsm->rx_code = code;
    }
    return code;
}

/**
 * @brief Check if there has been a repetition
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_repetition(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    return fsm_rx_get_repetition(p_fsm->p_fsm_rx);
}

/**
 * @brief Check if there has been received an erroneous code
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_error(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    return fsm_rx_get_error_code(p_fsm->p_fsm_rx);
}

/**
 * @brief Checks if any sub fsm is having any activity
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina struct
 * @return true
 * @return false
 */
bool check_activity(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    return (fsm_button_check_activity(p_fsm->p_fsm_button) || fsm_tx_check_activity(p_fsm->p_fsm_tx) || fsm_rx_check_activity(p_fsm->p_fsm_rx));
}

/**
 * @brief Returns the oposite of check_activity function
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina struct
 * @return true
 * @return false
 */
bool check_no_activity(fsm_t *p_this)
{
    return !check_activity(p_this);
}

///////////////////////////////////////////////////////////////////////
/* State machine output or action functions */
///////////////////////////////////////////////////////////////////////
/**
 * @brief Transmit the next code stored in memory after a short button press.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_send_next_msg(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint8_t tx_codes_index = p_fsm->tx_codes_index;
    uint32_t *tx_codes_arr = p_fsm->tx_codes_arr;

    fsm_tx_set_code(p_fsm->p_fsm_tx, tx_codes_arr[tx_codes_index]);
    fsm_button_reset_duration(p_fsm->p_fsm_button);

    /*Debug for know which button is pressed*/
    char bufer[9];
    sprintf(bufer, "%d", tx_codes_index);
    port_lcd_print(0, bufer);

    p_fsm->tx_codes_index++;
    p_fsm->tx_codes_index %= COMMANDS_MEMORY_SIZE;
}

/**
 * @brief Actuate according to the code received
 *
 * This function can be used to act to the RGB LED or any other hardware.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_execute_code(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    char string[16];
    sprintf(string, "0x%08lX", p_fsm->rx_code);
    port_lcd_print(0, string);

    _process_rgb_code(p_fsm->rgb_id, p_fsm->rx_code);

    fsm_rx_reset_code(p_fsm->p_fsm_rx);
}

/**
 * @brief Switch the system to receiver mode
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_tx_off_rx_on(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    fsm_rx_set_rx_status(p_fsm->p_fsm_rx, true);
    // function to enable led -> _process_rgb_code();

    fsm_button_reset_duration(p_fsm->p_fsm_button);

    port_lcd_print(0, "Modo RX");
}

/**
 * @brief Switch the system to transmitter mode
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_rx_off_tx_on(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    fsm_rx_set_rx_status(p_fsm->p_fsm_rx, false);
    // function to disable led -> _process_rgb_code();

    fsm_button_reset_duration(p_fsm->p_fsm_button);

    port_lcd_print(0, "Modo TX");
}

/**
 * @brief Actuate accordingly when receiving a repetition
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_execute_repetition(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    /* TO BE IMPLEMENTED */

    fsm_rx_reset_code(p_fsm->p_fsm_rx);
}

/**
 * @brief Discard and reset the code of the infrared receiver FSM.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_discard_rx_and_reset(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    fsm_rx_reset_code(p_fsm->p_fsm_rx);
}

/**
 * @brief Start the low power mode.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
void do_sleep(fsm_t *p_this)
{
    port_system_sleep();
}

/*Transition table*/

static fsm_trans_t fsm_trans_retina[] = {
    {WAIT_TX, check_short_pressed, WAIT_TX, do_send_next_msg},
    {WAIT_TX, check_long_pressed, WAIT_RX, do_tx_off_rx_on},
    {WAIT_TX, check_no_activity, SLEEP_TX, do_sleep},
    {SLEEP_TX, check_no_activity, SLEEP_TX, do_sleep},
    {SLEEP_TX, check_activity, WAIT_TX, NULL},
    {WAIT_RX, check_code, WAIT_RX, do_execute_code},
    {WAIT_RX, check_repetition, WAIT_RX, do_execute_repetition},
    {WAIT_RX, check_error, WAIT_RX, do_discard_rx_and_reset},
    {WAIT_RX, check_long_pressed, WAIT_TX, do_rx_off_tx_on},
    {WAIT_RX, check_no_activity, SLEEP_RX, do_sleep},
    {SLEEP_RX, check_no_activity, SLEEP_RX, do_sleep},
    {SLEEP_RX, check_activity, WAIT_RX, NULL},
    {-1, NULL, -1, NULL}};

/* Other auxiliary functions */

fsm_t *fsm_retina_new(fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_retina_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_retina_init(p_fsm, p_fsm_button, button_press_time, p_fsm_tx, p_fsm_rx, rgb_id);
    return p_fsm;
}

void fsm_retina_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    fsm_init(p_this, fsm_trans_retina);

    p_fsm->p_fsm_button = p_fsm_button;
    p_fsm->p_fsm_tx = p_fsm_tx;
    p_fsm->p_fsm_rx = p_fsm_rx;
    p_fsm->long_button_press_ms = button_press_time;
    p_fsm->tx_codes_index = 0;
    p_fsm->rgb_id = rgb_id;

    port_rgb_init(rgb_id);

    /*Codes array*/
    p_fsm->tx_codes_arr[0] = MY_RED_BUTTON;
    p_fsm->tx_codes_arr[1] = MY_GREEN_BUTTON;
    p_fsm->tx_codes_arr[2] = MY_BLUE_BUTTON;
}
