/*
    Copyright 2020-2021 Picovoice Inc.

    You may not use this file except in compliance with the license. A copy of the license is located in the "LICENSE"
    file accompanying this source.

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
    an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
    specific language governing permissions and limitations under the License.
*/
/**
 * @file pv_audio_rec.c
 * @brief Portable functions to record the voice.
*/

#include <stdbool.h>
#include <stdlib.h>

#include "stm32f411e_discovery_audio.h"

#include "picovoice.h"

#define PV_AUDIO_REC_AUDIO_FREQUENCY (16000U)   /*!< Frequency of audio sampling needed*/
#define PV_AUDIO_REC_CHANNEL_NUMBER (1)         /*!< Number of channels for audio recording*/
#define PV_AUDIO_REC_RECORD_BUFFER_SIZE (512)   /*!< Size of audio recording buffer*/
#define PV_AUDIO_REC_VOLUME_LEVEL (100)         /*!< Volume of the audio recorded*/

#define AUDIO_IN_PCM_BUFFER_SIZE    ((uint32_t)(PV_AUDIO_REC_AUDIO_FREQUENCY / 1000 * PV_AUDIO_REC_CHANNEL_NUMBER)) /*!< Size of the buffer used to store the PDM-PCM conversion*/
#define AUDIO_IN_PDM_BUFFER_SIZE    ((uint32_t)(128 * PV_AUDIO_REC_AUDIO_FREQUENCY / 16000 * PV_AUDIO_REC_CHANNEL_NUMBER)) /*!< Size of the buffer used to store the PDM-PCM conversion*/
static uint16_t record_pdm_buffer[AUDIO_IN_PDM_BUFFER_SIZE];/*!< Declaration of the pdm buffer*/
static uint16_t record_pcm_buffer[2 * AUDIO_IN_PCM_BUFFER_SIZE];/*!< Declaration of the pcm buffer. The values are stored on even indexes since are 32bits values*/
static int16_t ping_pong_buffer[2][PV_AUDIO_REC_RECORD_BUFFER_SIZE];/*!< Set of buffers used to be able to record while processing the data*/

static int32_t last_read_index = -1;    /*!< Index of the last readed buffer*/
static int32_t read_index = 1;          /*!< Index of the actual readable buffer*/
static int32_t write_index = 0;         /*!< Index of the writable buffer*/
static int32_t buffer_index = 0;        /*!< Index of the buffer used to write it*/

/**
 * @brief Struct with the information of the audio_recording process
 * 
 */
struct {
    uint32_t channel_number;        /*!< Number of channels that are being recorded*/
    uint32_t audio_frequency;       /*!< Audio frequecy used*/
    uint32_t record_buffer_size;    /*!< Size of the record buffer*/
    uint16_t *record_pcm_buffer;    /*!< Direction of the buffer to store the data conversion*/
    bool is_recording;              /*!< State of the recorder*/
} pv_audio_rec;

/**
 * @brief Initialize the audio recording 
 * 
 * This initialization is made by the HAL by setting the I2S with the configuration needed
 * 
 * @return pv_status_t 
 */
pv_status_t pv_audio_rec_init(void) {
    pv_audio_rec.channel_number = PV_AUDIO_REC_CHANNEL_NUMBER;
    pv_audio_rec.audio_frequency = PV_AUDIO_REC_AUDIO_FREQUENCY;
    pv_audio_rec.record_buffer_size = PV_AUDIO_REC_RECORD_BUFFER_SIZE;
    pv_audio_rec.record_pcm_buffer = record_pcm_buffer;

    if (BSP_AUDIO_IN_Init(PV_AUDIO_REC_AUDIO_FREQUENCY, DEFAULT_AUDIO_IN_BIT_RESOLUTION, PV_AUDIO_REC_CHANNEL_NUMBER) != AUDIO_OK) {
        return PV_STATUS_INVALID_STATE;
    }
    if (BSP_AUDIO_IN_SetVolume(PV_AUDIO_REC_VOLUME_LEVEL) != AUDIO_OK) {
        return PV_STATUS_INVALID_STATE;
    }
    return PV_STATUS_SUCCESS;
}

/**
 * @brief Starts the audio recording
 * 
 * @return pv_status_t 
 */
pv_status_t pv_audio_rec_start(void) {
    if (BSP_AUDIO_IN_Record((uint16_t*)(record_pdm_buffer), AUDIO_IN_PDM_BUFFER_SIZE) != AUDIO_OK) {
        return PV_STATUS_INVALID_STATE;
    }
    pv_audio_rec.is_recording = true;
    return PV_STATUS_SUCCESS;
}

/**
 * @brief Return the ping-pong buffer that is ready to process
 * 
 * @return int16_t* Buffer with the audio sampling
 */
int16_t *pv_audio_rec_get_new_buffer(void) {
    if (read_index == -1) {
        return NULL;
    }

    if (last_read_index == read_index) {
        return NULL;
    }

    last_read_index = read_index;

    return ping_pong_buffer[read_index];
}

/**
 * @brief Function called when the recording buffer is completed.
 * 
 * Parses the PDM samples to PCM and writes them into the ping-pong buffer. When that buffer is full, it is set to read and the other is set to write.
 * 
 */
void BSP_AUDIO_IN_TransferComplete_CallBack(void) {
    BSP_AUDIO_IN_PDMToPCM((uint16_t*) &record_pdm_buffer[AUDIO_IN_PDM_BUFFER_SIZE / 2],
            record_pcm_buffer);
    for (uint32_t i = 0; i < AUDIO_IN_PCM_BUFFER_SIZE; i++) {
        ping_pong_buffer[write_index][buffer_index++] = record_pcm_buffer[i * 2];
    }
    if (buffer_index >= PV_AUDIO_REC_RECORD_BUFFER_SIZE) {
        read_index = write_index;
        write_index = 1 - write_index;
        buffer_index = 0;
    }
}

/**
 * @brief Function called when the recording buffer is half completed.
 * 
 * Parses the PDM samples to PCM and writes them into the ping-pong buffer. When that buffer is full, it is set to read and the other is set to write.
 * 
 */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void) {
    BSP_AUDIO_IN_PDMToPCM((uint16_t*) &record_pdm_buffer[0],
            record_pcm_buffer);
    for (uint32_t i = 0; i < AUDIO_IN_PCM_BUFFER_SIZE; i++) {
        ping_pong_buffer[write_index][buffer_index++] = record_pcm_buffer[i * 2];
    }
    if (buffer_index >= PV_AUDIO_REC_RECORD_BUFFER_SIZE) {
        read_index = write_index;
        write_index = 1 - write_index;
        buffer_index = 0;
    }
}

/**
 * @brief Deinit the audio recorder
 * 
 */
void pv_audio_rec_deinit(void) {
    BSP_AUDIO_IN_Stop();
}
