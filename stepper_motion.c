/*
 * File:   frequency_test.c
 * Author: Max
 *
 * Created on January 30, 2023, 4:28 PM
 */

void waiting(int a);
void step(int steps);
#include <avr/io.h>

int main(void) {
    /* Replace with your application code */
    PORTC.DIRSET = 0b00001111;
    PORTC.OUT |= 0b00001111;
    
    
    
    while (1) {
        ccw_step(64);
        for (int i =0 ; i<5 ; i++){
            waiting(10000);
            waiting(10000);
            waiting(10000);
        }
waiting(10000);waiting(10000);waiting(10000);
        
    }
}

void waiting(int a){
    for(int i = 0; i<a; i++)
        SLPCTRL.CTRLA = 0b000000001;
}

void cw_step(int steps){
    int a = 500;
    for(int i = 0; i < steps; i++){
        PORTC.OUT = 0b00000111;
        waiting(a);
        PORTC.OUT = 0b00000011;
        waiting(a);
        PORTC.OUT = 0b00001011;
        waiting(a);
        PORTC.OUT = 0b00001001;
        waiting(a);
        PORTC.OUT = 0b00001101;
        waiting(a);
        PORTC.OUT = 0b00001100;
        waiting(a);
        PORTC.OUT = 0b00001110;
        waiting(a);
        PORTC.OUT = 0b00000110;
        waiting(a);
    }
}

void ccw_step(int steps){
    int a = 500;
    for(int i = 0; i < steps; i++){
        PORTC.OUT = 0b00000110;
        waiting(a);
        PORTC.OUT = 0b00001110;
        waiting(a);
        PORTC.OUT = 0b00001100;
        waiting(a);
        PORTC.OUT = 0b00001101;
        waiting(a);
        PORTC.OUT = 0b00001001;
        waiting(a);
        PORTC.OUT = 0b00001011;
        waiting(a);
        PORTC.OUT = 0b00000011;
        waiting(a);
        PORTC.OUT = 0b00000111;
        waiting(a);
    }
}