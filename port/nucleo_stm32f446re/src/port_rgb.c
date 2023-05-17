/**
 * @file port_rgb.c
 * @brief Portable functions to interact with RGB LEDs.
 *
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 08/05/2023
 */

/* Includes */
#include "port_rgb.h"
#include "port_system.h"

/* Structs */
/**
 * @brief structure to define the HW dependencies of an RGB LED.
 *
 */
typedef struct
{
    GPIO_TypeDef *p_port_red;   /*!< Port to which RED LED is connected */
    uint8_t pin_red;            /*!< Pin to which RED LED is connected */
    GPIO_TypeDef *p_port_green; /*!< Port to which GREEN LED is connected */
    uint8_t pin_green;          /*!< Pin to which GREEN LED is connected */
    GPIO_TypeDef *p_port_blue;  /*!< Port to which BLUE LED is connected */
    uint8_t pin_blue;           /*!< Pin to which BLUE LED is connected */
} port_rgb_hw_t;

/* Global variables */
/**
 * @brief Array of elements that represents the HW characteristics of the RGB LEDs
 *
 */
static port_rgb_hw_t rgb_arr[] = {
    [RGB_0_ID] = {.p_port_red = RGB_R_0_GPIO, .pin_red = RGB_R_0_PIN, .p_port_green = RGB_G_0_GPIO, .pin_green = RGB_G_0_PIN, .p_port_blue = RGB_B_0_GPIO, .pin_blue = RGB_B_0_PIN},
};

/* Functions */

void port_rgb_set_color (uint8_t rgb_id, uint8_t r, uint8_t g, uint8_t b)
{
    port_rgb_hw_t p_rgb_hw = rgb_arr[rgb_id];

    port_system_gpio_write(p_rgb_hw.p_port_red, p_rgb_hw.pin_red, r>0);
    port_system_gpio_write(p_rgb_hw.p_port_green, p_rgb_hw.pin_green, g>0);
    port_system_gpio_write(p_rgb_hw.p_port_blue, p_rgb_hw.pin_blue, b>0);
}

void port_rgb_success(uint8_t rgb_id)
{
    port_rgb_hw_t p_rgb_hw = rgb_arr[rgb_id];

    for (uint8_t i = 0; i < 3; i++)
    {
        port_system_gpio_write(p_rgb_hw.p_port_green, p_rgb_hw.pin_green, true);
        port_system_delay_ms(100);
        port_system_gpio_write(p_rgb_hw.p_port_green, p_rgb_hw.pin_green, false);
        port_system_delay_ms(100);
    }
}

void port_rgb_init(uint8_t rgb_id)
{
    port_rgb_hw_t p_rgb_hw = rgb_arr[rgb_id];

    port_system_gpio_config(p_rgb_hw.p_port_red, p_rgb_hw.pin_red, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
    port_system_gpio_config(p_rgb_hw.p_port_green, p_rgb_hw.pin_green, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
    port_system_gpio_config(p_rgb_hw.p_port_blue, p_rgb_hw.pin_blue, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);

    port_rgb_set_color(rgb_id, 0, 0, 0);
}