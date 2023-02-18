////////////////////////////////////////////////////////////////////
// Author:          Max McEvoy
// Title:           pwm_test.c
// MCU:             AVR128DB28
// Date Created:    February 2, 2023
// Description:     This program plays around with the s9g servo motor
//                  It uses a 50 period with a a variable duty based on input
//                  potentiometer is 50k with something resistor PD1
// Rev. History:    One and done
////////////////////////////////////////////////////////////////////

#include <avr/io.h>

int main(void) {
    // Enable global interrupts.
    
    // Set internal clock frequency to 16 MHz.
    CCP = 0xd8;
    CLKCTRL.OSCHFCTRLA = 0b00010100;
    while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
        ;
    }
    
    SREG = 0b10000000;
    
    // Set the ADC reference level to VDD.
    VREF.ADC0REF = 0b10000101; //21.5.1
    
    // Enable the ADC interrupt.
    ADC0.INTCTRL = 0b00000001; //33.5.11
    //Window Comparator interrupt disabled
    // Result Ready Interrupt enabled
    
    // Select PD2 (AIN2) as the ADC input.
    ADC0.MUXPOS = 0x02; //33.5.7
    ADC0.MUXNEG = 0x40;
    // Select minimum clock divide.
    ADC0.CTRLC = 0x00; //33.5.3
    
    // Select single ended mode, 12 bit resolution and free-running modes.
    ADC0.CTRLA = 0b00000011; //32.5.1
    
    ADC0.COMMAND = 0x01; //33.5.9
    //start conversation
    
    TCA0.SINGLE.CTRLA = 0b00001011;//pre scale 64
    TCA0.SINGLE.INTCTRL = 0b00010001;
    TCA0.SINGLE.PER = 2500; //Trial and Error 50hz
    //16x10^6 / 2 * pre scale * PER = frequency
    TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .88; //97 max 88
    PORTC.DIRSET = 0b00000011;
    int state = -1;
    while (1) {
        int a = TCA0.SINGLE.INTFLAGS;
        if(a & 1){
            PORTC.OUT &= 0b11111110;
            TCA0.SINGLE.INTFLAGS |= 0b00000001;
        }
        if(a & 0b00010000){
            PORTC.OUT |= 0b0000001;
            TCA0.SINGLE.INTFLAGS |= 0b00010000;
        }
        if(ADC0.INTFLAGS == 1){
            if(ADC0.RES > 2730){
                if( state != 0) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .88;
                state = 0;
            }
            else if(ADC0.RES > 2457){
                if( state != 1) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .89;
                state = 1;
            }
            else if (ADC0.RES > 2184){
                if( state != 2) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .90;
                state = 2;
            }
            else if (ADC0.RES > 1911){
                if( state != 3) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .91;
                state = 3;
            }
            else if(ADC0.RES > 1638){
                if( state != 4) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .92;
                state = 4;
            }
            else if (ADC0.RES > 1365){
                if( state != 5) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .93;
                state = 5;
            }
            else if (ADC0.RES > 1092){
                if( state != 6) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .94;
                state = 6;
            }
            else if(ADC0.RES > 819){
                if( state != 7) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .95;
                state = 7;
            }
            else if (ADC0.RES > 546){
                if( state != 8) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .96;
                state = 8;
            }
            else if (ADC0.RES < 0273){
                if( state != 9) TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER * .97;
                state = 9;
            }
            ADC0.INTFLAGS = 1;
        }
        
        
    }
    
    
}
