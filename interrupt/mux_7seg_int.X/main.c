#include <xc.h>

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

#define digit_on   0   // common cathode: enable by grounding
#define digit_off  1

unsigned char number = 0;
unsigned char i, j;
unsigned char timer_count = 0;

/* Common Cathode 7-seg codes (abcdefg) */
unsigned char seg[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void __interrupt() isr(void)
{
    if (INTCONbits.TMR0IF)
    {
        TMR0 = 59;   // reload for ~10ms

        if (timer_count == 0)
        {
            PORTCbits.RC0 = digit_on;   // Digit 1 ON
            PORTCbits.RC1 = digit_off;  // Digit 2 OFF
            PORTB = seg[i];
        }
        else if (timer_count == 1)
        {
            PORTCbits.RC1 = digit_on;   // Digit 2 ON
            PORTCbits.RC0 = digit_off;  // Digit 1 OFF
            PORTB = seg[j];
        }

        timer_count++;
        if (timer_count >= 2)
            timer_count = 0;

        INTCONbits.TMR0IF = 0;
    }
}

void main(void)
{
    TRISB = 0x00;   // segments output
    TRISC = 0x00;   // digit control output

    OPTION_REG = 0b00000111; // prescaler 1:256, internal clock
    TMR0 = 59;

    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1;

    while (1)
    {
        i = number / 10;   // tens digit
        j = number % 10;   // units digit

        number++;
        if (number == 100)
            number = 0;

        __delay_ms(500);
    }
}
