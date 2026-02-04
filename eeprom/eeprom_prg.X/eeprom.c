/*
 * File:   eeprom.c
 * Author: aravindhan-tv
 *
 * Created on February 3, 2026, 10:03 PM
 */

//PORTb PORTD TRISB are predefines as addresses
#include <xc.h>

//turn off WDT (Watch dog timer )
#pragma config WDTE=OFF

void ReadByteFromEE(const unsigned char address)
{
    EEADR = address;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    byte = EEDATA;
}

//WriteByteToEE(7, 0x0A);   // 00 - FE

void WriteByteToEE(unsigned char data, const unsigned char address)
{
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while (PIR2bits.EEIF == 0);
    PIR2bits.EEIF = 0;
}

void main()
{
    TRISD = 0x00;
    PORTD = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;
    lcd_init();

    lcd_command(0x80);
    lcd_string(" EEPROM VALUE:", 16);

    while(1)
    {
        WriteByteToEE(7, 0x0A);

        ReadByteFromEE(0x0A);

        lcd_command(0xC0);
        lcd_data(byte + 0x30);
    }
}
