/**
 * @file port_button.c
 * @brief File containing functions related to the HW of the button FSM.
 *
 * This files defines an internal struct which coontains the HW information of the 
 *
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 09/02/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "port_button.h"
#include "port_rx.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structo to define the HW dependencies of a button
*/
typedef struct
{
    GPIO_TypeDef *p_port;   
    uint8_t pin;
    bool flag_pressed;
} port_button_hw_t;


/**
 * @brief Array of elements that represents the HW characteristics of the buttons
 * 
 */
static port_button_hw_t buttons_arr[] = {
    [BUTTON_0_ID] = {.p_port = BUTTON_0_GPIO, .pin = BUTTON_0_PIN, .flag_pressed = false},
    [BUTTON_1_ID] = {.p_port = BUTTON_1_GPIO, .pin = BUTTON_1_PIN, .flag_pressed = false},
    [BUTTON_2_ID] = {.p_port = BUTTON_2_GPIO, .pin = BUTTON_2_PIN, .flag_pressed = false},
    [BUTTON_3_ID] = {.p_port = BUTTON_3_GPIO, .pin = BUTTON_3_PIN, .flag_pressed = false},
    [BUTTON_4_ID] = {.p_port = BUTTON_4_GPIO, .pin = BUTTON_4_PIN, .flag_pressed = false},
};

/* Global functions ------------------------------------------------------------*/
void port_button_interruption_handler(uint8_t button_id)
{
    EXTI->PR |= BIT_POS_TO_MASK(buttons_arr[button_id].pin);
    if (buttons_arr[button_id].p_port->IDR & BIT_POS_TO_MASK(buttons_arr[button_id].pin))
    {
        buttons_arr[button_id].flag_pressed = false;
    }
    else
    {
        buttons_arr[button_id].flag_pressed = true;
    }
}

void port_button_init(uint32_t button_id)
{
    GPIO_TypeDef *p_port = buttons_arr[button_id].p_port;
    uint8_t pin = buttons_arr[button_id].pin;

    if(button_id == 0){
        port_system_gpio_config(p_port, pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
    }else{
        port_system_gpio_config(p_port, pin, GPIO_MODE_IN, GPIO_PUPDR_PUP);
    }
    port_system_gpio_config_exti(p_port, pin, 0xf);
    port_system_gpio_exti_enable(pin,1,0);
}

bool port_button_is_pressed(uint32_t button_id)
{
    return buttons_arr[button_id].flag_pressed;
}

uint32_t port_button_get_tick()
{
    return port_system_get_millis();
}

bool* port_button_get_p_flag(uint8_t button_id)
{
    return &buttons_arr[button_id].flag_pressed;
}


//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Handler for interruptions on PIN0
*/
void EXTI0_IRQHandler()
{
    EXTI->PR |= BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin);
    if(buttons_arr[BUTTON_0_ID].p_port->IDR & BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin)){
        buttons_arr[BUTTON_0_ID].flag_pressed = true;
    }else{
        buttons_arr[BUTTON_0_ID].flag_pressed = false;
    }
}