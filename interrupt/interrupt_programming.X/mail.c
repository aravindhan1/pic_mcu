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

void __interrupt() isr(void) {
    
    if (INTCONbits.INTF == 1)
    {
        PORTCbits.RC0 = ~PORTCbits.RC0;
        INTCONbits.INTF = 0;
    }

}

void main(void) {
    INTCONbits.GIE=1;
    INTCONbits.INTE = 1;
    INTCONbits.PEIE=1;
    
    OPTION_REGbits.INTEDG=1;
    TRISC0 = 0;
    while (1) {
        

    }
}
