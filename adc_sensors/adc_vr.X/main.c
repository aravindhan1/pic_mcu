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

#define RS PORTBbits.RB0
#define RW PORTBbits.RB1
#define EN PORTBbits.RB2

unsigned char a,b,c,d,e,f;

void lcd_data(unsigned char data)
{
    PORTB = (data & 0xF0);
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
    PORTB = ((data<<4)& 0xF0);
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTB = (cmd & 0xF0);
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
    PORTB = ((cmd<<4)& 0xF0);
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

unsigned int adc_value;
void main(void) {
    TRISB = 0x00;
    lcd_init();
    ADCON0bits.ADCS0=1;//}
    ADCON0bits.ADCS1=0;////--FOSC/8
    ADCON1bits.ADCS2=0;//}
    ADCON0bits.ADON = 1;//power up adc module
    ADCON1bits.PCFG0 = 0;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.ADFM=1;//right justified
    ADCON0bits.CHS0=0;//}
    ADCON0bits.CHS1=0;////- Selected channel 0;
    ADCON0bits.CHS2=0;//}
    ADCON0bits.GO_DONE = 1; ///Start ADC converstion
    
    while (1) {
        lcd_command(0x80);
        lcd_string("Pot Value:",10);
            ADCON0bits.CHS0=0;//}
            ADCON0bits.CHS1=0;////- Selected channel 0;
            ADCON0bits.CHS2=0;//}
            ADCON0bits.GO_DONE = 1; ///Start ADC converstion
            while(PIR1bits.ADIF == 0); //adc conversion complete
            adc_value = ADRESH<<8;
            adc_value = adc_value + ADRESL;
            
            
            //let  adc_value = 1023
            
            a = adc_value /10; //102
            b = adc_value % 10; //3 last digit
            c = a/10;
            d = a%10; //2 third digit
            e = c/10; //e = 1 first digit
            f = c%10; //f=0 second digit
            lcd_command(0x8B);
            lcd_data(e+0x30);
            lcd_data(f+0x30);
            lcd_data(d+0x30);
            lcd_data(b+0x30);
            
    }
    return;
}
