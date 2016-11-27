  list p=18f4585
    #include "p18f4585.inc"
    CONFIG OSC= IRCIO67
    CONFIG MCLRE=ON 
; puertos B comoI/O
    CONFIG PBADEN = OFF
    CONFIG LVP = OFF   
Dato EQU 0x010
Conta EQU 0x011
    ORG 0x0000
main
    MOVFF 0xFB4, Dato
    btfsc   Dato, 
    incf     Conta, A
    btfsc   Dato, 3
    incf     Conta, A
    btfsc   Dato, 2
    incf     Conta, A
    btfsc   Dato, 0
   incf     Conta, A
   ADDWF    Dato, 

    SWAPF       Conta, f
    SWAPF       Dato, f

    clrf     Conta
 END
              