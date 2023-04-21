/////////////////////////////////////////////////////////////////////////////
// Processor:     AVR128DB28
// Bus Speed:     
// Author:        Maxwell McEvoy
// Details:       Real Time Clock 
// Revision History
//      Created: FEB 25 2023
//      
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
	char sec;
	char min;
	char hour;
    char day;
    char date;
    char month;
    char year;
} rtc_t;
int get_temp();
void init_rtc();
void get_rtc_time(rtc_t *rtc);
void set_rtc_time(char sec, char min, char hour, char day, char date, char month, char year);
