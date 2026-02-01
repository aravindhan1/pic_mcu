#include <xc.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2

void lcd_data(unsigned char data)
{
    PORTC = (data & 0xF0);
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
    PORTC = ((data<<4)& 0xF0);
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTC = (cmd & 0xF0);
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
    PORTC = ((cmd<<4)& 0xF0);
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_string(const unsigned char *str, unsigned char num)
{
    unsigned char i;
    for(i=0;i<num;i++){
        lcd_data(str[i]);
    }
}

void lcd_init()
{
    lcd_command(0x02);
    lcd_command(0x28);//4bit mode 16x2
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
}


void main(void) {
    TRISC = 0x00;
    TRISD = 0x00;
    lcd_init();
    while (1) {
        lcd_command(0x86);
        lcd_string("Embedded",8);
        lcd_command(0xC0);
        lcd_string("System",6);
    }
    return;
}
