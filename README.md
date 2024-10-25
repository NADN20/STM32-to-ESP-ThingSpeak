# STM32-to-ESP-ThingSpeak
This project uses an STM32 microcontroller to read data from its internal temperature sensor, which is then transmitted to an ESP8266 module via serial communication. The ESP8266 forwards the data to ThingSpeak for visualization and analysis. 



                                        ┌──────────────────────────────┐
                                        │       System Startup         │
                                        │ ─ HAL_Init()                 │
                                        │ ─ SystemClock_Config()       │
                                        └──────────────┬───────────────┘
                                                       │
                                                       ▼
                                        ┌──────────────────────────────┐
                                        │ Peripheral Initialization    │
                                        │ ─ Initialize GPIO, DMA,      │
                                        │   ADC, UART, TIM             │
                                        └──────────────┬───────────────┘
                                                       │
                                                       ▼
                                        ┌──────────────────────────────┐
                                        │ Start TIM6 in Interrupt Mode │
                                        │ ─ HAL_TIM_Base_Start_IT()    │
                                        └──────────────┬───────────────┘
                                                       │
                                                       ▼
                                             ┌───────────────────────┐
                                             │    Main While Loop    │
                                             │       (while(1))      │
                                             └───────────────────────┘

                                      ┌───────────────┬───────────────┬───────────────┐
                                      │               │               │               │
                                      ▼               ▼               ▼               ▼
                      ┌──────────────────────┐   ┌────────────────────┐   ┌────────────────────────┐
                      │ ADC Conversion       │   │ UART DMA Complete  │   │ TIM Period Elapsed     │
                      │ Complete Callback    │   │ Callback           │   │ Callback               │
                      │ ─ Read ADC value     │   │ ─ Set isSent flag  │   │ ─ Trigger ADC Start    │
                      │ ─ Calculate temp     │   │   to 1 (Ready)     │   │   with Interrupt       │
                      │ ─ Send via UART DMA  │   │                    │   └────────────────────────┘
                      └──────────────────────┘   └────────────────────┘

                      ┌────────────────────────────────────────────────────────────────────────────┐
                      │ External Interrupt on GPIO (Button Press)                                  │
                      │ ─ Start ADC Conversion with Interrupt (HAL_ADC_Start_IT)                   │
                      └────────────────────────────────────────────────────────────────────────────┘
                                        │
                                        ▼
                                  ┌─────────────┐
                                  │ Error       │
                                  │ Handler     │
                                  └─────────────┘

Detailed Explanation of Each Section
System Initialization:

HAL_Init(): Initializes the Hardware Abstraction Layer, setting up the system’s basic settings.
SystemClock_Config(): Configures the system clock to ensure proper timing and frequency settings for peripheral operation.
Peripheral Initialization:

Initializes various peripherals, including GPIO, DMA, ADC, UART, and TIM6.
DMA: Configures Direct Memory Access to enable data transfer from ADC to memory and from memory to UART without CPU intervention.
ADC: Configures the ADC for analog-to-digital conversion, which will read the sensor data (temperature).
UART: Sets up UART for serial communication, particularly in DMA mode.
TIM6: Configures Timer 6, which will trigger periodic ADC conversions via interrupts.
Starting TIM6 in Interrupt Mode:

HAL_TIM_Base_Start_IT(&htim6): Enables TIM6 interrupt, which generates periodic interrupts to trigger the ADC conversion.
Main While Loop:

This loop is essentially idle as the main code logic is interrupt-driven. The system remains in this loop, waiting for interrupts.
Interrupt and Callback Functions:

ADC Conversion Complete Callback (HAL_ADC_ConvCpltCallback):

Triggered when ADC completes a conversion.
Reads the converted ADC value using HAL_ADC_GetValue.
Converts the ADC value to temperature:

Temperature (°C) = ((ADC_Value * Vref) / ADC_Resolution) * Scaling_Factor + Offset

Sends the temperature value via UART using DMA.
Toggles an LED on GPIOB_PIN_6 for visual feedback.
UART Transmission Complete Callback (HAL_UART_TxCpltCallback):

Triggered when UART transmission completes.
Sets the isSent flag to 1, indicating that UART is ready for another transmission.
Timer Period Elapsed Callback (HAL_TIM_PeriodElapsedCallback):

Triggered by TIM6 at regular intervals.
Starts an ADC conversion by calling HAL_ADC_Start_IT(&hadc), initiating another cycle of data acquisition.
GPIO External Interrupt Callback (HAL_GPIO_EXTI_Callback):

Triggered by an external interrupt from a button press on GPIO pin.
Starts ADC conversion using HAL_ADC_Start_IT(&hadc), enabling manual triggering.
Error Handling:

Error_Handler(): Captures any errors within the code. Disables interrupts to prevent further code execution until the error is resolved, helping identify any issues during debugging.


@par Application description
This application show how to send data to a server using the STM32F4 MCU and a WiFi module ESP8266-E01.

@par Hardware and Software environment 
Below the different steps of the process:

  -1- connect the module to the STM32F4-Discovery board:
      
              _____________________              _____________________
             |                     |            |                     |
             |                  Tx - ----- - Rx-PA10                  | 
             |      ESP8266        |                   STM32l1        |
             |                  Rx - ----- - Tx-PA9                   |
             |        E01          |                    Disco         |
             |                  GND- ----------- GND                  |
             |                     |            |                     |
             |_____________________|            |_____________________|


  -2- Set the STM32F4 USART as following:
       - Baude Rate = 9600
       - Parity     = none
       - Stop bit   = 1
       - wordLength = 8 Bit
       

  -3- All the ESP8266 commands are set in the RX_ESP_TS.ino file

                                  
