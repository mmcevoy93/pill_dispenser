/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       LCD Library
// Revision History :
// Created : Unknown
//  Dec 07 2020 - Modified Documentation
//  Dec    2020 - Modified names, modified to use timer for delays
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
// 8-Bit interface init on LCD
// LCD is wired to PTH for data, PK0:2 for control :
// 2     1     0     PTK 
// A     R/W*  E     LCD 
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// LCD Address Scheme (HEX):
// 00 01 ... 12 13
// 40 41 ... 52 53
// 14 15 ... 26 27
// 54 55 ... 66 67
////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// note: lcd_Init initializes the timer!
void lcd_Init (void);
void lcd_Inst (int val);
void lcd_Data (unsigned char val);
void lcd_Addr (unsigned char addr);
void lcd_AddrXY (unsigned char ix, unsigned char iy);
void lcd_String (char const * straddr);
void lcd_StringXY (unsigned char ix, unsigned char iy, char const * const straddr);

void lcd_DispControl (unsigned char curon, unsigned char blinkon);
void lcd_Clear (void);
void lcd_Home (void);
void lcd_ShiftL (void);
void lcd_ShiftR (void);

void lcd_CGAddr (unsigned char addr);
void lcd_CGChar (unsigned char cgAddr, unsigned char const * const cgData);

