/*
 * File:   main.c
 * Author: aravindhan-tv
 *
 * Created on January 29, 2026, 11:06 AM
 */

//PORTb PORTD TRISB are predefines as addresses
#include <xc.h>

//turn off WDT (Watch dog timer )
#pragma config WDTE=OFF
#define _XTAL_FREQ 20000000
#define segment_on 1
#define segment_off 0

unsigned char  number = 78;
unsigned char i,j; 
unsigned char seg[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void main(void) {
    TRISB = 0x00;
    TRISC = 0x00;
    while (1) {
        i = number/10;
        j = number%10;
        //1. turn on tbnhe d1 and turn off the d2
        PORTCbits.RC0 = segment_on;
        PORTCbits.RC1 = segment_off;
        //2. Print req data in d1
        PORTB = seg[i];
        __delay_ms(10);
        //3. turn on d2 and off d1
        PORTCbits.RC1 = segment_on;
        PORTCbits.RC0 = segment_off;
        //4. Print req data on d2
        PORTB = seg[j];
        __delay_ms(10);    
    }
}
