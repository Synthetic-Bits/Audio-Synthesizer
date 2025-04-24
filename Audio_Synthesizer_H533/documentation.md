
## Src

- `channel_driver.c` – Controls STM32 timers and DAC to generate seven PWM waveform channels plus a noise channel, and provides functions to enable/disable channels, select waveforms, and manage volume and per-voice parameters.  

- `main.c` – Contains the main application entry point for the Audio-Synthesizer.

- `midi.c` – Parses incoming MIDI bytes, decodes MIDI events (note on/off, control changes, pitch bend), and drives the audio channels via the channel driver API.  

- `rcc.c` – Offers simple functions to enable RCC clocks for GPIOA–C, TIM2/3/4, DAC1, and USART1/3 on the STM32H5.  

- `sample_timer.c` – Configures TIM2 as the sample-rate timer and invokes a user-registered callback on each tick to update audio output.  

- `stm32h5xx_hal_msp.c` – Implements the HAL MSP init/de-init hooks, setting up system-level clocks, interrupt priorities, and debug interfaces.  

- `stm32h5xx_it.c` – Defines default ISR handlers for Cortex faults, SysTick, and peripheral interrupts, calling HAL or BSP callbacks as needed.

- `syscalls.c` – Provides minimal Newlib syscall stubs (_read, _write, _open) for bare-metal STM32, routing standard I/O through weak `__io_putchar`/`__io_getchar` hooks.  

- `sysmem.c` – Implements the `_sbrk()` syscall to grow the heap for `malloc` and friends, using linker symbols to prevent heap/stack collision. 

- `system_setup.c` – Implements the Error_Handler loop, configures the system clock (CSI + PLL, bus dividers, flash delay), initializes the instruction cache, and provides a full-assert failure hook.  

- `system_stm32h5xx.c` – Sets up the Error_Handler, system clock (PLL/CSI) and bus dividers, and initializes the instruction cache.  

- `uart.c` – Configures USART1 for user console I/O and USART3 for MIDI, implements interrupt-driven receive plus blocking send/receive routines, and offers a `printu()` helper for easy UART printing.


## Samples

- `attack_base.h` - Defines a 1024-entry short lookup table for the attack curve of the ADSR envelope for the synthesizer.

- `decay_base.h` - Defines a 1024-entry short lookup table for the decay portion of the ADSR envelope for the synthesizer.

- `modulation_base.h` - Defines a 16384-entry short lookup table for the modulation of the ADSR envelope for the synthesizer.

- `ramp_base.h` - Defines a 1024-entry unsigned short lookup table (generated from MATLAB) for the ramp waveform.

- `sine_base.h` - Defines a 1024-entry unsigned short lookup table (generated from MATLAB) for the sine waveform.

- `square_base.h` - Defines a 1024-entry unsigned short lookup table (generated from MATLAB) for the square waveform.

- `trig_base.h` - Defines a 1024-entry unsigned short lookup table (generated from MATLAB) for the trig waveform.



