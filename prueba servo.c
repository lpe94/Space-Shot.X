#include <xc.h>
#include <p18f4585.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define _XTAL_FREQ 4000 //para poder utilizar delays con el compilador
#pragma config OSC=IRCIO67
#pragma config MCLRE=ON
#pragma config PBADEN=OFF
#pragma config LVP=OFF
#pragma config WDT=OFF


void Delay (void);
void Delay2 (void);
void Antibounce (void);
void main (void){
    // configurar oscilador
    OSCCON = 0x60;
    //Configurar puerto B
    ADCON1 = 0x0F;
    PORTB=0;
    LATB=0;
    TRISBbits.RB7=0;
    TRISBbits.RB6=1;
    LATC=0;
    PORTC=0;
    TRISC=0b00001000;
    while (1){
        if( PORTCbits.RC3==0){
            PORTC=0b00000000;
        }
        if( PORTCbits.RC3==1){
            PORTC=0b00001111;
            _delay(2100);

            PORTC=0b00000000;
            _delay(42800);
        }
   // PORTBbits.RB7= 0;
    //_delay (42800);
       }
    //while(1){
       // if (PORTBbits.RB6==1){
           // Antibounce();
          //  if (PORTBbits.RB6==1){
            //    PORTBbits.RB7= 1;
            //    _delay(1200);
            //    PORTBbits.RB7= 0;
            //    _delay (42800);
            //}
            
        //}
           
    //} 
    //   srand (3);
   // unsigned char test = rand()%6 ;
    //unsigned char wiw = 0xAF1;
    //wiw = test;
}  
    
void Delay (void){
unsigned char L3REG=100;
while (L3REG++ <100){
    }
}
void Delay2 (void){
unsigned char L3REG=26;
while (L3REG++ <255){
    }
}
void Antibounce (void){
unsigned char L1REG = 0; 
unsigned char L2REG; 
while(L1REG++ < 13){ 
    L2REG = 0; 
    while(L2REG++ < 255); 
    }
}


