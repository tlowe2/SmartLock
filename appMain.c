/*
===============================================================================
 Name        : appMain.c
 Author      : Teddy Lowe, Joseph Jennings
===============================================================================
*/


#include "LPC11Uxx.h"
#include "clkconfig.h"
#include "gpio.h"
#include "uart.h"


//extern volatile uint32_t timer16_1_counter[2];
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];
extern volatile int character;
volatile uint32_t timer32_0_counter[4];


//Password information
uint8_t password[5];
uint8_t passBuffer[5];
int pwok = 0;
int pwWait = 0;

//variables to choose
int time;
int stop = 4200;
uint32_t interval;
int state = 1; //1 is open

void lockControl(int dir){

	// dir = 1 open, dir = 0 close, else return and do nothing
	if(dir == 1){
		time = 100;
	}else if(dir == 0){
		time = 15;
	}else{
		return;
	}

	GPIOSetBitValue( 0, 7, 1 );
	// Generate PWM signal for opening/closing the lock on pin 0_9
	timer32_0_counter[0] = 0;
	timer32_0_counter[1] = 0;
	while (1){
		if ( (timer32_0_counter[0] >= 0) && (timer32_0_counter[0] <= time/2) )
		{
		  GPIOSetBitValue( 0, 9, 0 );
		}
		if ( (timer32_0_counter[0] > time/2) && (timer32_0_counter[0] <= time) )
		{
		  GPIOSetBitValue( 0, 9, 1 );
		}
		else if ( timer32_0_counter[0] > time )
		{
		  timer32_0_counter[0] = 0;
		}

		if (timer32_0_counter[1] >= stop){
			GPIOSetBitValue( 0, 7, 0 );
			return;
		}
	}
}

//
// Recieve password and change or don't
int passwordRecieve(int mode){

	int i, j, correct;
	while (UARTCount < 5);

	for (i = 0; i <= 4; i++){
		passBuffer[i] = UARTBuffer[i];
		if (passBuffer[i] != password[i]){
			return 0;
		}
	}

	if (!mode){
		return 1;
	}

	UARTCount = 0;
	while (UARTCount < 5);
	for (i = 0; i <= 4; i++){
			password[i] = UARTBuffer[i];
		}
	return 1;

}

void TIMER32_0_IRQHandler(void) {

	if ( LPC_CT32B0->IR & (0x01<<0) )
	  {
		timer32_0_counter[0]++; //increment frequency use counter
		timer32_0_counter[1]++; //increment ten second counter
		LPC_CT32B0->IR = 0x1<<0;			/* clear interrupt flag */
	  }

	return;
 }

/* TIMER32 and TIMER32 Interrupt Initialization */
void TIMERInit() {

	volatile uint32_t i;

	/* Enables Clock to timer32_0 */
	 LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

	 /* Set match register with value for interrupts*/
	 LPC_CT32B0->MR0 = interval;

	/* Set all timer values to 0 */
  	for ( i = 0; i < 4; i++ )
	{
      timer32_0_counter[i] = 0;
	}

  	/* Enable MAT0 to be used for the timer, pull up resistor enabled*/
	LPC_IOCON->PIO1_24           &= ~0x07;
	LPC_IOCON->PIO1_24           |= 0x01;		/* Timer0_32 MAT0 */

  	/* Clear External Match Register */
	LPC_CT32B0->EMR &= ~(0xFF<<4);

	/* MR0/1/2/3 Toggle CHECK THIS SHIT */
	LPC_CT32B0->EMR |= ((0x3<<4)|(0x3<<6)|(0x3<<8)|(0x3<<10));

	/* Tell match control register Interrupt and Reset on MR0 */
	LPC_CT32B0->MCR = 3;

	NVIC_SetPriority(TIMER_32_0_IRQn,3);

	 /* Enables timer interrupt */
	 NVIC_EnableIRQ(TIMER_32_0_IRQn);

	 /* Enables timer32_0*/
	 LPC_CT32B0->TCR = 1;

	 return;

 }


int main(void) {

	SystemCoreClockUpdate();

	int i;

	password[0] = '1';
	password[1] = '2';
	password[2] = '3';
	password[3] = '4';
	password[4] = '5';

	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);

	interval = SystemCoreClock/10000 - 1;


	TIMERInit();
	UARTInit(9600);

	GPIOSetDir( 0, 9, 1 );
	GPIOSetDir( 0, 7, 1 );


	while (1){
		if (character){
			character = 0;
			// Not waiting for password
			if (!pwWait){
				switch (UARTBuffer[0]){

				// State request
				case 's':
					UARTBuffer[0] = 0;
					UARTCount = 0;
					if (state){
						UARTSend( (uint8_t *)"1",1);
					}else{
						UARTSend( (uint8_t *)"0",1);
					}
					break;

				// Lock Request
				case '0':
					UARTBuffer[0] = 0;
					UARTCount = 0;
					// Already Locked
					if (!state){
						UARTSend( (uint8_t *)"0",1);
					}else{
						// Password?
						if (pwok == 1){
							lockControl(0);
							state = 0;
							pwok = 0;
							UARTSend( (uint8_t *)"0",1);
						}else{
							UARTSend( (uint8_t *)"p",1);
							pwWait = 1;
						}
					}
					break;

				// Unlock Request
				case '1':
					UARTBuffer[0] = 0;
					UARTCount = 0;
					// Already Unlocked
					if (state){
						UARTSend( (uint8_t *)"1",1);
					}else{
						// Password?
						if (pwok == 1){
							lockControl(1);
							state = 1;
							UARTSend( (uint8_t *)"1",1);
						}else{
							UARTSend( (uint8_t *)"p",1);
							pwWait = 1;
						}
					}
					break;

				// Change password request
				case 'c':
					UARTBuffer[0] = 0;
					UARTCount = 0;
					pwok = passwordRecieve(1);
					if (pwok){
						UARTSend( (uint8_t *)"c",1);
					}else{
						UARTSend( (uint8_t *)"x",1);
					}
					break;

				default:
					UARTBuffer[0] = 0;
					UARTCount = 0;
				}

			}

			// Waiting for password
			else if (pwWait){
				switch (UARTBuffer[0]){

				// Ready to send password
				case 'p':
					UARTBuffer[0] = 0;
					UARTCount = 0;
					pwok = passwordRecieve(0);
					// Good password
					if (pwok){
						UARTSend( (uint8_t *)"g",1);
						pwWait = 0;
					}
					// Bad password
					else{
						UARTSend( (uint8_t *)"x",1);
					}
					break;

				// 'Cancel'
				case 's':
					UARTBuffer[0] = 0;
					UARTCount = 0;
					pwWait = 0;
					break;

				default:
					UARTBuffer[0] = 0;
					UARTCount = 0;
					UARTSend( (uint8_t *)"p",1);
				}
			}
		}
	}

}
