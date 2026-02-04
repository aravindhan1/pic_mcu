/*
 * File:   eeprom_lcd.c
 * Author: aravindhan-tv
 * MCU: PIC16F877A
 */

#include <xc.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// LCD control pins
#define RS RD0
#define RW RD1
#define EN RD2

unsigned char byte;   // to store EEPROM read value

// ---------- LCD FUNCTIONS ----------
void lcd_command(unsigned char cmd)
{
    PORTC = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_data(unsigned char data)
{
    PORTC = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_string(const char *str)
{
    while(*str)
    {
        lcd_data(*str++);
    }
}

void lcd_init()
{
    __delay_ms(20);
    lcd_command(0x38); // 8-bit, 2 line
    lcd_command(0x0C); // display ON cursor OFF
    lcd_command(0x06); // auto increment
    lcd_command(0x01); // clear display
    __delay_ms(2);
}

// ---------- EEPROM FUNCTIONS ----------
void WriteByteToEE(unsigned char data, unsigned char address)
{
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0; // EEPROM memory
    EECON1bits.WREN = 1;  // Enable write

    INTCONbits.GIE = 0;   // Disable interrupts

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while(PIR2bits.EEIF == 0); // wait till write complete
    PIR2bits.EEIF = 0;

    EECON1bits.WREN = 0;
    INTCONbits.GIE = 1;   // Enable interrupts
}

unsigned char ReadByteFromEE(unsigned char address)
{
    EEADR = address;
    EECON1bits.EEPGD = 0; // EEPROM memory
    EECON1bits.RD = 1;
    return EEDATA;
}

// ---------- MAIN ----------
void main()
{
    TRISD = 0x00; // LCD data port
    PORTD = 0x00;

    TRISC = 0x00; // LCD control port
    PORTC = 0x00;

    lcd_init();

    lcd_command(0x80);
    lcd_string(" EEPROM VALUE:");

    while(1)
    {
        WriteByteToEE(7, 0x0A);     // write value 7 at address 0x0A
        __delay_ms(50);

        byte = ReadByteFromEE(0x0A);  // read back

        lcd_command(0xC0);           // second line
        lcd_data(byte + 0x30);       // convert to ASCII

        __delay_ms(1000);
    }
}
