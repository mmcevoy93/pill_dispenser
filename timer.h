/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Maxwell McEvoy
// Details:       Timer Library
// Revision History
//      Created: FEB 23 2020
//      
/////////////////////////////////////////////////////////////////////////////

// not specifically useful, but documents code a little better
typedef enum Timer_Prescale
{
  Timer_Prescale_1 = 0,
  Timer_Prescale_2 = 1,
  Timer_Prescale_4 = 2,
  Timer_Prescale_8 = 3,
  Timer_Prescale_16 = 4,
  Timer_Prescale_64 = 5,
  Timer_Prescale_256 = 6,
  Timer_Prescale_1024 = 7
} Timer_Prescale;

typedef enum MHz_Output
{
  MHz_Output_1 = 0,
  MHz_Output_2 = 1,
  MHz_Output_3 = 2,
  MHz_Output_4 = 3,
  MHz_Output_8 = 5,
  MHz_Output_12 = 6,
  MHz_Output_16 = 7,
  MHz_Output_20 = 8,
  MHz_Output_24 = 9
} MHz_Output;

// actions pins can take on compare event


// timer general (uses OC0)
// prescale value as 2^N (0 - 7 as argument = bus / (1 to 128)) in general, 7 would be used (div by 128)
//  now uses enum, so division value is readily documented
// uiOffset is initial offset from TCNT (first event time)
// enableInt is flag for enabling interrupts on OC0
// pin action is action pin should take on event
void Timer_Init (MHz_Output MHz, Timer_Prescale prescale, int enableInt);

// blocking delay for x ms - requires previous InitTimer call
// calculates values required to generate 1ms delay (may not be perfectly accurate)
//  then loops ms times
// return error code (-1) on error
// uses OC6, no pin action for timing
int Timer_Sleep (unsigned int ms);

