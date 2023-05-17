/**
 * @file retina.c
 * @brief Main file.
 * @author Sistemas Digitales II
 * @date 2023-01-01
 */

/* Includes ------------------------------------------------------------------*/
#include "retina.h"

#include "fsm_voice.h"

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Inicializamos el sistema*/
    port_system_init();

    if (LCD_CONNECTED)
    {
        /* Inicializamos pantalla*/
        port_lcd_init(0);

        port_lcd_print(0, "Modo TX");
    }

    /* Creamos el array de botones*/
    fsm_t *button_arr[NUMBER_BUTTONS];

    if (VERSION < 5)
    {
        button_arr[0] = fsm_button_new(150, 0);
    }
    else
    {
        for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
        {
            button_arr[i] = fsm_button_new(150, i);
        }
    }

    /*Creamos el transmisor*/
    fsm_t *p_fsm_tx = fsm_tx_new(0);

    /*Creamos el receptor*/
    fsm_t *p_fsm_rx = fsm_rx_new(0);

    /* Creamos el picovoice */
    fsm_t* p_fsm_voice = NULL;
    if (PICOVOICE_ACTIVE)
    {
        p_fsm_voice = fsm_voice_new(p_fsm_tx);
    }

    fsm_t *p_fsm_retina;

    if (VERSION < 5)
    {
        p_fsm_retina = fsm_retina_new(button_arr[0], 1000, p_fsm_tx, p_fsm_rx, 0);
    }
    else
    {
        p_fsm_retina = fsm_retina_new_v2(button_arr, 1000, p_fsm_tx, p_fsm_rx, 0);
    }

    while (1)
    {
        for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
        {
            fsm_fire(button_arr[i]);
        }
        if (VERSION >= 2)
        {
            fsm_fire(p_fsm_tx);
        }
        if (VERSION >= 3)
        {
            fsm_fire(p_fsm_rx);
        }
        if (PICOVOICE_ACTIVE)
        {
            fsm_fire(p_fsm_voice);
        }
        else
        {
            fsm_fire(p_fsm_retina);
        }
    }

    /*Destroy de fsm*/
    for (uint8_t i = 0; i < NUMBER_BUTTONS; i++)
    {
        fsm_destroy(button_arr[i]);
    }
    fsm_destroy(p_fsm_tx);
    fsm_destroy(p_fsm_retina);
}
