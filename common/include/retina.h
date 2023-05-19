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
#define VERSION 4               /*!< Version we are running of retina*/
#define LCD_CONNECTED true     /*!< Indicates if there is a LCD connected*/
#define PICOVOICE_ACTIVE true   /*!< Indicates if the we are using voice recognition*/



#endif /* RETINA_H_ */
