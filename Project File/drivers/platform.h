#ifndef PLATFORM_H
#define PLATFORM_H

#include "LPC407x_8x_177x_8x.h"

// Core peripheral frequency.
#define CLK_FREQ  120000000UL

typedef enum {

 P0_0 = (0 << 16) | 0,
 P0_1 = (0 << 16) | 1,
 P0_2 = (0 << 16) | 2,
 P0_3 = (0 << 16) | 3,
 P0_4 = (0 << 16) | 4,
 P0_5 = (0 << 16) | 5,
 P0_6 = (0 << 16) | 6,
 P0_7 = (0 << 16) | 7,
 P0_8 = (0 << 16) | 8,
 P0_9 = (0 << 16) | 9,
 P0_10= (0 << 16) | 10,
 P0_11= (0 << 16) | 11,
 P0_12= (0 << 16) | 12,
 P0_13= (0 << 16) | 13,
 P0_14= (0 << 16) | 14,
 P0_15= (0 << 16) | 15,
 P0_16= (0 << 16) | 16,
 P0_17= (0 << 16) | 17,
 P0_18= (0 << 16) | 18,
 P0_19= (0 << 16) | 19,
 P0_20= (0 << 16) | 20,
 P0_21= (0 << 16) | 21,
 P0_22= (0 << 16) | 22,
 P0_23= (0 << 16) | 23,
 P0_24= (0 << 16) | 24,
 P0_25= (0 << 16) | 25,
 P0_26= (0 << 16) | 26,
 P0_27= (0 << 16) | 27,
 P0_28= (0 << 16) | 28,
 P0_29= (0 << 16) | 29,
 P0_30= (0 << 16) | 30,
 P0_31= (0 << 16) | 31,

 
 P1_0 = (1 << 16) | 0,
 P1_1 = (1 << 16) | 1,
 P1_2 = (1 << 16) | 2,
 P1_3 = (1 << 16) | 3,
 P1_4 = (1 << 16) | 4,
 P1_5 = (1 << 16) | 5,
 P1_6 = (1 << 16) | 6,
 P1_7 = (1 << 16) | 7,
 P1_8 = (1 << 16) | 8,
 P1_9 = (1 << 16) | 9,
 P1_10= (1 << 16) | 10,
 P1_11= (1 << 16) | 11,
 P1_12= (1 << 16) | 12,
 P1_13= (1 << 16) | 13,
 P1_14= (1 << 16) | 14,
 P1_15= (1 << 16) | 15,
 P1_16= (1 << 16) | 16,
 P1_17= (1 << 16) | 17,
 P1_18= (1 << 16) | 18,
 P1_19= (1 << 16) | 19,
 P1_20= (1 << 16) | 20,
 P1_21= (1 << 16) | 21,
 P1_22= (1 << 16) | 22,
 P1_23= (1 << 16) | 23,
 P1_24= (1 << 16) | 24,
 P1_25= (1 << 16) | 25,
 P1_26= (1 << 16) | 26,
 P1_27= (1 << 16) | 27,
 P1_28= (1 << 16) | 28,
 P1_29= (1 << 16) | 29,
 P1_30= (1 << 16) | 30,
 P1_31= (1 << 16) | 31,
 
 P2_0 = (2 << 16) | 0,
 P2_1 = (2 << 16) | 1,
 P2_2 = (2 << 16) | 2,
 P2_3 = (2 << 16) | 3,
 P2_4 = (2 << 16) | 4,
 P2_5 = (2 << 16) | 5,
 P2_6 = (2 << 16) | 6,
 P2_7 = (2 << 16) | 7,
 P2_8 = (2 << 16) | 8,
 P2_9 = (2 << 16) | 9,
 P2_10= (2 << 16) | 10,
 P2_11= (2 << 16) | 11,
 P2_12= (2 << 16) | 12,
 P2_13= (2 << 16) | 13,
 P2_14= (2 << 16) | 14,
 P2_15= (2 << 16) | 15,
 P2_16= (2 << 16) | 16,
 P2_17= (2 << 16) | 17,
 P2_18= (2 << 16) | 18,
 P2_19= (2 << 16) | 19,
 P2_20= (2 << 16) | 20,
 P2_21= (2 << 16) | 21,
 P2_22= (2 << 16) | 22,
 P2_23= (2 << 16) | 23,
 P2_24= (2 << 16) | 24,
 P2_25= (2 << 16) | 25,
 P2_26= (2 << 16) | 26,
 P2_27= (2 << 16) | 27,
 P2_28= (2 << 16) | 28,
 P2_29= (2 << 16) | 29,
 P2_30= (2 << 16) | 30,
 P2_31= (2 << 16) | 31, 
 
 P3_0 = (3 << 16) | 0,
 P3_1 = (3 << 16) | 1,
 P3_2 = (3 << 16) | 2,
 P3_3 = (3 << 16) | 3,
 P3_4 = (3 << 16) | 4,
 P3_5 = (3 << 16) | 5,
 P3_6 = (3 << 16) | 6,
 P3_7 = (3 << 16) | 7,
 P3_8 = (3 << 16) | 8,
 P3_9 = (3 << 16) | 9,
 P3_10= (3 << 16) | 10,
 P3_11= (3 << 16) | 11,
 P3_12= (3 << 16) | 12,
 P3_13= (3 << 16) | 13,
 P3_14= (3 << 16) | 14,
 P3_15= (3 << 16) | 15,
 P3_16= (3 << 16) | 16,
 P3_17= (3 << 16) | 17,
 P3_18= (3 << 16) | 18,
 P3_19= (3 << 16) | 19,
 P3_20= (3 << 16) | 20,
 P3_21= (3 << 16) | 21,
 P3_22= (3 << 16) | 22,
 P3_23= (3 << 16) | 23,
 P3_24= (3 << 16) | 24,
 P3_25= (3 << 16) | 25,
 P3_26= (3 << 16) | 26,
 P3_27= (3 << 16) | 27,
 P3_28= (3 << 16) | 28,
 P3_29= (3 << 16) | 29,
 P3_30= (3 << 16) | 30,
 P3_31= (3 << 16) | 31,

 P4_0 = (4 << 16) | 0,
 P4_1 = (4 << 16) | 1,
 P4_2 = (4 << 16) | 2,
 P4_3 = (4 << 16) | 3,
 P4_4 = (4 << 16) | 4,
 P4_5 = (4 << 16) | 5,
 P4_6 = (4 << 16) | 6,
 P4_7 = (4 << 16) | 7,
 P4_8 = (4 << 16) | 8,
 P4_9 = (4 << 16) | 9,
 P4_10= (4 << 16) | 10,
 P4_11= (4 << 16) | 11,
 P4_12= (4 << 16) | 12,
 P4_13= (4 << 16) | 13,
 P4_14= (4 << 16) | 14,
 P4_15= (4 << 16) | 15,
 P4_16= (4 << 16) | 16,
 P4_17= (4 << 16) | 17,
 P4_18= (4 << 16) | 18,
 P4_19= (4 << 16) | 19,
 P4_20= (4 << 16) | 20,
 P4_21= (4 << 16) | 21,
 P4_22= (4 << 16) | 22,
 P4_23= (4 << 16) | 23,
 P4_24= (4 << 16) | 24,
 P4_25= (4 << 16) | 25,
 P4_26= (4 << 16) | 26,
 P4_27= (4 << 16) | 27,
 P4_28= (4 << 16) | 28,
 P4_29= (4 << 16) | 29,
 P4_30= (4 << 16) | 30,
 P4_31= (4 << 16) | 31,

 P5_0 = (5 << 16) | 0,
 P5_1 = (5 << 16) | 1,
 P5_2 = (5 << 16) | 2,
 P5_3 = (5 << 16) | 3,
 P5_4 = (5 << 16) | 4,
 P5_5 = (5 << 16) | 5,
 P5_6 = (5 << 16) | 6,
 P5_7 = (5 << 16) | 7,
 P5_8 = (5 << 16) | 8,
 P5_9 = (5 << 16) | 9,
 P5_10= (5 << 16) | 10,
 P5_11= (5 << 16) | 11,
 P5_12= (5 << 16) | 12,
 P5_13= (5 << 16) | 13,
 P5_14= (5 << 16) | 14,
 P5_15= (5 << 16) | 15,
 P5_16= (5 << 16) | 16,
 P5_17= (5 << 16) | 17,
 P5_18= (5 << 16) | 18,
 P5_19= (5 << 16) | 19,
 P5_20= (5 << 16) | 20,
 P5_21= (5 << 16) | 21,
 P5_22= (5 << 16) | 22,
 P5_23= (5 << 16) | 23,
 P5_24= (5 << 16) | 24,
 P5_25= (5 << 16) | 25,
 P5_26= (5 << 16) | 26,
 P5_27= (5 << 16) | 27,
 P5_28= (5 << 16) | 28,
 P5_29= (5 << 16) | 29,
 P5_30= (5 << 16) | 30,
 P5_31= (5 << 16) | 31,

} Pin;

//* Pin definitions *//

// Module 5: IntDemo, IntProjectReactionTime
// Module 7: AnalogLabSignalGenerator
// Module 8: TimerLabSignalGenerator
// Push-button.
#define P_SW       P2_10

// Module 5, 6, 7, 8, 9
// RGB LEDs.
#define P_LED_R    P1_11
#define P_LED_G    P1_5
#define P_LED_B    P1_7

// Module 6: GPIOProjectSlideWhistle, GPIOLabBasicUI
// Joystick control.
#define P_SW_UP    P5_2
#define P_SW_CR    P5_3
#define P_SW_DN    P5_1
#define P_SW_LT    P5_0
#define P_SW_RT    P5_4

// Module 5: IntDemo
// Debug signals.
#define P_DBG_ISR  P1_2
#define P_DBG_MAIN P1_3

// Module 6: GPIOProjectSlideWhistle
// Speaker driven with GPIO.
#define P_SPEAKER  P1_31

// Module 6: GPIOLabBasicUI
// Module 8: TimerProjectClock
// Module 9: SerialDemoUART, SerialProjectGPSSpeedometer
// LCD control.
#define P_LCD_RS       P1_24
#define P_LCD_RW       P1_23
#define P_LCD_E        P1_20
#define P_LCD_DATA     P5_0

// Module 7, AnalogProject
// IR LED.
#define P_IR           P0_23

// Module 9, UART
#define P_TX        P0_0
#define P_RX        P0_1

// Module 9, I2C
#define P_SDA        P0_27
#define P_SCL        P0_28

// Other pins (for documentation).
#define  P_ADC          P0_23
#define  P_DAC          P0_26
#define  P_CMP_PLUS     P0_9
#define  P_CMP_NEG      P0_8


/* Other useful macros */

#define GET_PORT_INDEX(pin)        ((pin) >> 16)
#define GET_PIN_INDEX(pin)         ((pin) & 0xFF)

#define RGB_HIGH 0
#define RGB_LOW  1

#define GET_GPIO_PORT(pin)         ((LPC_GPIO_TypeDef*) (LPC_GPIO0_BASE + 0x20 * GET_PORT_INDEX(pin)))     //Select GPIO port
#define GET_IOCON(pin)             ((uint32_t*) (LPC_IOCON_BASE + 0x80 * GET_PORT_INDEX(pin) + 0x04 * GET_PIN_INDEX(pin)))     //Select IOCON pin

#define ADC_BITS 12
#define ADC_MASK ((1u << ADC_BITS) - 1)
#define DAC_BITS 10
#define DAC_MASK ((1u << DAC_BITS) - 1)

// LEDs are active high or low?
#define LED_ON         0
#define LED_OFF        1

#endif

/* Pin configeration
PeripheralClock     //  120MHz/4

// Push-button              
P2_10            //SW2

// RGB LEDs                 
P1_11            //P25
P1_5             //P28
P1_7             //P26
														
// Joystick control
P5_2        //P32
P5_3        //P31
P5_1        //P38
P5_0        //P39
P5_4        //P37
		
// Debug signals.
P1_2        //P30
P1_3        //P29

// Speaker driven with GPIO
P1_31       //P20

// Module 6: GPIOLabBasicUI
// Module 8: TimerProjectClock
// Module 9: SerialDemoUART, SerialProjectGPSSpeedometer
// LCD control
RS       P1_24        //P5
RW       P1_23        //P6
E        P1_20        //P7
DATA     P5_0         //P39 - P38, P32 - P31 4bit (P5_0-P5_3)

// Module 7, AnalogProject
// IR LED
IR       P0_23        //P15(ADC0_IN[0])

// Module 9, UART
P_TX        P0_0         //UART0  P9
P_RX        P0_1         //UART0  P10

// Module 9, I2C
I2C_SDA        P0_27//I2C0  U5
I2C_SCL        P0_28//I2C0  U5

// Other pins (for documentation)
P_ADC          P0_24       //P16 (ADC0_IN[1])
P_DAC          P0_26       //P18
P_CMP_PLUS     P0_9        //P11 (CMP1_IN[2])   VP
P_CMP_NEG      P0_8        //P12 (CMP1_IN[3])   VM
										
*/
// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
