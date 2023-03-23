/**
 * @file picovoice.c
 * @brief Picofile main programm
 * @author José Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
*/

#include "picovoice.h"

#define MEMORY_BUFFER_SIZE ${MEMORY_BUFFER_SIZE}


static const char* ACCESS_KEY = "${ACCESS_KEY}";


static uint8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));


static const uint8_t *keyword_array = ${KEYWORD_ARRAY};
const float porcupine_sensitivity = 0.5f;


static void wake_word_callback(void) {
    // logic to execute upon detection of wake word
}


static const uint8_t *context_array = ${CONTEXT_ARRAY};
const float rhino_sensitivity = 0.75f;
const float rhino_endpoint_duration_sec = 1.0f;
const bool rhino_require_endpoint = true;


static void inference_callback(pv_inference_t *inference) {
    // `inference` exposes three immutable properties:
    // (1) `IsUnderstood`
    // (2) `Intent`
    // (3) `Slots`
    // ..
    pv_inference_delete(inference);
}


pv_picovoice_t *picovoice = NULL;


const pv_status_t status = pv_picovoice_init(
        ACCESS_KEY,
        MEMORY_BUFFER_SIZE,
        memory_buffer,
        sizeof(keyword_array),
        keyword_array,
        porcupine_sensitivity,
        wake_word_callback,
        sizeof(context_array),
        context_array,
        rhino_sensitivity,
        rhino_endpoint_duration_sec,
        rhino_require_endpoint,
        inference_callback,
        &picovoice);


if (status != PV_STATUS_SUCCESS) {
    // error handling logic
}

