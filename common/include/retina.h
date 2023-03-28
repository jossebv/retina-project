/**
 * @file retina.h
 * @brief Header for retina.c file.
 * @author Sistemas Digitales II
 * @date 2023-01-01
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef RETINA_H_
#define RETINA_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_lcd.h"

/* Other includes */
#include "fsm_button.h"
#include "fsm_tx.h"
#include "fsm_retina.h"
#include "fsm_retina_v2.h"
#include "fsm_rx.h"

/* Versions and peripherals */
#define VERSION 2
#define LCD_CONNECTED true


#endif /* RETINA_H_ */
