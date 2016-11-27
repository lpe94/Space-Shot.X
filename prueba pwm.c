#include <xc.h>
#include <p18f4585.h>

#pragma config OSC=IRCIO67
#pragma config MCLRE=ON
#pragma config PBADEN=OFF

void interrupt ISRH(void);
void Interr0_ISR(void);
void Interr1_ISR(void);

void main(void){
// configurar oscilador
    OSCCON = 0x60;
    CCP1CON=0x00;
    TRISCbits.RC2= 0;
    T2CON=0x02;
    PR2= 686;
    CCPR1L=0;
    CCP1CON=0x15;
    TMR2=0;
    RCONbits.IPEN=1;
    INTCONbits.GIEH=1;
    INTCON2bits.INTEDG0=1;
    INTCONbits.INT0IE=1;
    INTCON3bits.INT1IP=1;
    INTCON2bits.INTEDG1=1;
    INTCON3bits.INT1IE=1;
    IPR1bits.TMR2IP=1;
    PIE1bits.TMR2IE=1;
    T2CONbits.TMR2ON=1;
    while(1){}    
}
void interrupt IRH(void){
    if (INTCONbits.INT0F==1)
        Interr0_ISR();
    if (INTCON3bits.INT1E==1)
        Interr1_ISR();
    PIR1bits.TMR2IF=0;
    }
void Interr0_ISR(void){
    INTCONbits.INT0IF=0;
    CCPR1L=19;
    TMR2=0;

}
void Interr1_ISR(void){
    INTCON3bits.INT1IF=0;
    CCPR1L=131;
    TMR2=0;
}