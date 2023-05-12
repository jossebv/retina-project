/**
 * @file port_lcd.c
 * @brief LCD port configuration.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 2023-02-02
 */

/* Includes ------------------------------------------------------------------*/
#include "port_lcd.h"


/* Definitions ----------------------------------------------------------------*/
#define BASE_RS_MASK 0x1 
#define BASE_RW_MASK 0x1
#define OUTPUT_MASK 0x01
#define NO_PUPD_MASK 0x00

#define LCD_INT_TIM_US 10

#define ENABLE (pin+4)
#define RS (pin+5)

/*Typedefs*/
/**
 * @brief Struct to define the HW dependencies of a lcd display
*/
typedef struct
{
    GPIO_TypeDef *p_port;
    uint8_t pin;
}port_lcd_t;

/*Global variables*/
static port_lcd_t lcd_array[] = {
    [LCD_0_ID] = {.p_port = LCD_0_PORT, .pin = LCD_0_PIN}
};

/* Setup functions */

/**
 * @brief Function that times the information sending to the LCD
 * 
 * @param port Port where the LCD is connected
 * @param pin Lowest pin number (D0) cant be greater than 6
 * @param data 8 bits to be sent
 * @param mode Writing instruction(0) or message(1)
 */
void port_lcd_write(GPIO_TypeDef *port, uint8_t pin, uint8_t data, uint8_t mode)
{
    port_system_gpio_write(port, RS, mode); /* Set the RS pin*/

    for(size_t i = 0; i < 8; i++)  /* Write the output on the pins*/
    {
        bool value = ((data & BIT_POS_TO_MASK((i))) != 0);
        port_system_gpio_write(port, pin + i, value);
    }

    port_system_delay_micros(1);    /* Wait for 10 microseconds*/
    port_system_gpio_write(port, ENABLE, 1);    /* Set ENABLE to 1*/
    port_system_delay_micros(10);    /* Wait for 100 microseconds*/
    port_system_gpio_write(port, ENABLE, 0);    /* Set ENABLE to 0*/
    port_system_delay_micros(10);    /* Wait for 100 microseconds*/
    port_system_gpio_clear(port);   /* Clears the output*/
}

void port_lcd_write_4pin(GPIO_TypeDef *port, uint8_t pin, uint8_t data, uint8_t mode, uint8_t bits) //bits = 1, envio los 8
{
    port_system_gpio_write(port, RS, mode); /* Set the RS pin*/

    for(size_t i = 4; i < 8; i++)  /* Write the output on the pins*/
    {
        bool value = ((data & BIT_POS_TO_MASK((i))) != 0);
        port_system_gpio_write(port, pin + i - 4, value);
    }

    port_system_delay_micros(1);    /* Wait for 10 microseconds*/
    port_system_gpio_write(port, ENABLE, 1);    /* Set ENABLE to 1*/
    port_system_delay_micros(10);    /* Wait for 100 microseconds*/
    port_system_gpio_write(port, ENABLE, 0);    /* Set ENABLE to 0*/
    port_system_delay_micros(10);    /* Wait for 100 microseconds*/

    //lower bits
    if(bits){
        for(size_t i = 0; i < 4; i++)  /* Write the output on the pins*/
        {
            bool value = ((data & BIT_POS_TO_MASK((i))) != 0);
            port_system_gpio_write(port, pin + i, value);
        }

        port_system_delay_micros(1);    /* Wait for 10 microseconds*/
        port_system_gpio_write(port, ENABLE, 1);    /* Set ENABLE to 1*/
        port_system_delay_micros(10);    /* Wait for 100 microseconds*/
        port_system_gpio_write(port, ENABLE, 0);    /* Set ENABLE to 0*/
        port_system_delay_micros(10);    /* Wait for 100 microseconds*/
    }
    port_system_gpio_clear(port);   /* Clears the output*/
}

static void port_lcd_config_4pin(GPIO_TypeDef *port, uint8_t pin)
{
    port_system_delay_micros(5);    /* Waits 50 microseconds*/
    port_lcd_write_4pin(port,pin, FUNCTION_SET(1,0,0), 0,0);   
    port_system_delay_micros(1);    /* Waits 10 microseconds*/
    port_lcd_write_4pin(port,pin, FUNCTION_SET(1,0,0), 0,0);
    port_system_delay_micros(1);    /* Waits 10 microseconds*/
    port_lcd_write_4pin(port,pin, FUNCTION_SET(1,0,0), 0,0);
    port_system_delay_micros(1);    /* Waits 10 microseconds*/

    port_lcd_write_4pin(port,pin, FUNCTION_SET(0,0,0), 0,0); /* One line, resolution of 8 points*/
    port_system_delay_micros(5);
    port_lcd_write_4pin(port,pin, DISPLAY_ON_OFF(0,0,0), 0,1); /* Display off*/
    port_system_delay_micros(5);
    port_lcd_write_4pin(port,pin, CLR_DISPLAY, 0,1);   /* Clear display*/
    port_system_delay_ms(2);
    port_lcd_write_4pin(port, pin, ENTRY_MODE_SET(1,0), 0,1);  /* Position on DDRAM increments when writing*/
    port_system_delay_micros(1);
    port_lcd_write_4pin(port, pin, DISPLAY_ON_OFF(1,0,0), 0,1); /* Display on*/
}

void port_lcd_init(uint8_t lcd_id)
{
    GPIO_TypeDef *p_port = lcd_array[lcd_id].p_port;
    uint8_t pin = lcd_array[lcd_id].pin;
    /* Initialize all pins needed for the outputs*/
    for (uint8_t i = 0; i < 6; i++)
    {
        port_system_gpio_config(p_port, pin+i, OUTPUT_MASK, NO_PUPD_MASK);
    }
    //MODIFICAR AQUÍ PARA QUE EL TIM5 SIGA SIENDO 10us
    uint64_t psc = ((SystemCoreClock/2)/1000000*LCD_INT_TIM_US)/((65535+1)) - 1;
    uint64_t arr = ((SystemCoreClock/2)*LCD_INT_TIM_US)/(1000000*(psc+1)) - 1;
    port_system_timer5_init(arr,psc); /* Enables the timer 5 to interrupt each 10 us*/
    port_lcd_config_4pin(p_port, pin); /* Instructions to setup the display*/
    port_lcd_write_4pin(p_port, pin, CLR_DISPLAY, 0, 1);
}


/* Other auxiliary functions */

void port_lcd_print(uint8_t lcd_id, char string[])
{
    if (!LCD_CONNECTED)
    {
        return;
    }
    
    GPIO_TypeDef *p_port = lcd_array[lcd_id].p_port;
    uint8_t pin = lcd_array[lcd_id].pin;

    port_lcd_write_4pin(p_port, pin, CLR_DISPLAY, 0,1); /* Clears the display*/
    port_system_delay_ms(2);    /* Waits for the display to be cleaned*/
    uint8_t i = 0;
    while(string[i] != 0x0)
    {
        port_lcd_write_4pin(p_port, pin, string[i], 1,1);    /* Writes the char on the LCD memory*/
        i++;
        port_system_delay_micros(5); /* Waits for the instruction be done*/
    }
   
}