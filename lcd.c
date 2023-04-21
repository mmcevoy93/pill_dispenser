#include <avr/io.h>
#include "lcd.h"
// other includes, as *required* for this implementation
/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////
//gotta be inputs, set R/W* high
#define lcd_RWUp PORTA.OUT |= 0b00100000;
//set R/W* low, // gotta be outputs 
#define lcd_RWDown PORTA.OUT &= 0b11011111;

#define lcd_EUp PORTA.OUT |= 0b00010000;
#define lcd_EDown PORTA.OUT &= 0b11101111; 
#define lcd_RSUp PORTA.OUT |= 0b01000000; //22.3.2.15
#define lcd_RSDown PORTA.OUT &= 0b10111111; //22.3.2.15
#define lcd_MicroDelay { char __x = 1; while (--__x); }
#define lcd_longDelay { int __x = 1000; while (--__x); }

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////

//block function wait for LCD to be not busy
void lcd_Busy(){
    lcd_longDelay;
    int temp = PORTC.OUT & 0b00001000;
    PORTC.OUTCLR = 0x08;
    PORTC.PIN4CTRL = 0b10000000;//invert pin 7 to be input
    lcd_RSDown;     //instruction RS=0
    lcd_RWUp;       //Reading   R/W-1
    do{
        lcd_EUp;
        lcd_MicroDelay  //tested @ 20MHz, delay for tDDR absolutely needed (data sheet)
        lcd_MicroDelay               //says 360ns (P58 + P49)
        lcd_MicroDelay                //@50ns per clock and average 2.5 cycles per instruction, this
        lcd_MicroDelay                //is about 3 average assembly ubstructon.
        lcd_MicroDelay                // status (d7) and address (d6:0)
        
        lcd_EDown;
    } 
    while (PORTC.IN & 0b00001000);
    //repeat until device is not busy
    PORTC.PIN4CTRL = 0b10000000; //invert pin 7 to be output again
    PORTC.OUT |= temp;
}

//send a byte to the inst reg of LCD on custom port
void lcd_Inst (int val){
    lcd_Busy(); //wait for lcd to be not busy
    lcd_RWDown;     //writing R/W=0
    lcd_RSDown;     //command RS=0
    PORTC.OUT = 0xF & val>>4;        //present data on port
    lcd_EUp;
    lcd_MicroDelay;
    lcd_EDown;      //and... latch
    PORTC.OUT = 0xF & val;        //present data on port
    lcd_EUp;
    lcd_MicroDelay;
    lcd_EDown;      //and... latch

}

void lcd_Init(){
    PORTA.DIRSET |= 0b01110000;
    PORTC.DIRSET= 0b00001111;
    
    lcd_longDelay;//delay may be there from reset
    lcd_EDown;
    lcd_RSDown;
    lcd_RWDown; 
    lcd_longDelay;
    lcd_EUp;
    lcd_EDown;
    lcd_longDelay;
    lcd_EUp;
    lcd_EDown;
    lcd_longDelay;
    //Function set
    lcd_Inst(0x02); //move cursor home.
    lcd_Inst(0x28); //Function Set //Data length 4 bits
    lcd_Inst(0x01); //Clear
    lcd_Inst(0x0c); //Cursor off
    lcd_Inst(0x06); //increment cursor
    lcd_Busy();
;}

void lcd_Data (unsigned char val){
    //writes a single character to the LCD screen behind the order there
    lcd_Busy();
    lcd_RWDown;     //writing R/W=0
    lcd_RSUp;     //command RS=0
    PORTC.OUT = 0xF & val>>4;        //present data on port
    lcd_EUp;
    lcd_MicroDelay;
    lcd_EDown;      //and... latch
    PORTC.OUT = 0xF & val;        //present data on port
    
    lcd_EUp;
    lcd_MicroDelay;
    lcd_EDown;      //and... latch
}

void lcd_Clear (){

    lcd_Busy();
    lcd_RWDown;     //writing R/W=0
    lcd_RSDown;       //command RS=0
    //Byte of data for Port H
    lcd_Inst(0x01);        //present data on port //22.3.2.46
    lcd_EUp;        //Latching?
    lcd_EDown;      //and... latch
}



void lcd_String (char const * straddr){
    //no busy since I am using other functions
    char letter = *(straddr);
    char i = 0;
    while(letter!=0){
        lcd_Data(letter);
        letter = *(straddr + (++i));
    }
}
void lcd_AddrXY(unsigned char ix, unsigned char iy){
    lcd_Busy();
    //Byte of data for Port H
    if(iy==0)
        lcd_Inst(0b10000000 | ix);
    else if(iy==1)
        lcd_Inst(0b10000000 | (ix + 0x40)); 
    else if(iy==2)
        lcd_Inst(0b10000000 | (ix + 0x14));       
    else if(iy==3)
        lcd_Inst(0b10000000 | (ix + 0x54));       
    
}
