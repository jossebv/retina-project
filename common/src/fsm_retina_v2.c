/**
 * @file fsm_retina_v2.c
 * @brief Retina FSM main file.
 * @author Jose Manuel Bravo Pacheco
 * @author VictorGarcia Garcia
 * @date 22/02/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "commands.h"
#include "fsm_button.h"
#include "fsm_tx.h"
#include "fsm_rx.h"
#include "fsm_retina_v2.h"

/* Global variables -----------------------------------------------------------*/
uint32_t tx_1_commands[NUMBER_BUTTONS] = {ON_BUTTON, CHANGE_BUTTON, MY_RED_BUTTON};   /*!< First controller*/
uint32_t tx_2_commands[NUMBER_BUTTONS] = {ON_BUTTON, CHANGE_BUTTON, MY_GREEN_BUTTON}; /*!< Second controller*/
uint32_t tx_3_commands[NUMBER_BUTTONS];                                               /*!< Third controller*/
uint32_t tx_4_commands[NUMBER_BUTTONS];                                               /*!< Fourth controller*/
uint32_t tx_5_commands[NUMBER_BUTTONS];                                               /*!< Fith controller*/

uint32_t *commands_table[NUMBER_CONTROLLERS] = {tx_1_commands, tx_2_commands, tx_3_commands, tx_4_commands, tx_5_commands}; /*!< Table with the directions of the different commands arrays*/

/* Defines and enums ----------------------------------------------------------*/

/* Enums */
enum
{
    OFF_RETINA = 0, /*!< State off of the retina board*/
    WAIT_TX,        /*!< State to wait in transmission mode */
    WAIT_RX,        /*!< State to wait in receiver mode*/
    SLEEP_TX,       /*!< State to be sleeping in transmission mode*/
    SLEEP_RX,       /*!< State to be sleeping in reception mode*/
};

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the Retina FSM on its second version.
 */
typedef struct
{
    fsm_t f;                               /*!< Retina FSM  */
    fsm_t *button_arr[NUMBER_BUTTONS];     /*!< Pointer to the FSM of the user button*/
    fsm_t *p_fsm_tx;                       /*!< Pointer to the FSM of the infrared transmitter*/
    fsm_t *p_fsm_rx;                       /*!< Pointer to the FSM of the infrared receiver*/
    uint32_t long_button_press_ms;         /*!< Duration of the button press to change between transmitter and receiver modes*/
    uint32_t tx_codes_arr[NUMBER_BUTTONS]; /*!< Array to store in the memory of the system a number of codes to send in a loop*/
    uint32_t button_id;                    /*!< Saves which button has been pressed*/
    uint8_t actual_controller;             /*!< The index of the actual controller*/
    uint8_t available_controllers;         /*!< Gives the number of controllers available to work*/
    uint32_t rx_code;                      /*!< Stores the code received*/
    uint8_t rx_index;                      /*!< Index of which button are we programming*/
    uint8_t rgb_id;                        /*!< Identifier of the RGB LED*/
    uint32_t color_mem;
    bool rgb_state;
} fsm_retina_t;

/* Private functions*/

/**
 * @brief Load the commands of the controller to the fsm
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @param commands  Pointer to the array containing the commands to be loaded
 */
void _load_commands(fsm_t *p_this, uint32_t *commands)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    for (uint8_t i = 1; i < NUMBER_BUTTONS; i++)
    {
        p_fsm->tx_codes_arr[i] = commands[i];
    }
}

/* State machine input or transition functions */
/**
 * @brief Check if the button has been pressed fast to send a new command
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_short_pressed_v2(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    for (uint8_t i = 2; i < NUMBER_BUTTONS; i++)
    {
        uint32_t duration = fsm_button_get_duration(p_fsm->button_arr[i]);
        if (duration > 0 && duration < p_fsm->long_button_press_ms)
        {
            p_fsm->button_id = i;
            return true;
        }
    }
    return false;
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
    uint32_t duration = fsm_button_get_duration(p_fsm->button_arr[1]);
    return (duration > p_fsm->long_button_press_ms && duration != 0);
}

/**
 * @brief Checks if the button 0, which will be allways the one of change mode, has been short pressed
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 * @return true
 * @return false
 */
static bool check_change_controller(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint32_t duration = fsm_button_get_duration(p_fsm->button_arr[1]);

    return (duration > 0 && duration < p_fsm->long_button_press_ms);
}

static bool check_on_button(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint32_t duration = fsm_button_get_duration(p_fsm->button_arr[0]);

    return (duration > 0);
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
 * @brief Checks for every button if the fsm is doing any activity
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 *
 * @return true
 * @return false
 */
bool check_buttons_array_activity(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
    {
        if (fsm_button_check_activity(p_fsm->button_arr[i]))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if any of the elements of the system is active
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 *
 * @return true
 * @return false
 */
static bool check_activity(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    return (check_buttons_array_activity(p_this) || fsm_tx_check_activity(p_fsm->p_fsm_tx) || fsm_rx_check_activity(p_fsm->p_fsm_rx));
}

/**
 * @brief Check if any of the elements of the system is not active
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 *
 * @return true
 * @return false
 */
static bool check_no_activity(fsm_t *p_this)
{
    return !check_activity(p_this);
}
///////////////////////////////////////////////////////
/* State machine output or action functions */
///////////////////////////////////////////////////////


/**
 * @brief Transmit the next code stored in memory after a short button press.
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_send_next_msg_v2(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint32_t *tx_codes_arr = p_fsm->tx_codes_arr;
    uint8_t button_id = p_fsm->button_id;

    fsm_tx_set_code(p_fsm->p_fsm_tx, tx_codes_arr[button_id]);
    fsm_button_reset_duration(p_fsm->button_arr[button_id]);

    /*Debug for know which button is pressed*/
    char bufer[16];
    sprintf(bufer, "M:%d - B:%d", p_fsm->actual_controller + 1, button_id-1);
    port_lcd_print(0, bufer);
}

/**
 * @brief Change the array of commands associated with the buttons
 *
 * @param p_this Pointer to an fsm_t struct that contains a fsm_retina_t
 */
static void do_change_controller(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    uint8_t available_controllers = p_fsm->available_controllers;
    uint8_t next_controller = (p_fsm->actual_controller + 1) % available_controllers;

    p_fsm->actual_controller = next_controller;

    _load_commands(p_this, commands_table[next_controller]);
    fsm_button_reset_duration(p_fsm->button_arr[1]);

    /*LCD debug*/

    char bufer[15];
    sprintf(bufer, "Mando: %d", next_controller + 1);
    port_lcd_print(0, bufer);
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

    fsm_button_reset_duration(p_fsm->button_arr[1]);

    p_fsm->rx_index = 2;

    port_lcd_print(0, "Modo RX");
}

/**
 * @brief Switch the system to transmitter mode
 *
 * When this happens, a new controller has been configured, so we must increment the available controllers
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_rx_off_tx_on(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    fsm_rx_set_rx_status(p_fsm->p_fsm_rx, false);

    fsm_button_reset_duration(p_fsm->button_arr[1]);

    // p_fsm->available_controllers++;

    port_lcd_print(0, "Modo TX");
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

    // ACCION AL RECIBIR
    uint8_t rx_index = p_fsm->rx_index;
    if (rx_index < NUMBER_BUTTONS)
    {
        uint32_t *tx_commands = commands_table[p_fsm->available_controllers];
        tx_commands[rx_index] = p_fsm->rx_code;
    }

    char string[16];
    sprintf(string, "0x%08lX", p_fsm->rx_code);
    port_lcd_print(0, string);
    port_rgb_success(p_fsm->rgb_id);

    if (rx_index == NUMBER_BUTTONS - 1)
    {
        p_fsm->available_controllers++;
        do_rx_off_tx_on(p_this);
        p_this->current_state = WAIT_TX;
    }
    p_fsm->rx_index++;
    // FIN ACCION RECIBIR

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
 * @brief  Start the low power mode
 *
 * @param p_this Pointer to an fsm_t struct that contains an fsm_retina_t
 */
static void do_sleep(fsm_t *p_this)
{
    port_system_sleep();
}

static void do_turn_on(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    fsm_button_reset_duration(p_fsm->button_arr[0]);
    do_rx_off_tx_on(p_this);
}

static void do_turn_off(fsm_t *p_this)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;

    fsm_button_reset_duration(p_fsm->button_arr[0]);
    do_rx_off_tx_on(p_this);
    port_lcd_print(0, "Sistema apagado");
    do_sleep(p_this);
}
/*Transition table*/

static fsm_trans_t fsm_trans_retina[] = {
    {OFF_RETINA, check_on_button, WAIT_TX, do_turn_on},
    {WAIT_TX, check_on_button, OFF_RETINA, do_turn_off},
    {WAIT_TX, check_on_button, OFF_RETINA, do_turn_off},
    {WAIT_TX, check_short_pressed_v2, WAIT_TX, do_send_next_msg_v2},
    {WAIT_TX, check_change_controller, WAIT_TX, do_change_controller},
    {WAIT_TX, check_no_activity, SLEEP_TX, do_sleep},
    {SLEEP_TX, check_no_activity, SLEEP_TX, do_sleep},
    {SLEEP_TX, check_activity, WAIT_TX, NULL},
    {WAIT_TX, check_long_pressed, WAIT_RX, do_tx_off_rx_on},
    {WAIT_RX, check_long_pressed, WAIT_TX, do_rx_off_tx_on},
    {WAIT_RX, check_code, WAIT_RX, do_execute_code},
    {WAIT_RX, check_error, WAIT_RX, do_discard_rx_and_reset},
    {WAIT_RX, check_repetition, WAIT_RX, do_execute_repetition},
    {WAIT_RX, check_no_activity, SLEEP_RX, do_sleep},
    {SLEEP_RX, check_no_activity, SLEEP_RX, do_sleep},
    {SLEEP_RX, check_activity, WAIT_RX, NULL},
    {-1, NULL, -1, NULL}};

/* Other auxiliary functions */

fsm_t *fsm_retina_new_v2(fsm_t *button_arr[], uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_retina_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_retina_init_v2(p_fsm, button_arr, button_press_time, p_fsm_tx, p_fsm_rx, rgb_id);
    return p_fsm;
}

void fsm_retina_init_v2(fsm_t *p_this, fsm_t *button_arr[], uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id)
{
    fsm_retina_t *p_fsm = (fsm_retina_t *)p_this;
    fsm_init(p_this, fsm_trans_retina);

    for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
    {
        p_fsm->button_arr[i] = button_arr[i];
    }

    p_fsm->p_fsm_tx = p_fsm_tx;
    p_fsm->p_fsm_rx = p_fsm_rx;
    p_fsm->long_button_press_ms = button_press_time;
    p_fsm->actual_controller = 0;
    p_fsm->available_controllers = CONTROLLERS_INITIALLY_AV;
    p_fsm->rx_code = 0;
    p_fsm->rgb_id = rgb_id;

    port_rgb_init(rgb_id);

    /*Codes array*/
    _load_commands(p_this, commands_table[p_fsm->actual_controller]);

    /*Display*/
    port_lcd_print(0, "Sistema apagado");
}
