#include <avr/io.h>
#include "timer.h"
#include "DS3231_rtc.h"
#include <string.h>
/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////
#define rtc_Delay { Timer_Sleep(1); }
/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////
void init_rtc(){
    TWI0.CTRLA = 0b00000000;
    TWI0.MBAUD = 20;//no idea if this is right
    TWI0.MCTRLA |= 0b00000011;
    TWI0.MCTRLB &= 0b11110000;
    TWI0.MSTATUS = 0x01;
}

void get_rtc_time(rtc_t *rtc){
    char m = 0;
    char d = 0;
    TWI0.MCTRLB &= 0b11111000;
    TWI0.MADDR = 0b11010000;rtc_Delay //Start and Write
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA = 0x00;rtc_Delay  //Word address for seconds
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MADDR = 0b11010001;rtc_Delay  //Start again and read
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    rtc->sec = TWI0.MDATA;rtc_Delay
    rtc->min = TWI0.MDATA;rtc_Delay
    rtc->hour = TWI0.MDATA;rtc_Delay
    rtc->day = TWI0.MDATA;rtc_Delay
    
    rtc->date = TWI0.MDATA;rtc_Delay
    
    rtc->month = TWI0.MDATA;rtc_Delay
    
    TWI0.MCTRLB |= 0b00000100; //NACK
    rtc->year = TWI0.MDATA;rtc_Delay
    TWI0.MCTRLB |= 0x03; //STOP
}
void set_rtc_time(char sec, char min, char hour, char day, char date, char month, char year){
    TWI0.MCTRLB &= 0b11111000;
    TWI0.MADDR = 0b11010000;rtc_Delay //Start and Write
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA = 0x00;rtc_Delay  //Word address for seconds
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA =  (sec/10<<4) + sec%10;rtc_Delay //seconds
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK        
    TWI0.MDATA =  (min/10<<4) + min%10;rtc_Delay //minutes
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA =  0b11000000 +(hour/10<<4) + hour%10;rtc_Delay //hours
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA =  (day/10<<4) + day%10;rtc_Delay //days
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA =  (date/10<<4) + date%10;rtc_Delay //date
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA =  (month/10<<4) + month%10;rtc_Delay //month
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MDATA =  (year/10<<4) + year%10;rtc_Delay //year 
    while(TWI0.MSTATUS & 0b00010000){;} // RXACK
    TWI0.MCTRLB |= 0x03; //STOP
}
