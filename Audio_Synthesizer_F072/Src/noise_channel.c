/**
 *******************************************************************************
 * @file    noise_channel.c
 * @brief   DAC-based Noise Channel Control Interface Source File
 *******************************************************************************
 **/

 #include "noise_channel.h"
 #include "noise_base.h"   // Defines: const unsigned char noise_base[16384]
 #include "stm32f0xx_hal.h"
 
 /* Private Variables ---------------------------------------------------------*/
 /** Noise channel state structure */
 typedef struct {
     uint32_t count;   /**< Sample index (0 ~ 16383) */
     uint16_t freq;    /**< Increment value for sample index (controls output frequency) */
     uint8_t vol;      /**< Volume (0 ~ 127) */
     uint8_t on_off;   /**< Channel on/off (1: on, 0: off) */
 } noise_channel_state_t;
 
 static noise_channel_state_t noise_state;
 
 /* External Variables --------------------------------------------------------*/
 /** DAC handle (global variable defined in main.c) */
 extern DAC_HandleTypeDef hdac1;
 
 /* Exported Functions --------------------------------------------------------*/
 
 /**
   * @brief  Initialize the noise channel state.
   * @retval None
   */
 void noise_channel_init(void)
 {
     noise_state.count = 0;
     noise_state.freq  = 0;
     noise_state.vol   = 127;  // Default maximum volume
     noise_state.on_off = 0;
 }
 
 /**
   * @brief  Enable the noise channel output by starting the DAC.
   * @retval None
   */
 void noise_channel_enable(void)
 {
     if (HAL_DAC_Start(&hdac1, DAC_CHANNEL_1) != HAL_OK)
     {
         // Error handling (add code as needed)
     }
 }
 
 /**
   * @brief  Disable the noise channel output by stopping the DAC.
   * @retval None
   */
 void noise_channel_disable(void)
 {
     HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
 }
 
 /**
   * @brief  Turn the noise channel on or off.
   * @param  state: 1 to turn on, 0 to turn off.
   * @retval None
   */
 void noise_channel_on_off(uint8_t state)
 {
     if (state)
     {
         noise_state.on_off = 1;
         noise_state.count  = 0;  // Reset sample index when turning on
     }
     else
     {
         noise_state.on_off = 0;
     }
 }
 
 /**
   * @brief  Set the noise channel frequency.
   * @param  freq: Increment value for the sample index (controls output frequency).
   * @retval None
   */
 void noise_channel_frequency(uint16_t freq)
 {
     noise_state.freq = freq;
 }
 
 /**
   * @brief  Set the noise channel volume.
   * @param  volume: Volume value (0 ~ 127).
   * @retval None
   */
 void noise_channel_volume(uint8_t volume)
 {
     noise_state.vol = volume;
 }
 
 /**
   * @brief  Update the noise channel output and update the DAC value.
   *         This function should be called periodically.
   * @retval None
   */
 void noise_channel_update(void)
 {
     if (!noise_state.on_off)
     {
         // If channel is off, output a mid-scale value (e.g., 2048 for a 12-bit DAC)
         HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
         return;
     }
 
     // Increment sample index and wrap around (noise_base size is 16384)
     noise_state.count += noise_state.freq;
     noise_state.count %= 16384;
 
     // Retrieve 8-bit sample (0 ~ 255) from the noise_base array
     uint8_t sample = noise_base[noise_state.count];
 
     // Adjust volume: apply linear scaling (full scale when volume is 127)
     uint32_t adjusted = ((uint32_t)sample * noise_state.vol) / 127;
 
     uint32_t dac_val = (adjusted* 4095) / 255;
     HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_val);
 }
 
 /**
   * @brief  Initialize the GPIO pin used by the DAC.
   * @retval None
   */
 void init_gpio_for_dac(void)
 {
     __HAL_RCC_GPIOA_CLK_ENABLE();
     
     GPIO_InitTypeDef GPIO_InitStruct = {0};
     GPIO_InitStruct.Pin  = GPIO_PIN_4;
     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 }
 
 /**
   * @brief  Initialize the DAC.
   * @retval None
   */
 void init_dac(void)
 {
     __HAL_RCC_DAC1_CLK_ENABLE();
     
     hdac1.Instance = DAC1;
     HAL_DAC_Init(&hdac1);
     
     DAC_ChannelConfTypeDef sConfig = {0};
     sConfig.DAC_Trigger      = DAC_TRIGGER_NONE;
     sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
     
     HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1);
 }
 