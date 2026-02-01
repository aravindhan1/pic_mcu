/*
 * File:   main.c
 * Author: aravindhan-tv
 *
 * Created on February 1, 2026, 8:31 PM
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
