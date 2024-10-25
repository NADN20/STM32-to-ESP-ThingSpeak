# STM32-to-ESP-ThingSpeak
This project uses an STM32L152RBT6 microcontroller to read data from its internal temperature sensor, which is then transmitted to an ESP8266 module via serial communication. The ESP8266 forwards the data to ThingSpeak for visualization and analysis. 

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
                                                  │   Error     │
                                                  │   Handler   │
                                                  └─────────────┘

Simplified Summary
System Initialization:

HAL_Init(): Prepares basic parameters.
SystemClock_Config(): Configures the clock for proper peripheral operation.
Peripheral Initialization:

Configures DMA, ADC, UART, and Timer 6 for specific operations.
Starting TIM6:

Enables TIM6 interrupt for regular ADC conversions.
Main Loop:

Remains idle, waiting for interrupts to execute actions.
Interrupt Callbacks:

Manage the completion of ADC conversions, UART transmissions, and button presses, allowing for operations like data sending.
Error Handling:

Error_Handler(): Manages errors and halts execution for debugging.

@par Application description
This application show how to send data to a server using the STM32l1 MCU and a WiFi module ESP8266-E01.

@par Hardware and Software environment 
Below the different steps of the process:

  -1- connect the ESP to the STM32L1-Discovery board:
      
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

                                  
