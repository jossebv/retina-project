/**
 * @file port_buzzer.c
 * @author Jose Manuel Bravo
 * @brief File for controlling an active buzzer.
 * @version 0.1
 * @date 2023-05-17
 * 
 */

/* Includes */
#include "port_buzzer.h"

/* Structs */
typedef struct{
    GPIO_TypeDef *p_port; /*!< Port of the buzzer */
    uint8_t pin;  /*!< Pin of the buzzer */
} port_buzzer_t;

/* Global variables */
static port_buzzer_t port_buzzer_arr[] = {
    [BUZZER_0_ID] = {.p_port = BUZZER_0_PORT, .pin = BUZZER_0_PIN},
};

/* Functions */

void port_buzzer_play(uint8_t buzzer_id)
{
    port_buzzer_t buzzer_hw = port_buzzer_arr[buzzer_id];

    for (uint8_t i = 0; i < 20; i++)
    {
        port_system_gpio_write(buzzer_hw.p_port, buzzer_hw.pin, true);
        port_system_delay_ms(2);
        port_system_gpio_write(buzzer_hw.p_port, buzzer_hw.pin, false);
        port_system_delay_ms(2);
    }
    port_system_delay_ms(50);
    for (uint8_t i = 0; i < 30; i++)
    {
        port_system_gpio_write(buzzer_hw.p_port, buzzer_hw.pin, true);
        port_system_delay_ms(2);
        port_system_gpio_write(buzzer_hw.p_port, buzzer_hw.pin, false);
        port_system_delay_ms(2);
    }
}

void port_buzzer_init(uint8_t buzzer_id)
{
    port_buzzer_t buzzer_hw = port_buzzer_arr[buzzer_id];

    port_system_gpio_config(buzzer_hw.p_port, buzzer_hw.pin, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
}