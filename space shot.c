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
//#pragma INTCON2.RBPU=0 //pull up resistors on

#define A0 PORTAbits.RA0
#define A1 PORTAbits.RA1
#define A2 PORTAbits.RA2
#define A3 PORTAbits.RA3
#define A4 PORTAbits.RA4
#define A5 PORTAbits.RA5
#define A6 PORTAbits.RA6
#define A7 PORTAbits.RA7

#define B0 PORTBbits.RB0
#define B1 PORTBbits.RB1
#define B2 PORTBbits.RB2
#define B3 PORTBbits.RB3
#define B4 PORTBbits.RB4
#define B5 PORTBbits.RB5
#define B6 PORTBbits.RB6
#define B7 PORTBbits.RB7


#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E PORTCbits.RC2
#define C3 PORTCbits.RC3
#define C4 PORTCbits.RC4
#define C5 PORTCbits.RC5
#define C6 PORTCbits.RC6
#define C7 PORTCbits.RC7

#define D0 PORTDbits.RD0
#define D1 PORTDbits.RD1
#define D2 PORTDbits.RD2
#define D3 PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

#define E0 PORTEbits.RE0
#define E1 PORTEbits.RE1
#define E2 PORTEbits.RE2
#define Reset PORTEbits.RE3

#pragma ORG=0x0000 //origen en 0

//==========Guardar en ROM el menu de inicio==============
//========================================================
const unsigned char INICIO[]@0x20= "Bienvenido";
unsigned char myRAMarray[10] @ 0x020; // RAM data.
const unsigned char DIFICULTAD []@0x30="Dificultad?";
unsigned char myRAMarray2[10] @ 0x030; // RAM data.
const unsigned char NIVEL1 []@0x40="1=facil";
unsigned char myRAMarray3[7] @ 0x040; // RAM data.
const unsigned char NIVEL2 []@0x50="2=medio";
unsigned char myRAMarray4[7] @ 0x050; // RAM data.
const unsigned char NIVEL3 []@0x60="3=dificil";
unsigned char myRAMarray5[9] @ 0x060; // RAM data.

//============Guardar en ROM el menu de resultados==============
//===========================================================
const unsigned char RESULTADO []@0x70="Tu resultado es:";
unsigned char myRAMarray6[15] @ 0x070; // RAM data.
//===========Declaración de subrutinas========================
//===========================================================
void LCDsetup (void);
void LongDelay (void);
void ShortDelay (void);
void LCDready (void);
void LCDdisplay (void);
void Command (void);
void Servo_In (void);
void Servo_End (void);
void Antibounce (void);
void One_Shot_N1 (void);
void Reset_One_Shot_N1 (void);
void One_Shot_N2 (void);
void Reset_One_Shot_N2 (void);
void One_Shot_N3 (void);
void Reset_One_Shot_N3 (void);
void One_Shot_B1 (void);
void Reset_One_Shot_B1 (void);
void One_Shot_B2 (void);
void Reset_One_Shot_B2 (void);
void One_Shot_B3 (void);
void Reset_One_Shot_B3 (void);
void One_Shot_B4 (void);
void Reset_One_Shot_B4 (void);
void One_Shot_B5 (void);
void Reset_One_Shot_B5 (void);
void N1 (void);
void N2 (void);
void N3 (void);
void Bienvenido (void);
void Resultado (void);
void Dificultad(void);
void Menu1 (void);
void Menu2 (void);
void Menu3 (void);
void interrupt ISRH(void);
//===========Declaración de variables globales========================
//===========================================================
unsigned char t; //variable t global
unsigned char counter; //variable counter global
unsigned char counter2; //variable counter global
unsigned char acierto; //vatiable acierto global
unsigned char intentos; //variable intentos global
unsigned char score[3]; //variable intentos global
unsigned char ons_N1; //one shot para boton dificultad 1=facil
unsigned char ons_N2; //one shot para boton dificultad 2=medio
unsigned char ons_N3; //one shot para boton dificultad 3=dificil
unsigned char ons_B1; //one shot para boton1
unsigned char ons_B2; //one shot para boton2
unsigned char ons_B3; //one shot para boton3
unsigned char ons_B4; //one shot para boton4
unsigned char ons_B5; //one shot para boton5
void main (void){
    //*******configurar oscilador*********
    OSCCON = 0x60;
    //********Configurar puerto A*********
    CVRCONbits.CVREN=0;
    PORTA= 0x00;
    LATA= 0x00;
    TRISAbits.RA0= 0xFF; //Pushbutton 0
    TRISAbits.RA1= 0xFF; //Pushbutton 1
    TRISAbits.RA2= 0xFF; //Pushbutton 2
    TRISAbits.RA3= 0xFF; //Pushbutton 3
    TRISAbits.RA4= 0xFF; //Pushbutton 4
    TRISAbits.RA5= 0x00; //LED 5
    //*****Configurar puerto B***********
    ADCON1 = 0x0F;
    PORTB= 0x00;
    LATB = 0x00;
    TRISB = 0xF0; //entrada del teclado matricial
    //**********Configurar puerto C*******
    PORTC= 0x00;
    LATC= 0x00;
    TRISC=0x00; // RS, RW y E del display y 4 LEDS
    //********Configurar puerto D********* 
    CMCON = 0x07;
    PORTD = 0x00;
    LATD = 0x00;
    TRISD = 0x00; //Conexión al LCD
    //********Configurar puerto E********
    PORTE = 0x00;
    LATE = 0x00;
    TRISEbits.RE0 = 0x00; //pulse width para el servo
    TRISEbits.RE1 = 0x00;
    TRISEbits.RE2 = 0x00;
    //**********Configurar Interrupciones********
    RCONbits.IPEN=1; //Enables priority feature. 
    INTCONbits.GIEH=1; //Enables all high priority interruptions. 
    INTCONbits.RBIE=1; //Enables the RB port state change interrupt.
    INTCON2bits.RBIP=1; // PORT B Change Interrupt Priority Bit = High priority. 
    //********Inicializar Space Shot************
    LCDsetup(); //inicializa el display lcd
    Servo_In(); //coloca al servo en la posición de inicio
    //*******Se espera una selección del usuario********
        Bienvenido(); //despliega mensaje de bienvenido
        Dificultad(); //despliega mensaje de dificultad?
        Menu1(); //despliega mensaje de 1=facil
        Menu2(); //despliega mensaje de 2=dificil
        Menu3(); //despliega mensaje de 3=dificil
    while (1){ //mientras los botones de dificultad no sean presionados el menu se sigue mostrando
       for (t=0; t<10; t++){ //se genera un counter de 0-100 para la generacion de numeros aleatorios 
        }
        
    }

    
        //Resultado ();
    }
  
void interrupt IRH(void){
    //*******Se detecta cual dificultad fue seleccionada*******  
    if (PORTBbits.RB5==1){
        One_Shot_N1 ();
        if (ons_N1 == 1) // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
        Reset_One_Shot_N1();
    }
    if (PORTBbits.RB6==1){
        One_Shot_N2 ();
        if (ons_N2 == 1) // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
        Reset_One_Shot_N2();
    }
    if (PORTBbits.RB7==1){
        One_Shot_N3 ();
        if (ons_N3 == 1) // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
        Reset_One_Shot_N3();
    }
    INTCONbits.RBIF=0;
    }
// ==========Subrutinas Display LCD==============================
//==============================================================
void LCDsetup (void){
    E=0x00; 
    LongDelay();
    // Function set
    PORTD= 0x38;
    Command ();
    LongDelay();
    // LCD display on/off control
    PORTD= 0x0F;
    Command ();
    LCDready ();
    // LCD clear display
    PORTD= 0x01;
    Command ();
    LCDready();
}
void LCDready (void){
    TRISD=0xFF;
    RS=0;
    RW=1;
    while (D7 == 1)
    { E=1;
     ShortDelay();
      E=0; 
    }
    TRISD=0x00;
}
void LCDdisplay (void){
    RS=1;
    RW=0;
    E=1;
    ShortDelay();
    E=0;
}
void Command (void){
    RS=0;
    RW=0;
    E=1;
    ShortDelay();
    E=0;
}
void LongDelay (void) {
unsigned  char LDelay=0;
while(LDelay++ < 84){  
    }
}
void ShortDelay (void){
unsigned  char SDelay=0;
while(SDelay++ < 1){  
    }
}

//================Desplegar menu de inicio==========================
//=================================================================
void Bienvenido (void){
unsigned char x;
for(x=0;x<10;x++){
 myRAMarray [x] = INICIO [x];
 PORTD=myRAMarray[x];
 LCDdisplay();
 LCDready();
 }
for(x=0; x<10; x++){
_delay(197000);
}
 }

void Dificultad (void){
D7=0;
LCDsetup();
unsigned char y;
for(y=0;y<11;y++){
 myRAMarray2 [y] =  DIFICULTAD[y];
 PORTD=myRAMarray2[y];
 LCDdisplay();
 LCDready();
 }
for(y=0; y<10; y++){
_delay(197000);
}
}
void Menu1 (void){
D7=0;
LCDsetup();
unsigned char n1;
for(n1=0;n1<7;n1++){
 myRAMarray3 [n1] =  NIVEL1[n1];
 PORTD=myRAMarray3[n1];
 LCDdisplay();
 LCDready();
 }
for(n1=0; n1<10; n1++){
_delay(197000);
}
}

void Menu2 (void){
D7=0;
LCDsetup();
unsigned char n2;
for(n2=0;n2<7;n2++){
 myRAMarray4 [n2] =  NIVEL2[n2];
 PORTD=myRAMarray4[n2];
 LCDdisplay();
 LCDready();
 }
for(n2=0; n2<10; n2++){
_delay(197000);
}
}

void Menu3 (void){
D7=0;
LCDsetup();
unsigned char n3;
for(n3=0;n3<9;n3++){
 myRAMarray5 [n3] =  NIVEL3[n3];
PORTD=myRAMarray5[n3];
 LCDdisplay();
 LCDready();}
for(n3=0; n3<10; n3++){
_delay(197000);
}
    PORTD= 0x01;
    Command ();
    LCDready();
}

//=======Subrutinas del servo===========================
//=====================================================
void Servo_In (void){
    for (counter=0; counter<9; counter++){
    E0=1;
    _delay(2100);
    E0=0;
    _delay(42800);
    }
}

void Servo_End (void){
    for (counter=0; counter<8; counter++){
    E0=1;
     _delay(400);
    E0=0;
    _delay(42800);
    }
}



//=============Subrutinas Eliminación ruido============
//====================================================
void Antibounce (void){
unsigned char L1REG = 0; 
unsigned char L2REG; 
while(L1REG++ < 13){ 
    L2REG = 0; 
    while(L2REG++ < 255); 
    }
}
void One_Shot_N1 (void){
    if (ons_N1 == 1) // IF One-Shot has been triggered THEN EXIT
        return;
    Antibounce();
    if (PORTBbits.RB5==1){
       N1();
       ons_N1=0;
    }
}
void Reset_One_Shot_N1 (void){
    if (PORTBbits.RB5==0) // si RB5=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (PORTBbits.RB5==0) // si RB5=0 bandera one shot =0
     ons_N1 = 0;
}

void One_Shot_N2 (void){
    if (ons_N2 == 1) // IF One-Shot has been triggered THEN EXIT
        return;
    Antibounce();
    if (PORTBbits.RB6==1){
       N2();
       ons_N2=0;
    }
}
void Reset_One_Shot_N2 (void){
    if (PORTBbits.RB6==0) // si RB5=0 eliminar rebotes
        Antibounce();
    else // ELSE EXIT
    return;
    if (PORTBbits.RB6==0) // si RB5=0 bandera one shot =0
     ons_N2 = 0;
}

void One_Shot_N3 (void){
    if (ons_N3 == 1) // IF One-Shot has been triggered THEN EXIT
        return;
    Antibounce();
    if (PORTBbits.RB7==1){
       N3();
       ons_N3=0;
    }
}
void Reset_One_Shot_N3 (void){
    if (PORTBbits.RB7==0) // si RB7=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (PORTBbits.RB7==0) // si RB7=0 bandera one shot =0
     ons_N3 = 0;
}

void One_Shot_B1 (void){
    if (ons_B1 == 1) // IF One-Shot has been triggered THEN EXIT
        return;
    Antibounce();
    if (A0==1){
        C3=1;
        _delay(197000);
        C3=0;
        counter2++;
       ons_B1=0;
    }
}
void Reset_One_Shot_B1 (void){
    if (A0==0) // si boton1=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (A0==0) // si RB5=0 bandera one shot =0
     ons_B1 = 0;
   
}
void One_Shot_B2 (void){
    if (ons_B2 == 1) // IF One-Shot has been triggered THEN EXIT
    return;
    Antibounce();
    if (A1==1){
        C4=1;
        _delay(197000);
        C4=0;
        counter2++;
       ons_B2=0;
    }
}
void Reset_One_Shot_B2 (void){
    if (A1==0) // si boton1=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (A1==0) // si RB5=0 bandera one shot =0
     ons_B2 = 0;
    
}
void One_Shot_B3 (void){
    if (ons_B3 == 1) // IF One-Shot has been triggered THEN EXIT
    return;
    Antibounce();
    if (A2==1){
        C5=1;
        _delay(197000);
        C5=0;
       counter2++;
       ons_B3=0;
    }
}
void Reset_One_Shot_B3 (void){
    if (A2==0) // si boton1=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (A2==0) // si RB5=0 bandera one shot =0
     ons_B3 = 0;
    
}
void One_Shot_B4 (void){
    if (ons_B4 == 1) // IF One-Shot has been triggered THEN EXIT
    return;
    Antibounce();
    if (A3==1){
        C6=1;
        _delay(197000);
        C6=0;
        counter2++;
       ons_B4=0;
    }
}
void Reset_One_Shot_B4 (void){
    if (A3==0) // si boton1=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (A3==0) // si RB5=0 bandera one shot =0
     ons_B4 = 0;
   
}
void One_Shot_B5 (void){
    if (ons_B5 == 1) // IF One-Shot has been triggered THEN EXIT
    return;
    Antibounce();
    if (A4==1){
        C7=1;
        _delay(197000);
        C7=0;
        counter2++;
       ons_B5=0;
    }
}
void Reset_One_Shot_B5 (void){
    if (A4==0) // si boton1=0 eliminar rebotes
    Antibounce();
    else // ELSE EXIT
    return;
    if (A4==0) // si RB5=0 bandera one shot =0
     ons_B5 = 0;
   
}

//==============Nivel 1 Facil============================
//=======================================================
void N1 (void) {
    D7=0;
    LCDsetup();
    unsigned char n1;
    for(n1=0;n1<7;n1++){
    myRAMarray3 [n1] =  NIVEL1[n1]; //despliega la selección de dificultad
    PORTD=myRAMarray3[n1];
    LCDdisplay();
    LCDready();
 }
    for(n1=0; n1<10; n1++){
    _delay(197000);
    }
    PORTD= 0x01;// borrar display lcd
    Command ();
//******generar secuencia aleatoria********
    srand(t);
    unsigned char test= (rand()%5+1);
    acierto=0;
    unsigned char secuencia[2]; // se declara arrelgo de secuencia de leds
    unsigned char secuencia_gen[2]; //se declara arreglo de secuencia generada por el usuario
    while (acierto<3){ // ciclo hasta conseguir los aciertos
        for (counter=1 ; counter<=2; counter++){ //ciclo para generar el # de leds
            test = (rand()%5+1);
            if (test<=1){ ////si test = 1 prender led1 = C3
                PORTC=0b00001000;
                secuencia[counter]=1;
                PORTD= test+48;
                LCDdisplay();
                LCDready();
                for(n1=0; n1<10; n1++){
                    _delay(197000);
                }
            }
            else if(test<=2){ //si test = 2 prender led2 = C4
                PORTC=0b00010000;
                secuencia[counter]=2;
                PORTD= test+48;
                LCDdisplay();
                LCDready();
                for(n1=0; n1<10; n1++){
                    _delay(197000);
                }
            }
            else if(test<=3){ // si test = 3 prender led3 = C5         
                PORTC=0b00100000;
                secuencia[counter]=3;
                PORTD= test+48;
                LCDdisplay();
                 LCDready();
                for(n1=0; n1<10; n1++){
                   _delay(197000);
                }
            }
            else if(test<=4){ ////si test = 4 prender led4 = C6           
                PORTC=0b01000000;
                secuencia[counter]=4;
                PORTD= test+48;
                LCDdisplay();
                LCDready();
                for(n1=0; n1<10; n1++){
                    _delay(197000);
                }
            }
            else if(test<=5){ ////si test = 5 prender led5 = C7         
                PORTC=0b10000000;
                secuencia[counter]=5;
                PORTD= test+48;
                LCDdisplay();
                LCDready();
                for(n1=0; n1<10; n1++){
                    _delay(197000);
                }
            }
            PORTC=0; //apaga los leds entre cada ronda
            PORTD= 0x01;// borrar display lcd
            Command ();
            LCDready();
            _delay(197000);
        }
            for (counter2= 0; counter2<2;) { //ciclo para generar scuencia de botones
                if(A0==1){
                    One_Shot_B1();
                    if (ons_B1 == 1) {// IF One-Shot?s Trigger is ON THEN Reset-One-Shot
                    Reset_One_Shot_B1();
                    }
                    secuencia_gen[counter2]=1; //guardar #1 en la posición counter2 del arreglo
                    PORTD= secuencia_gen[counter2]+48; // mostrar en el lcd
                    LCDdisplay();
                     LCDready();
                     _delay(19700);
                    if(secuencia[counter2]!=secuencia_gen[counter2]){
                       counter=0;
                       intentos++;
                       break;
                    }
                }
                if(A1==1){
                    One_Shot_B2();
                    if (ons_B2 == 1){ // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
                        Reset_One_Shot_B2();
                    }
                    secuencia_gen[counter2]=2; //guardar #2 en la posición counter2 del arreglo
                    PORTD= secuencia_gen[counter2]+48; //mostrar en lcd
                    LCDdisplay();
                     LCDready();
                     _delay(19700);
                    if(secuencia[counter2]!=secuencia_gen[counter2]){
                        counter=0;
                        intentos++;
                        break;
                        } 
                    }
                if(A2==1){
                    One_Shot_B3();
                    if (ons_B3 == 1){ // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
                        Reset_One_Shot_B3();
                    }
                    secuencia_gen[counter2]=3; //guardar #3 en la posición counter2 del arreglo
                    PORTD= secuencia_gen[counter2]+48;//mostrar en lcd
                    LCDdisplay();
                     LCDready();
                     _delay(19700);
                    if(secuencia[counter2]!=secuencia_gen[counter2]){
                        counter=0;
                        intentos++;
                        break;
                    }
                }
                if(A3==1){
                    One_Shot_B4();
                    if (ons_B4 == 1){ // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
                        Reset_One_Shot_B4();
                    }
                    secuencia_gen[counter2]=4; //guardar #4 en la posición counter2 del arreglo
                    PORTD= secuencia_gen[counter2]+48; //mostrar en lcd
                    LCDdisplay();
                    LCDready();
                    _delay(19700);
                    if(secuencia[counter2]!=secuencia_gen[counter2]){
                        counter=0;
                        intentos++;
                        break;
                    }
                }
                if(A4==1){
                    One_Shot_B5();
                    if (ons_B5 == 1){ // IF One-Shot?s Trigger is ON THEN Reset-One-Shot
                        Reset_One_Shot_B5();
                    }
                    secuencia_gen[counter2]=5; //guardar #5 en la posición counter2 del arreglo
                    PORTD= secuencia_gen[counter2]+48; //mostrar en lcd
                    LCDdisplay();
                    LCDready();
                    _delay(19700);
                    if(secuencia[counter2]!=secuencia_gen[counter2]){
                        counter=0;
                        intentos++;
                        break;
                    }
                }
                PORTC=0b00000000;
            
                if (secuencia[1]==secuencia_gen[1]&& secuencia[2]==secuencia_gen[2]){
                    intentos++;
                    acierto++;
                    for(n1=0; n1<=3; n1++){
                        PORTC=0b1111000;
                        _delay(19700);
                         PORTC=0b00000000;
                    }    
                }
            
            }
        C3=0;
        C4=0;
        C5=0;
        C6=0;
        C7=0;
        PORTD= 0x01;// borrar display lcd
        Command ();
        LCDready();
        _delay(19700);
            
    }
    
    PORTD= 0x01;// borrar display lcd
    Command ();
    LCDready();
    Resultado(); //mostrar resultado

}

//==============Nivel 2 Medio============================
//=======================================================
void N2 (void) {
D7=0;
LCDsetup();
unsigned char n2;
for(n2=0;n2<7;n2++){
 myRAMarray4 [n2] =  NIVEL2[n2];
 PORTD=myRAMarray4[n2];
 LCDdisplay();
 LCDready();
 }
for(n2=0; n2<10; n2++){
_delay(197000);
}
   //******generar secuencia aleatoria******
    srand(t);
    unsigned char test= (rand()%5+1);
    counter=0;
    acierto=0;
    unsigned char secuencia[7]; 
    while (counter<=7){
        test = (rand()%5+1);
    if (test<=1){
        PORTC=0b00001000;
        secuencia[counter];
        for(n2=0; n2<8; n2++){
         _delay(197000);
        }
    }
    else if(test<=2){
        PORTC=0b00010000;
        secuencia[counter];
        for(n2=0; n2<8; n2++){
        _delay(197000);
        }
    }
    else if(test<=3){          
        PORTC=0b00100000;
        secuencia[counter];
        for(n2=0; n2<8; n2++){
        _delay(197000);
        }
    }
    else if(test<=4){            
        PORTC=0b01000000;
        secuencia[counter];
        for(n2=0; n2<8; n2++){
        _delay(197000);
        }
    }
    else if(test<=5){          
        PORTC=0b10000000;
        secuencia[counter];
        for(n2=0; n2<8; n2++){
        _delay(197000);
        }
    }
    counter++;
    PORTC=0; //apaga todos los leds entre cada ronda
    }
    Resultado(); //despliega el resultado
    PORTD= 0x01; //borrar el display lcd
    Command ();
    LCDready();
}

//==============Nivel 3 Dificil============================
//=========================================================
void N3 (void) {
D7=0;
LCDsetup();
unsigned char n3;
for(n3=0;n3<9;n3++){
 myRAMarray5 [n3] =  NIVEL3[n3];
PORTD=myRAMarray5[n3];
 LCDdisplay();
 LCDready();}
for(n3=0; n3<10; n3++){
_delay(197000);
}
//*******generar secuencia aleatoria********
    srand(t);
    unsigned char test= (rand()%5+1);
    counter=0;
    acierto=0;
    unsigned char secuencia[10]; 
    while (counter<=10){
        test = (rand()%5+1);
    if (test<=1){
        PORTC=0b00001000;
        secuencia[counter];
        for(n3=0; n3<6; n3++){
         _delay(197000);
        }
    }
    else if(test<=2){
        PORTC=0b00010000;
        secuencia[counter];
        for(n3=0; n3<6; n3++){
        _delay(197000);
        }
    }
    else if(test<=3){          
        PORTC=0b00100000;
        secuencia[counter];
        for(n3=0; n3<6; n3++){
        _delay(197000);
        }
    }
    else if(test<=4){            
        PORTC=0b01000000;
        secuencia[counter];
        for(n3=0; n3<6; n3++){
        _delay(197000);
        }
    }
    else if(test<=5){          
        PORTC=0b10000000;
        secuencia[counter];
        for(n3=0; n3<6; n3++){
        _delay(197000);
        }
    }
    counter++;
    PORTC=0; 
    }
    
    Resultado();
    PORTD= 0x01; //borrar el display lcd
    Command ();
    LCDready();
}

//===============Display Resultados===============
//================================================
void Resultado (void){
D7=0;
LCDsetup();
unsigned char R;
for(R=0;R<15;R++){
 myRAMarray6 [R] = RESULTADO [R];
 PORTD=myRAMarray6[R];
 LCDdisplay();
 LCDready();}
PORTC=0b11111000; //prender todos los leds
Servo_In();
for(R=0; R<10; R++){
_delay(197000);
 PORTD= 0x01;// borrar display lcd
 Command ();
 LCDready();
 score[2]= acierto/intentos;
 if ((acierto/intentos)<1){
     for(R=1; R<3; R++){ 
        PORTD= score[R]+48;
        LCDdisplay();
        LCDready();
     }
 }
}
 


}
