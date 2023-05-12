/*
    Copyright 2020-2021 Picovoice Inc.

    You may not use this file except in compliance with the license. A copy of the license is located in the "LICENSE"
    file accompanying this source.

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
    an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
    specific language governing permissions and limitations under the License.
*/
/**
 * @file pv_audio_rec.h
 * @brief Header for pv_audio_rec.c file.
 */

#ifndef PV_AUDIO_REC_H
#define PV_AUDIO_REC_H

#include "picovoice.h"

/**
 * @brief Init the system to rec the audio
 * 
 * @return pv_status_t 
 */
pv_status_t pv_audio_rec_init(void);

/**
 * @brief Create a new recorder for capturing the voice
 * 
 * @return pv_status_t 
 */
pv_status_t pv_audio_rec_start(void);

/**
 * @brief Get a new buffer with the audio information
 * 
 * @return int16_t* 
 */
int16_t *pv_audio_rec_get_new_buffer(void);

/**
 * @brief Deinit of the audio recorder
 * 
 */
void pv_audio_rec_deinit(void);

#endif // PV_AUDIO_REC_H
