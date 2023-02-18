#include "timer.h"
#include <avr/io.h>
// internal reference to the bus speed, set by Timer_Init, used by Sleep
unsigned long _Timer_Bus_Clock = 0;
int _prescale = 0;

// init the timer and enable it (around OC0)
void Timer_Init (MHz_Output MHz, Timer_Prescale prescale, int enableInt)
{
  // save the bus rate, so other functions can make reference to it
  
  CCP = 0xd8;
  CLKCTRL.OSCHFCTRLA = MHz<<2;//12.5.7
  
  
  while( CLKCTRL.MCLKSTATUS & 0b00000001 ){;}
  
  
  //23.5.1
  TCA0.SINGLE.CTRLA |= 0b00000001; //enable
  TCA0.SINGLE.CTRLA |= prescale << 1; //Clockselect
  TCA0.SINGLE.CTRLA &= 0b01111111; //Standby Sleep mode disabled
  
  TCA0.SINGLE.CTRLB = 0b00000000;
  //23.5.11
  TCA0.SINGLE.INTFLAGS = 0b01110001; //Clear all int flags if any
  if(enableInt)//23.5.10
    TCA0.SINGLE.INTCTRL = 0b00000001; //Timer Overflow interrupt Enabled
  
  //save the pre scale for later use
  if(prescale==0)  _prescale =1;
  else if(prescale==1)  _prescale = 2;
  else if(prescale==2)  _prescale = 4;
  else if(prescale==3)  _prescale = 8;
  else if(prescale==4)  _prescale = 16;
  else if(prescale==5)  _prescale = 64;
  else if(prescale==6)  _prescale = 256;
  else if(prescale==7)  _prescale = 1024;
  
  //Save timer bus for later use
  if(MHz == 0) _Timer_Bus_Clock = 1000000;
  else if(MHz == 1) _Timer_Bus_Clock = 2000000;
  else if(MHz == 2) _Timer_Bus_Clock = 3000000;
  else if(MHz == 3) _Timer_Bus_Clock = 4000000;
  else if(MHz == 5) _Timer_Bus_Clock = 8000000;
  else if(MHz == 6) _Timer_Bus_Clock = 12000000;
  else if(MHz == 7) _Timer_Bus_Clock = 16000000;
  else if(MHz == 8) _Timer_Bus_Clock = 20000000;
  else if(MHz == 9) _Timer_Bus_Clock = 24000000;
}

// assumes :
//  TSCR1 |= 0x80;      // enable main timer
// and OC6 is avail (this code does not use pin drive for OC6)
// note, this is approximate, as there will be rounding error for some prescale values
// each tick is offset from TCNT, not previous OC6 value (adds drift)
int Timer_Sleep (unsigned int ms)
{  
    
    TCA0.SINGLE.PER = ms*(_Timer_Bus_Clock  /(_prescale * 2000)) - 1; //65535 largest
    
    
    PORTA.OUT ^= 0b00100000;
    while((TCA0.SINGLE.INTFLAGS & 1) == 0){
        ;
    }
    TCA0.SINGLE.INTFLAGS = 1;
    
  return 0;
}

