/**
 * @file retina.c
 * @brief Main file.
 * @author Sistemas Digitales II
 * @date 2023-01-01
 */

/* Includes ------------------------------------------------------------------*/
#include "retina.h"

/* Definitions ---------------------------------------------------------------*/

/* Variable initialization functions */

/* State machine input or transition functions */

/* State machine output or action functions */

/* Other auxiliary functions */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Inicializamos el sistema*/
    port_system_init();
    
    /* Inicializamos pantalla*/
    port_lcd_init(0);

    port_lcd_print(0, "Modo TX");
    
    /*Creamos los botones*/
    fsm_t* p_fsm_user_button = fsm_button_new(150, 0);
    fsm_t* p_fsm_button_1 = fsm_button_new(200, 1);
    // fsm_t* p_fsm_button_2 = fsm_button_new(200, 2);
    // fsm_t* p_fsm_button_3 = fsm_button_new(200, 3);
    //fsm_t* p_fsm_button_4 = fsm_button_new(200, 4);


    /*Creamos el transmisor*/
    fsm_t* p_fsm_tx = fsm_tx_new(0);

    /*Creamos el receptor*/
    fsm_t *p_fsm_rx = fsm_rx_new(0);

    /* Creamos el array de botones*/
    fsm_t* button_arr[NUMBER_BUTTONS];
    

    button_arr[0] = p_fsm_user_button;
    button_arr[1] = p_fsm_button_1;
    // button_arr[2] = p_fsm_button_2;
    // button_arr[3] = p_fsm_button_3;
    //button_arr[4] = p_fsm_button_4;

    /*Creamos la FSM principal*/
    fsm_t *p_fsm_retina = fsm_retina_new_v2(button_arr, 1000, p_fsm_tx, p_fsm_rx);

    //fsm_t* p_fsm_retina = fsm_retina_new(p_fsm_user_button, 1000, p_fsm_tx, p_fsm_rx);

    while(1)
    {
        /*Fire the fsm*/
        for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
        {
            fsm_fire(button_arr[i]);
        }
        fsm_fire(p_fsm_tx);
        fsm_fire(p_fsm_rx);
        fsm_fire(p_fsm_retina);
    }

    /*Destroy de fsm*/
    for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
    {
        fsm_destroy(button_arr[i]);
    }
    fsm_destroy(p_fsm_tx);
    fsm_destroy(p_fsm_retina);
}
