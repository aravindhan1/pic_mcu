/*
 * File:   pwm.c
 * Author: aravindhan-tv
 *
 * Created on February 3, 2026, 7:57 PM
 */

//PORTb PORTD TRISB are predefines as addresses
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
void set_pwm_duty_cycle(unsigned int duty_cycle){
    CCPR1L  = (unsigned char)(duty_cycle>>2);
    CCP1CON = CCP1CON & 0xCF; //clearing bit 4 and 5 in ccp1con register
    CCP1CON = CCP1CON | (0x30 & (duty_cycle<<4));
}

void init_config() {
    TRISC   = 0x00;
    CCP1CON = 0x0C;
    PR2     = 0xFF;
    T2CON   = 0x04;
    
}

void main(void) {
    unsigned int i;
    init_config();
    while (1) {
        for(i=0;i<1023;i++){
            set_pwm_duty_cycle(i);
            __delay_ms(2);
        }
        for(i=1023;i>0;i--){
            set_pwm_duty_cycle(i);
            __delay_ms(2);
        }

    }
    return;
}
