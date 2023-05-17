/**
 * @file port_rgb.h
 * @brief Header for port_rgb.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 08/05/2023
 */

#ifndef PORT_RGB_H
#define PORT_RGB_H

/* INCLUDES */
#include <stdio.h>
#include <stdint.h>

/* DEFINES */
#define RGB_0_ID 0       /*!< Button Identifier */
#define RGB_R_0_GPIO GPIOA    /*!< Red LED GPIO */
#define RGB_R_0_PIN 4    /*!< Red LED Pin */
#define RGB_G_0_GPIO GPIOA    /*!< Green LED GPIO */
#define RGB_G_0_PIN 5    /*!< Green LED Pin */
#define RGB_B_0_GPIO GPIOA   /*!< Blue LED GPIO */
#define RGB_B_0_PIN 6    /*!< Blue LED Pin*/

/**
 * @brief Configure the HW specifications of a gicen RGB LED
 * 
 * @param rgb_id This index is used to select the element of the rgb_arr[] array.
 */
void port_rgb_init(uint8_t rgb_id);

/**
 * @brief Set a color on the RGB LED.

This function receives integer values expected to be within the range [0, 255]. With this we can set any color by using PWM. The frequency of the PWM (timer) signal can be the same for the three LEDs. However each one need a dedicated channel. Any frequency higher than 25 Hz should be sufficient to avoid flicking. This would lead us to display up to 256^3=16.777.216 colors.

However, in the basic implementation of this function we are only using the values as true or false. With this we can display 2^3=8 colors (including WHITE and 'BLACK' (all LEDs OFF)).
 * 
 * @param rgb_id This index is used to select the element of the rgb_arr[] array.
 * @param r Intensity level of the RED LED.
 * @param g Intensity level of the GREEN LED.
 * @param b Intensity level of the BLUE LED.
 */
void port_rgb_set_color(uint8_t rgb_id, uint8_t r, uint8_t g, uint8_t b);


/**
 * @brief Function to blink the led
 * 
 * Informs to the user that the operation has been a success
 * 
 * @param rgb_id 
 */
void port_rgb_success(uint8_t rgb_id);

#endif