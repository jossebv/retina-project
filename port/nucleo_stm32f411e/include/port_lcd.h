/**
 * @file port_lcd.h
 * @brief Header for port_lcd.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 2023-02-02
 */

#ifndef PORT_LCD_H
#define PORT_LCD_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "port_system.h"
#include "retina.h"

/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define LCD_0_ID 0
#define LCD_0_PORT GPIOC
#define LCD_0_PIN 0

/* Messages -------------------------------------------------------------------*/


/* Instructions ---------------------------------------------------------------*/
#define CLR_DISPLAY 0x01  /*!< Clears the display*/
#define RTRN_HOME 0x02
#define ENTRY_MODE_SET(i, sh) (0x4 + i*0x2 + sh)   /* !< i-increment(1) or decrement(0) memory cursor (R/L), SH- enables lcd shifting*/
#define DISPLAY_ON_OFF(d, c, b) (0x8 + d*0x4 + c*0x2 + b)   /* !< on/off: d-display, c-cursor, b-blinking cursor*/
#define CURSOR_DISPLAY_SHIFT(s, r) (0x10 + 0x8*s + 0x4*r)   /* !< pending */
#define FUNCTION_SET(dl, n, f) (0x20 + 0x10*dl + 0x08*n + 0x04*f)    /* !< dl- 8bit/4bit input, n- 2 lines/1 line, f-font 11 dots/8 dots*/


/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Initialize the LCD with a 8 pin interface
 * 
 * 
 * @param lcd_id Id number of the LCD, it is unique for each LCD
 */
void port_lcd_init(uint8_t lcd_id);

/**
 * @brief Function to send a string to the lcd
 * 
 * @param lcd_id Id number of the LCD, it is unique for each LCD
 * @param string String to be printed
 */
void port_lcd_print(uint8_t lcd_id, char string[]);



#endif

