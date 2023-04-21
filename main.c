/*
 * File:   main.c
 * Author: Max
 *
 * Created on March 20, 2023, 4:42 PM
 */


#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"
#include "servo.h"
#include "DS3231_rtc.h"
struct med {
        char name[20];
        char days;
    } ;
struct med meds[8];
void cw_step(int steps);
void ccw_step(int steps);
void dispense(char day);
void get_med_data();
void write_meds();
void get_meds_stored();


#define delay { int __x = 2000; while (--__x); }


int main(void) {

    USART0.BAUD = 4000;
    USART0.CTRLC = 0b00000011;
    PORTA.DIRSET |= 0b00000001;
    PORTA.DIRCLR |= 0b00000010;
    USART0.CTRLB = 0b11000000;
    USART0.CTRLA = 0b10000000;
    
    
    servo_init();
    lcd_Init();
    init_rtc();
    rtc_t rtc;
    
    get_rtc_time(&rtc);
    

    //get_meds_stored();
    char c[20];
    
    PORTD.DIR = 0b00111100;
    
    for(;;){
        if(!(PORTD.IN & 0b00000010)){
            char day = 0;
            if(rtc.day == 1)
                day = 0b00100000;
            else if(rtc.day == 2)
                day = 0b00010000;
            else if(rtc.day == 3)
                day = 0b00001000;
            else if(rtc.day == 4)
                day = 0b00000100;
            else if(rtc.day == 5)
                day = 0b00100010;
            else if(rtc.day == 6)
                day = 0b00000001;
            else if(rtc.day == 7)
                day = 0b01000000;
            dispense(day);
        }
        if(!(PORTD.IN & 0b01000000)){
            lcd_Clear();
            lcd_AddrXY(0,0);
            lcd_String("Ready for Data");
            get_med_data();
            write_meds();
            lcd_Clear();
        }
        else{
            lcd_AddrXY(0,0);
            get_rtc_time(&rtc);
            int temp = get_temp();
            if(rtc.day==1)
                sprintf(c, "%02X:%02X:%02X %s",rtc.hour,rtc.min,rtc.sec, "Mon");
            if(rtc.day==2)
                sprintf(c, "%02X:%02X:%02X %s",rtc.hour,rtc.min,rtc.sec, "Tues");
            if(rtc.day==3)
                sprintf(c, "%02X:%02X:%02X %s",rtc.hour,rtc.min,rtc.sec, "Wed");
            if(rtc.day==4)
                sprintf(c, "%02X:%02X:%02X %s",rtc.hour,rtc.min,rtc.sec, "Thur");
            if(rtc.day==5)
                sprintf(c, "%02X:%02X:%02X %s",rtc.hour,rtc.min,rtc.sec, "Fri");
            if(rtc.day==6)
                sprintf(c, "%02X:%02X:%02X %s %02X/%02X",rtc.hour,rtc.min,rtc.sec, "Sat", rtc.date, rtc.month);
            if(rtc.day==7)
                sprintf(c, "%02X:%02X:%02X %s",rtc.hour,rtc.min,rtc.sec, "Sun");
            lcd_String(c);
            lcd_AddrXY(0,1);
            sprintf(c, "Temperature: %2d.%02d",temp>>8, (temp>>6 & 3)*25);
            lcd_String(c);
            lcd_Data(223);
            lcd_Data('C');
            lcd_AddrXY(0,2);
            lcd_String("Silver Button: Pills");
            lcd_AddrXY(0,3);
            lcd_String("White Button: Data");
            PORTD.OUT &= 0b11000011;
        }
    }
}

void dispense(char day){
    for(int i = 0; i < 8; i++){
        lcd_Clear();
        if(meds[i].days & day){
            lcd_AddrXY(0,0);
            lcd_String("Dispensing ");
            lcd_AddrXY(0,1);
            lcd_String(meds[i].name);
            //servo
            servo_angle(0.98);
            delay;
            delay;
            servo_angle(0.85);
        }
        cw_step(64);
    }
    
}
void cw_step(int steps){
    for(int i = 0; i < steps; i++){
        PORTD.OUT = 0b00011100;
        delay;
        PORTD.OUT = 0b00001100;
        delay;
        PORTD.OUT = 0b00101100;
        delay;
        PORTD.OUT = 0b00100100;
        delay;
        PORTD.OUT = 0b00110100;
        delay;
        PORTD.OUT = 0b00110000;
        delay;
        PORTD.OUT = 0b00111000;
        delay;
        PORTD.OUT = 0b00011000;
        delay;
    }
}

void ccw_step(int steps){
    for(int i = 0; i < steps; i++){
        PORTD.OUT = 0b00011000;
        delay;
        PORTD.OUT = 0b00111000;
        delay;
        PORTD.OUT = 0b00110000;
        delay;
        PORTD.OUT = 0b00110100;
        delay;
        PORTD.OUT = 0b00100100;
        delay;
        PORTD.OUT = 0b00101100;
        delay;
        PORTD.OUT = 0b00001100;
        delay;
        PORTD.OUT = 0b00011100;
        delay;
    }
}

void get_med_data(){
    char sec, min, hour, day, date, month, year = 0;
    char foo[8];
    int i = 0;
    int j = 0;
    char b = 0;
    char get_name = 1;
    while(!(USART0.STATUS & 0b10000000)){;}
    sec = USART0.RXDATAL;
    while(!(USART0.STATUS & 0b10000000)){;}
    min = USART0.RXDATAL;
    while(!(USART0.STATUS & 0b10000000)){;}
    hour = USART0.RXDATAL;
    while(!(USART0.STATUS & 0b10000000)){;}
    day = USART0.RXDATAL;
    while(!(USART0.STATUS & 0b10000000)){;}
    date = USART0.RXDATAL;
    while(!(USART0.STATUS & 0b10000000)){;}
    month = USART0.RXDATAL;
    while(!(USART0.STATUS & 0b10000000)){;}
    year = USART0.RXDATAL;
    while(1){
        while((USART0.STATUS & 0b10000000)){

            b = USART0.RXDATAL;
            if(b==0xff)break;
            if(get_name){
                meds[j].name[i++] = b;
                if(b == '\0'){
                    i = 0;
                    get_name = 0;
                }
            }
            else{
                meds[j].days = b;
                j++;
                get_name = 1;
            }
        }
        if(b==0xff){
            b = 0;
            for(int k = 0; k<8; k++){
                lcd_Clear ();
                lcd_AddrXY(0,0);
                lcd_String(meds[k].name);
                foo[0] = meds[k].days & 0b01000000 ? 'S' : ' ';
                foo[1] = meds[k].days & 0b00100000 ? 'M' : ' ';
                foo[2] = meds[k].days & 0b00010000 ? 'T' : ' ';
                foo[3] = meds[k].days & 0b00001000 ? 'W' : ' ';
                foo[4] = meds[k].days & 0b00000100 ? 'T' : ' ';
                foo[5] = meds[k].days & 0b00000010 ? 'F' : ' ';
                foo[6] = meds[k].days & 0b00000001 ? 'S' : ' ';
                foo[7] = '\0';
                lcd_AddrXY(12,0);
                lcd_String(" ");
                lcd_String(foo);
                cw_step(64);
            }
            j = 0;
            i = 0;
            break;
        }
    }
    set_rtc_time(sec,min,hour,day,date,month,year);
    
}

void write_meds(){
    NVMCTRL.CTRLA = 0x12;
    NVMCTRL.ADDR = 0x00;
    for(int i = 0; i<8; i++){
        char endof = 0;
        for(int j = 0; j<20; j++){
            char c = meds[i].name[j];
            if(c=='\0')
                endof = 1;
            if(endof){
                NVMCTRL.DATAL = 0;
            }
            else{
                NVMCTRL.DATAL =  meds[i].name[j];
            }
            NVMCTRL.ADDR0++;
        }
        NVMCTRL.DATAL=  meds[i].days;``
    }
}

/* void get_meds_stored(){
    TWI0.MCTRLB &= 0b11111000;
    TWI0.MADDR = 0b11010000;delay //Start and Write
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA = 0x13;delay
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MADDR = 0b11010001;delay  //Start again and read
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    
    for(int i = 0; i<8; i++){
        char endof = 0;
        for(int j = 0; j<20; j++){
            char c = TWI0.MDATA;delay
            while(TWI0.MSTATUS & 0b00010000){;}
            if(c=='\0')
                endof = 1;
            if(endof){
                meds[i].name[j] = 0;
            }
            else{
                meds[i].name[j] =  c;
            }
        }
        if(i==7)
            TWI0.MCTRLB |= 0b00000100; //NACK
        TWI0.MDATA =  meds[i].days;
        delay;
    }
    
    TWI0.MCTRLB |= 0x03; //STOP
} */