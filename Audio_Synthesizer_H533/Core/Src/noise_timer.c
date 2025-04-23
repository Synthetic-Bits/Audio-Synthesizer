// /**
// ******************************************************************************
// * @file    NOISE_TIMER.c
// * @brief   DAC1-CH2 white-noise driver using TIM7 interrupt + software LFSR
// * @author  Hayoung Im
// ******************************************************************************
// * @attention
// *
// * Copyright (c) 2025 Synthetic Bits.
// * All rights reserved.
// *
// * This software is licensed under terms that can be found in the LICENSE file
// * in the root directory of this software component.
// * If no LICENSE file comes with this software, it is provided AS-IS.
// *
// ******************************************************************************
// */

// /* Includes ------------------------------------------------------------------*/
// #include "noise_timer.h"
// #include "audio_config.h"    // MIDI_MAX_VAL
// #include "config.h"
// #include "rcc.h"             // RCC helper macros
// #include <stdint.h>
 

// /* Function Prototypes -------------------------------------------------------*/

// static uint16_t lfsr_next(void);
// void TIM7_IRQHandler(void);
// void noise_timer_enable(void);
// void noise_timer_disable(void);
// void noise_timer_on_off(uint8_t s);
// void noise_timer_volume(uint8_t v);
// void noise_timer_frequency(uint16_t f);

// static void tim7_init(uint32_t hz);
// void dac1_ch2_init(void);
// void noise_timer_init(void);


// /* ========================================================================== */
// /*                                                                            */
// /*    Local Variables Definitions                                             */
// /*                                                                            */
// /* ========================================================================== */

// #define SAMPLE_RATE_HZ   16384U   
// static uint16_t lfsr_state = 0xACE1u;    
 
// static uint16_t prev_freq = SAMPLE_RATE_HZ;  
// static uint8_t  prev_vol  = MIDI_MAX_VAL;  
// static uint8_t  prev_on   = 1;  

// volatile channel_state_t noise_state;

// /* ========================================================================== */
// /*                                                                            */
// /*    Control Functions                                                       */
// /*                                                                            */
// /* ========================================================================== */

// static uint16_t lfsr_next(void) {
//     uint16_t bit = ((lfsr_state >> 0) ^ (lfsr_state >> 1)) & 1u;
//     lfsr_state = (lfsr_state >> 1) | (bit << 11);
//     return lfsr_state;   
// }

// void TIM7_IRQHandler(void) {
//     if (TIM7->SR & TIM_SR_UIF) {
//         TIM7->SR &= ~TIM_SR_UIF;

//         if (noise_state.on_off && noise_state.vol) {
//             uint16_t raw    = lfsr_next();
//             uint32_t scaled = (uint32_t)raw * noise_state.vol / MIDI_MAX_VAL;
//             DAC1->DHR12R2   = (uint16_t)scaled;
//         }
//     }
// }
 
// void noise_timer_enable(void)          { noise_state.on_off = 1; }

// void noise_timer_disable(void)         { noise_state.on_off = 0; }

// void noise_timer_on_off(uint8_t s)     { noise_state.on_off = s; }
 
// void noise_timer_volume(uint8_t v) {
//     noise_state.vol = (v > MIDI_MAX_VAL ? MIDI_MAX_VAL : v);
// }
 
// void noise_timer_frequency(uint16_t f) {
//     if (!f) f = 1;
//     noise_state.freq = f;
//     TIM7->ARR = (1000000U / f) - 1;
//     TIM7->EGR = TIM_EGR_UG;   
// }

// void noise_timer_update(void)
// {
//     if (noise_state.freq != prev_freq) {
//         TIM7->ARR = (1000000U / noise_state.freq) - 1;
//         TIM7->EGR = TIM_EGR_UG;    // force update
//         prev_freq = noise_state.freq;
//     }

//     if ((uint8_t)noise_state.on_off != prev_on) {
//         if (noise_state.on_off) {
//             NVIC_EnableIRQ(TIM7_IRQn);
//         } else {
//             NVIC_DisableIRQ(TIM7_IRQn);
//         }
//         prev_on = (uint8_t)noise_state.on_off;
//     }

//     prev_vol = noise_state.vol;
// }

// /* ========================================================================== */
// /*                                                                            */
// /*    Initialization Functions                                                */
// /*                                                                            */
// /* ========================================================================== */

// static void tim7_init(uint32_t hz) {
//     RCC->APB1LENR |= RCC_APB1LENR_TIM7EN;
//     TIM7->PSC  = (SystemCoreClock / 1000000U) - 1;   
//     TIM7->ARR  = (1000000U / hz) - 1;             
//     TIM7->DIER |= TIM_DIER_UIE;                     
//     TIM7->CR1  |= TIM_CR1_CEN;                      
//     NVIC_EnableIRQ(TIM7_IRQn);
// }

// void dac1_ch2_init(void) {
//     RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
//     GPIOA->MODER |= (3UL << (5*2));

//     RCC->AHB2ENR |= RCC_AHB2ENR_DAC1EN;
//     DAC1->MCR    = (DAC1->MCR & ~DAC_MCR_MODE2_Msk) | DAC_MCR_MODE2_0;
//     DAC1->CR    |= DAC_CR_EN2;
// }

// void noise_timer_init(void) {
//     noise_state.freq   = SAMPLE_RATE_HZ;
//     noise_state.vol    = MIDI_MAX_VAL;
//     noise_state.on_off = 1;
//     dac1_ch2_init();
//     tim7_init(SAMPLE_RATE_HZ);
// }
 