/*
 * File:   main.c
 * Author: aravindhan-tv
 *
 * Created on January 29, 2026, 10:27 AM
 */

//PORTb PORTD TRISB are predefines as addresses
#include <xc.h>

//turn off WDT (Watch dog timer )
#pragma config WDTE=OFF

void init_config() {

}

void main(void) {
    init_config();
    while (1) {

    }
}
