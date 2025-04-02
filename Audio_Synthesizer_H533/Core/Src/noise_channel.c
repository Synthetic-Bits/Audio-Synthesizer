/**
 *******************************************************************************
 * @file    noise_channel.c
 * @brief   DAC-based Noise Channel Control Interface Source File
 * @attention
 *
 * This file implements the noise channel functionality using a DAC.
 * The noise waveform data is taken from noise_base.h.
 *
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "noise_channel.h"
#include "audio_config.h"
#include "channel_common.h"
#include "config.h"
#include "rcc.h"

#include "stm32h5xx_hal.h"
#include "noise_base.h"  // 노이즈 샘플 데이터 포함

/* Private Variables ---------------------------------------------------------*/

/* Noise channel state structure */
typedef struct {
    uint32_t count;       // 샘플 인덱스
    uint16_t freq;        // 인덱스 증가값 (출력 주파수 제어)
    uint8_t vol;          // 볼륨 (0~127)
    uint8_t on_off;       // 채널 온/오프 상태 (1: on, 0: off)
    waveforms_t waveform; // 현재 사용중인 웨이브폼 (여기서는 WAVEFORM_NOISE)
} noise_channel_state_t;

static noise_channel_state_t noise_state;

/* DAC handle (CubeMX나 다른 초기화 코드에서 설정된 DAC 핸들) */
extern DAC_HandleTypeDef hdac1;  // 예: DAC1 사용

/* Private Function Prototypes ---------------------------------------------*/
static void reset_noise_channel(void);

/* Exported Functions --------------------------------------------------------*/

void noise_channel_enable(void)
{
  // DAC 채널 (예: DAC_CHANNEL_1)에서 DAC 변환을 시작
  if (HAL_DAC_Start(&hdac1, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

void noise_channel_disable(void)
{
  // DAC 변환을 중지
  if (HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

void noise_channel_set_waveform(waveforms_t wave)
{
  if (wave != WAVEFORM_NOISE)
    return;  // 노이즈 채널은 WAVEFORM_NOISE만 지원
  
  noise_state.waveform = wave;
  // noise_base 배열을 사용하므로 별도 할당은 필요없음.
}

void noise_channel_on_off(uint8_t state)
{
  if (state)
  {
    noise_state.on_off = 1;
    noise_state.count = 0;  // 새 톤 시작 시 카운터 리셋
  }
  else
  {
    noise_state.on_off = 0;
  }
}

void noise_channel_volume(uint8_t volume)
{
  noise_state.vol = volume;
}

void noise_channel_frequency(uint16_t freq)
{
  noise_state.freq = freq;
}

void noise_channel_update(void)
{
  if (!noise_state.on_off)
  {
    // 채널이 off 상태이면 DAC 출력을 50% 중간값(예: 12비트 DAC의 경우 2048)으로 설정
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
    return;
  }

  // 샘플 카운터 증가
  noise_state.count += noise_state.freq;
  while (noise_state.count >= SAMPLE_FREQUENCY)  // SAMPLE_FREQUENCY는 audio_config.h에서 정의됨
  {
    noise_state.count -= SAMPLE_FREQUENCY;
  }

  // noise_base 배열에서 샘플 추출 (noise_base 배열 크기는 65536 샘플)
  uint16_t sample = noise_base[noise_state.count];

  // 볼륨에 따른 값 조정 (볼륨이 낮으면 신호 크기를 줄임)
  uint8_t shift = ((MIDI_MAX_VAL - noise_state.vol) >> 4);
  sample = sample >> shift;

  // DAC 출력 업데이트 (예: 12비트 DAC)
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sample);
}

void noise_channel_init(void)
{
  // 노이즈 채널 상태 초기화
  reset_noise_channel();

  // DAC 관련 초기화는 보통 CubeMX에서 설정하므로 별도 GPIO나 클럭 설정은 생략
  // 필요한 경우, HAL_DAC_Init() 등을 호출할 수 있음.
}

/* Private Functions ---------------------------------------------------------*/

static void reset_noise_channel(void)
{
  noise_state.count = 0;
  noise_state.on_off = 0;
  noise_state.freq = 0;
  noise_state.vol = MIDI_MAX_VAL;  // 최대 볼륨
  noise_state.waveform = WAVEFORM_NOISE;
}
