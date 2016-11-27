#include <xc.h>
#include <p18f4585.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define _XTAL_FREQ 4000 //para poder utilizar delays con el compilador

void main(void)
{
	//VARIABLE'S DECLARATION
	int round = 0; //USED FOR COUNTING EACH ROUND
	int vector = 0; //USED TO READ EACH ARRAY'S POSITION
	int general = 0; //USED FOR GENERAL PURPOSES
	int general2 = 0; //SECOND VARIABLE USED FOR GENERAL PURPOSES
	int pulsed = 0; //USED TO COUNT HOW MANY TIMES THE BUTTONS ARE PULSED
	int restart = 0; //USED TO RESTART THE GAME WHEN PLAYER LOOSES
	int button = 0; // USED TO STORE THE PULSED BUTTON
	int sequence[40]; //USED TO STORE THE OUPUTS OF EACH ROUND
    int gn=0;
	
	//RB7, RB6, RB5, RB4 AS OUTPUT. RB3, RB2, RB1, RB0 AS INPUT
	TRISB = 0x0F;
	//RA3, RA2, RA1, RA0 AS OUTPUT
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA1 = 0;
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA3 = 0;
	
	//************************************************
	//INITIAL SOUND AND LIGHT
	for(pulsed=0;pulsed<3;pulsed++)
	{
		//************************************************
		for(general=0;general<50;general++)
		{		
			PORTB=0x10;
            gn=60-general;
			_delay(gn);
			PORTB=0b00000000;
			_delay(gn);
			PORTA=0x0F; //SETTING ALL PORTA HIGH
		}
		//************************************************
	}
	
	//************************************************
	
	PORTA=0x00; //SETTING ALL PORTA LOW
	pulsed = 0;	
		
		//************************************************
		//INFINITE LOOP - OPEN
		while(1)
		{	
			//************************************************
			//EMPTIYNG ALL ROUNDS. I SUPPOSE 40 ROUNDS IS AN UNREACHEABLE RECORD :P
			for(round=0;round<40;round++)
			{
				sequence[round] = 0;
			}
			//************************************************
			
			//************************************************
			//LIGHTS EFFECT. WAITING FOR A BUTTON TO START A NEW GAME
			while(!PORTB)
			{
				PORTA=0x01;
				_delay(30000);
				PORTA=0x02;
                _delay(30000);
				PORTA=0x04;
				_delay(30000);
				PORTA=0x08;
				_delay(30000);
			}
			//************************************************
			PORTA=0x00; //SETTING ALL PORTA LOW
			_delay(151000); //INITIAL DELAY
			round = 0;

			//************************************************
			//40 ROUND LOOP - OPEN
			while(round < 40)
			{
				PORTA=0x00; // SETTING ALL PORTA LOW
				general = 0;
				
				//RESTART THE GAME WHEN PLAYER LOOSES
				if(restart == 2)
				{
					round = 0;
					restart = 0;
					vector = 0;
					pulsed = 0;
					break; //RESTART GAME
				}
					
			//THIS CODE GENERATES THE LIGHTS AND SOUNDS THAT THE PLAYER MUST FOLLOW
					
					//************************************************
					// IMPROVEMENT OF RANDOM NUMBER GENERATOR
					general2 = (rand() % 9) + 1;
					for(general = 0; general < general2;general++)
					{
						sequence[round]=rand() % 4;
					}
					//************************************************
					
					vector++;
					//************************************************
					//CHECK EACH ARRAY'S ELEMENT - OPEN
					for(round = 0;round < vector;round++)
					{
						//SWITCH TO DETERMINE WHICH PORT SET TO HIGH
						switch(sequence[round])
						{
							case 0:
							//************************************************
							//THIS CODE GENERATES SOUND AND LIGHT AT PORT RA0
							for(general = 0;general < 150;general++)
							{	
									
								PORTB=0x10;
								_delay(2200);
								PORTB=0b00000000;
								_delay(2200);
								PORTA=0x01;
							}
							//************************************************
							break;
		
							case 1:
							//************************************************
							//THIS CODE GENERATES SOUND AND LIGHT AT PORT RA1
							for(general = 0;general < 180;general++)
							{	
									
								PORTB=0x20;
								_delay(1800);
								PORTB=0b00000000;
								_delay(1800);
								PORTA=0x02;
							}
							//************************************************
							break;
		
							case 2:
							//************************************************
							//THIS CODE GENERATES SOUND AND LIGHT AT PORT RA2
							for(general = 0;general < 227;general++)
							{	
									
								PORTB=0x10;
								_delay(1400);
								PORTB=0x00;
								_delay(1400);
								PORTA=0x04;
							}
							//************************************************
							break;
		
							case 3:
							//************************************************
							//THIS CODE GENERATES SOUND AND LIGHT AT PORT RA3
							for(general = 0;general < 260;general++)
							{	
									
								PORTB=0x10;
								_delay(1200);
								PORTB=0x00;
								_delay(1200);
								PORTA=0x08;
							}
							//************************************************
							break;
						}
						PORTA=0x00; // SET PORTA TO LOW
						_delay(80000); // DELAY BETWEEN SOUNDS
					
					} //CHECK EACH ARRAY'S ELEMENT - CLOSE
					//************************************************
					
					round++; //NEXT ROUND COUNTER
					
					//************************************************
					// NO PULSED BUTTON LOOP - OPEN
					while(!PORTB)
					{
						PORTA=0x00; // SET PORTA TO LOW
					
					} // NO PULSED BUTTON LOOP - CLOSE
					//************************************************
					
					_delay(10);
					pulsed=0;
					restart=0;
					
					//************************************************
					// PULSED BUTTONS LOOP - OPEN
					while(1)
					{
						PORTA=0x00; //SET PORTA TO LOW
						// RESTART THE GAME
						if(restart == 2)
						{
							break;
						}
						
						if(restart == 1)
						{
							pulsed++;
							restart = 0;
							//IF PULSED IS GREATHER THAN ROUND, EXIT LOOP
							if(pulsed == (round - 1))
							{
								//************************************************
								// OK SOUND
								_delay(50000);
								for(general = 0; general < 30; general++)
								{
									PORTB=0x10;
									_delay(100*(35-general));
									PORTB=0x00;
									_delay(100*(35-general));
								}
								break;
							}
						}
						
						//************************************************
						// WHICH BUTTON IS PULSED LOOP - OPEN
						while(PORTB)
						{
							// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB3
							if(PORTBbits.RB3 == 1)
							{
								PORTB=0x10;
								_delay(1160);
								PORTB=0b00000000;
								_delay(1160);
								PORTA=0x08;
								button = 3;
							}
							
							// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB2
							if(PORTBbits.RB2 == 1)
							{
								PORTB=0x10;
								_delay(1360);
								PORTB=0x00;
								_delay(1360);
								PORTA=0x04;
								button = 2;
							}
							
							// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB1
							if(PORTBbits.RB1 == 1)
							{
								PORTB=0x10;
								_delay(1760);
								PORTB=0x00;
								_delay(1760);
								PORTA=0x02;
								button = 1;
							}
					
							// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB0
							if(PORTBbits.RB0 == 1)
							{
								PORTB=0x10;
								_delay(2160);
								PORTB=0x00;
								_delay(2160);
								PORTA=0x01;
								button = 0;
							}
						
							//CHECK IS PULSED BUTTON IS THE RIGHT OPTION
							if(sequence[pulsed] != button)
							{
								//SHOWS THE CORRECT OPTION
								restart = 2;
								vector=0;
								PORTA=0x00; //SET PORTA TO LOW
								pulsed == (round - 2); //EXIT LOOP
								
								// SWITCH TO SHOW TO THE PLAYER THE CORRECT OPTION, WITH THE WRONG SOUND
								switch(sequence[pulsed])
								{
									case 0:
									//************************************************
									// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB0
									for(general = 0;general < 175;general++)
									{	
											
										PORTB=0x10;
										_delay(6400);
										PORTB=0x00;
										_delay(6400);
										PORTA=0x01;
									}
									//************************************************
									break;
				
									case 1:
									//************************************************
									// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB1
									for(general = 0;general < 175;general++)
									{	
											
										PORTB=0x10;
										_delay(6400);
										PORTB=0x00;
										_delay(6400);
										PORTA=0x02;
									}
									//************************************************
									break;
				
									case 2:
									//************************************************
									// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB2
									for(general = 0;general < 175;general++)
									{	
											
										PORTB=0x10;
										_delay(6400);
										PORTB=0x00;
										_delay(6400);
										PORTA=0x04;
									}
									//************************************************
									break;
				
									case 3:
									//************************************************
									// THIS CODE GENERATES SOUND AND LIGHT FOR BUTTON RB3
									for(general = 0;general < 175;general++)
									{	
											
										PORTB=0x10;
										_delay(6400);
										PORTB=0x00;
										_delay(6400);
										PORTA=0x08;
									}
									//************************************************
									break;
								}
							}
							else
							{
								restart = 1;
							}
							
						} // WHICH BUTTON IS PULSED LOOP - CLOSE
						//************************************************
					
					} // PULSED BUTTONS LOOP - CLOSE
					//************************************************
					_delay(100000); // NEW ROUND DELAY
		
			} //40 ROUND LOOP - CLOSE
			//************************************************	
		
		} //INFINITE LOOP - CLOSE
		//************************************************
		
}
//************************************************