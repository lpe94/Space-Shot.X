list p=18f4585
    #include "p18f4585.inc"
    CONFIG OSC= IRCIO67
    CONFIG MCLRE=ON 
; puertos B comoI/O
    CONFIG PBADEN = OFF
    CONFIG LVP = OFF
    
    ORG 0x0000
main
; configurar oscilacion
    MOVLW	B'01100000'     ; 4 MHz internal oscillator
    MOVWF	OSCCON,A        ; Oscilator configuration

    BCF CVRCON, CVREN, A
;Configurar puerto B como entradas y salidas
    MOVLW 0x0F ; WREG = 0x0F
    MOVWF ADCON1, A ; Disables A/D Converter
    CLRF PORTB, A
    CLRF LATB, A
    BCF TRISB, RB7, A
    BSF TRISB, RB6, A
Loop BTFSC PORTB,RB6, A
     BRA DELAY
     BRA Loop
continue BTFSC PORTB,RB6, A
	 Call Pulso
	 BRA Loop
    
DELAY MOVLW D'13'
    MOVWF 0x10, A
L1 MOVLW D'255'
    MOVWF 0x11, A
L2 DECF 0x11, 1, A
    BNZ L2
    DECF 0x10, 1, A
   BNZ L1
   BRA continue
   
Pulso BSF PORTB, RB7, A
      Call DELAY
      BCF PORTB, RB7, A
      ;Call DELAY3
      Return
      
DELAY2 MOVLW D'100'
    MOVWF 0x10, A
L3  DECF 0x10, 1, A
    BNZ L3
   Return
   
DELAY3 MOVLW D'25'
    MOVWF 0x10, A
L4 MOVLW D'255'
    MOVWF 0x11, A
L5 DECF 0x11, 1, A
    BNZ L5
    DECF 0x10, 1, A
   BNZ L4
   Return
   
DELAY4 MOVLW D'13'
    MOVWF 0x10, A
L6 MOVLW D'255'
    MOVWF 0x11, A
L7 DECF 0x11, 1, A
    BNZ L7
    DECF 0x10, 1, A
   BNZ L6
   Return
   
END