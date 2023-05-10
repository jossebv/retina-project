/**
 * @file fsm_voice.c
 * @brief FSM for controlling voice detection
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 04/05/2023
 */

#include "retina.h"
#if(PICOVOICE_ACTIVE)

/* Includes --------------------------------------------------------------------*/
#include "fsm_voice.h"
#include "pv_params.h"
#include "fsm_tx.h"
#include "pv_audio_rec.h"
#include "pv_picovoice.h"
#include "stm32f411e_discovery.h"

#include "commands.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Struct to define the VOICE FSM
 */
typedef struct
{
    fsm_t f;                /*!< Infrared transmitter fsm*/
    fsm_t *p_fsm_tx;        /*!< Pointer to an fsm_t struct that contains an fsm_tx_t*/
    pv_picovoice_t *handle; /*!< Picovoice object handler*/
    int16_t *buffer;        /*!< Buffer for saving the audio records*/
} fsm_voice_t;

/* Defines and enums ----------------------------------------------------------*/

static const char *ACCESS_KEY = "zoYqhBJY9DDHtk3FTFhZYpCp9hWRLX+F/zm7bYKUgwzgSPV6S2MAzg=="; // AccessKey string obtained from Picovoice Console (https://picovoice.ai/console/)

static int8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));

static const float PORCUPINE_SENSITIVITY = 0.5f;    //Sensitive for wake word detection
static const float RHINO_SENSITIVITY = 0.5f;        //Sensitive for intent detection
static const float RHINO_ENDPOINT_DURATION_SEC = 1.0f;  //Delay between speak and intent processing
static const bool RHINO_REQUIRE_ENDPOINT = true;

char color[10];     //String to store the color we want the color to change to
pv_inference_t *inference; //Pointer to the inference proccesed object
bool isUnderstood; // Boolean to confirm that the inference is understood

/* Enums */
enum
{
    WAIT_VOICE = 0,
};

static void error_handler(void)
{
    while (true)
        ;
}

/* State machine input or transition functions */

/**
 * @brief Checks if the buffer has an audio frame.
 * 
 * @param p_this   Pointer to an fsm_t struct that contains an fsm_voice_t
 * @return true 
 * @return false 
 */
static bool check_buffer(fsm_t *p_this)
{
    fsm_voice_t *p_fsm = (fsm_voice_t *)p_this;
    int16_t *buffer = pv_audio_rec_get_new_buffer();
    p_fsm->buffer = buffer;

    return buffer;
}

/**
 * @brief Checks if the picovoice intent was understood.
 * 
 * This function is necessary since to process the intent we need to acces to the transmitter, hence we need the fsm_t.
 * 
 * The picovoice object calls the inference_callback, which gives us the inference object. From there is 
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_understood(fsm_t *p_this)
{
    return isUnderstood;
}

/* State machine output or action functions */
static void do_process(fsm_t *p_this)
{
    fsm_voice_t *p_fsm = (fsm_voice_t *)p_this;

    pv_picovoice_t *handle = p_fsm->handle;
    int16_t *buffer = p_fsm->buffer;

    pv_status_t status = pv_picovoice_process(handle, buffer);
    if (status != PV_STATUS_SUCCESS)
    {
        printf("Picovoice process failed with '%s'", pv_status_to_string(status));
        error_handler();
    }
}

static void do_change_color(fsm_t *p_this)
{
    isUnderstood = false;
    fsm_voice_t *p_fsm = (fsm_voice_t *)p_this;

    if (strcmp(color, "rojo") == 0)
    {
        fsm_tx_set_code(p_fsm->p_fsm_tx, MY_RED_BUTTON);
    }
    else if (strcmp(color, "verde") == 0)
    {
        fsm_tx_set_code(p_fsm->p_fsm_tx, MY_GREEN_BUTTON);
    }
    else if (strcmp(color, "azul") == 0)
    {
        fsm_tx_set_code(p_fsm->p_fsm_tx, MY_BLUE_BUTTON);
    }

}

/* Picovoice Callbacks */
/**
 * @brief Callback function that is called when the wake word is detected
 */
static void wake_word_callback(void)
{
    BSP_LED_On(LED4);
}

static void inference_callback(pv_inference_t *inference)
{
    BSP_LED_Off(LED4);
    if (inference->is_understood && strcmp(inference->intent, "changeColor") == 0)
    {
        isUnderstood = true;
        sprintf(color,"%s", inference->values[0]);
    }
    else
    {
        isUnderstood = false;
    }

    pv_inference_delete(inference);
}

/* Transition table */
static fsm_trans_t fsm_trans_voice[] = {
    {WAIT_VOICE, check_understood, WAIT_VOICE, do_change_color},
    {WAIT_VOICE, check_buffer, WAIT_VOICE, do_process},
    {-1, NULL, -1, NULL},
};

/* Other auxiliary functions */

void picovoice_object_init(fsm_t *p_this)
{
    fsm_voice_t *p_fsm = (fsm_voice_t *)p_this;

    pv_status_t status = pv_audio_rec_init();
    if (status != PV_STATUS_SUCCESS)
    {
        printf("Audio init failed with '%s'", pv_status_to_string(status));
        error_handler();
    }

    status = pv_audio_rec_start();
    if (status != PV_STATUS_SUCCESS)
    {
        printf("Recording audio failed with '%s'", pv_status_to_string(status));
        error_handler();
    }

    pv_picovoice_t *handle = NULL;

    status = pv_picovoice_init(
        ACCESS_KEY,
        MEMORY_BUFFER_SIZE,
        memory_buffer,
        sizeof(KEYWORD_ARRAY),
        KEYWORD_ARRAY,
        PORCUPINE_SENSITIVITY,
        wake_word_callback,
        sizeof(CONTEXT_ARRAY),
        CONTEXT_ARRAY,
        RHINO_SENSITIVITY,
        RHINO_ENDPOINT_DURATION_SEC,
        RHINO_REQUIRE_ENDPOINT,
        inference_callback,
        &handle);
    if (status != PV_STATUS_SUCCESS)
    {
        printf("Picovoice init failed with '%s'", pv_status_to_string(status));
        error_handler();
    }
    p_fsm->handle = handle;
}

void fsm_voice_init(fsm_t *p_this, fsm_t *p_fsm_tx)
{
    fsm_voice_t *p_fsm = (fsm_voice_t *)p_this;

    fsm_init(p_this, fsm_trans_voice);

    picovoice_object_init(p_this);
    p_fsm->p_fsm_tx = p_fsm_tx;
}

/**
 * @brief Creates the FSM for controlling the voice commands
 *
 * @return Pointer to an fsm_t struct that contains an fsm_voice_t
 */
fsm_t *fsm_voice_new(fsm_t *p_fsm_tx)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_voice_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_voice_init(p_fsm, p_fsm_tx);
    return p_fsm;
}

#endif