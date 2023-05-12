/**
 * @file fsm_voice.h
 * @brief Header for fsm_voice.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 04/05/2023
 */

#ifndef FSM_VOICE_H_
#define FSM_VOICE_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Other includes */
#include "fsm.h"


/* Defines ------------------------------------------------------------------*/
#define MEMORY_BUFFER_SIZE (70 * 1024)      /*!< Memory buffer size for the picovoice audio processing */

/**
 * @brief Creates the FSM for controlling the voice commands
 *
 * @return Pointer to an fsm_t struct that contains an fsm_voice_t
 */
fsm_t *fsm_voice_new();

#endif