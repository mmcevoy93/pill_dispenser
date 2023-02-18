/*
 * File:   newavr-main.c
 * Author: Max
 *
 * Created on February 16, 2023, 8:53 PM
 */


#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"
#include "timer.h"


int main(void) {
    
    (void)lcd_Init();
    
    int ms, s = 0;
    char c[20];
    lcd_String("HELLO THIS IS A TEST");
    lcd_AddrXY(0,1);
    lcd_String("Although time is wrong");
    for(;;){
        
        lcd_AddrXY(0,3);
        (void)Timer_Sleep(1);
        sprintf(c, "Up Time %2d.%2d", s, ms);
        lcd_String(c);
        ms += 1;
        if (ms>1000){
            ms = 0;
            s +=1;
        }
        
    }
   
}
