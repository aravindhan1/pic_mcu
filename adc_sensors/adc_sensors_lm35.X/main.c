#include <xc.h>
#define _XTAL_FREQ 20000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2

int a,b,c,d,e,f;
unsigned int adc;
float temperature, adc1;

void lcd_data(unsigned char data)
{
    PORTC = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTC = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_string(const char *str)
{
    while(*str)
        lcd_data(*str++);
}

void lcd_init()
{
    lcd_command(0x38);
    lcd_command(0x06);
    lcd_command(0x0C);
    lcd_command(0x01);
}

void __interrupt() adc_conv(void)
{
    if(PIR1bits.ADIF)
    {
        adc = ((unsigned int)ADRESH << 8) | ADRESL;
        PIR1bits.ADIF = 0;
    }
}

void main(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;

    TRISC = 0x00;
    TRISD = 0x00;
    PORTC = PORTD = 0x00;

    lcd_init();
    lcd_command(0x80);
    lcd_string("Sensor");

    ADCON0 = 0x41;   // ADC ON, channel 0
    ADCON1 = 0x8E;   // Right justified, AN0 analog

    while(1)
    {
        ADCON0bits.GO_nDONE = 1;   // Start ADC

        adc1 = adc / 2.046;
        temperature = adc1 * 100;

        lcd_command(0x89);

        a = (int)temperature / 10;
        b = (int)temperature % 10;
        c = a % 10;
        d = a / 10;
        e = d % 10;
        f = d / 10;

        lcd_data(f + '0');
        lcd_data(e + '0');
        lcd_data('.');
        lcd_data(c + '0');
        lcd_data(b + '0');

        __delay_ms(500);
    }
}
