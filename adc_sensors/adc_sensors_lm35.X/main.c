#include <xc.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2

unsigned int a,b,c,d,e,f;
unsigned int temp,adc;
float temprature,adc1;

void delay(unsigned int i){
    while(i--);
}

void lcd_data(unsigned char data)
{
    PORTC = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
    
}

void lcd_command(unsigned char cmd)
{
    PORTC = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_string(const char *str, unsigned char num)
{
    unsigned char i;
    for(i=0;i<num;i++){
        lcd_data(str[i]);
    }
}

void lcd_init()
{
    lcd_command(0x38);
    lcd_command(0x06);
    lcd_command(0x0C);
    lcd_command(0x01);
}

void __interrupt adc_conv(){
    if(PIR1bits.ADIF==1){
        adc=(ADRESH<<8);
        adc=adc+ADRESL;
        PIR1bits.ADIF=0;
    }
}


void main(void) {
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    PIE1bits.ADIE=1;
    
    TRISC = 0x00;
    TRISD = 0x00;
    
    PORTC=PORTD=0x00;
    lcd_init();
    lcd_command(0x80);
    lcd_string("Sensor",6);
    
    ADCON0 = 0x41;
    ADCON1 = 0x8E;
    
    while (1) {
        ADCON0=ADCON0|(0x04);
        adc1=adc/2.046; //temprature value finding
        temprature = adc1*100; //fetching decimal place numbers
        
        lcd_command(0x89);
        a=(int)temprature/10;
        b=(int)temprature%10;
        c=a%10;
        d=a/10;
        e=d%10;
        f=d/10;
        chr(f+0x30);
        chr(e+0x30);
        chr('.');
        chr(c+0x30);
        chr(b+0x30);
    }
    return;
}
