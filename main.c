///*
//===============================================================================
// Name        : main.c
// Author      : $(author)
// Version     :
// Copyright   : $(copyright)
// Description : main definition
//===============================================================================
//*/
//
//
//#include "LPC11Uxx.h"
////#include "timer16.h"
//#include "clkconfig.h"
//#include "gpio.h"
//#include "uart.h"
//
//
////extern volatile uint32_t timer16_1_counter[2];
//extern volatile uint32_t UARTCount;
//extern volatile uint8_t UARTBuffer[BUFSIZE];
//extern volatile int character;
//volatile uint32_t timer32_0_counter[4];
//
//
////variables to choose
//int time;
//int stop = 4200;
//uint32_t interval;
//int state = 1; //1 is open
//
//void lockControl(int dir){
//
//	// dir = 1 open, dir = 0 close, else return and do nothing
//	if(dir == 1){
//		time = 100;
//	}else if(dir == 0){
//		time = 15;
//	}else{
//		return;
//	}
//
//
//	// Generate PWM signal for opening/closing the lock on pin 0_9
//	timer32_0_counter[0] = 0;
//	timer32_0_counter[1] = 0;
//	while (1){
//		if ( (timer32_0_counter[0] >= 0) && (timer32_0_counter[0] <= time/2) )
//		{
//		  GPIOSetBitValue( 0, 9, 0 );
//		}
//		if ( (timer32_0_counter[0] > time/2) && (timer32_0_counter[0] <= time) )
//		{
//		  GPIOSetBitValue( 0, 9, 1 );
//		}
//		else if ( timer32_0_counter[0] > time )
//		{
//		  timer32_0_counter[0] = 0;
//		}
//
//		if (timer32_0_counter[1] >= stop){
//			return;
//		}
//	}
//}
//
//void TIMER32_0_IRQHandler(void) {
//
//	if ( LPC_CT32B0->IR & (0x01<<0) )
//	  {
//		timer32_0_counter[0]++; //increment frequency use counter
//		timer32_0_counter[1]++; //increment ten second counter
//		LPC_CT32B0->IR = 0x1<<0;			/* clear interrupt flag */
//	  }
//
//	return;
// }
//
///* TIMER32 and TIMER32 Interrupt Initialization */
//void TIMERInit() {
//
//	volatile uint32_t i;
//
//	/* Enables Clock to timer32_0 */
//	 LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);
//
//	 /* Set match register with value for interrupts*/
//	 LPC_CT32B0->MR0 = interval;
//
//	/* Set all timer values to 0 */
//  	for ( i = 0; i < 4; i++ )
//	{
//      timer32_0_counter[i] = 0;
//	}
//
//  	/* Enable MAT0 to be used for the timer, pull up resistor enabled*/
//	LPC_IOCON->PIO1_24           &= ~0x07;
//	LPC_IOCON->PIO1_24           |= 0x01;		/* Timer0_32 MAT0 */
//
//  	/* Clear External Match Register */
//	LPC_CT32B0->EMR &= ~(0xFF<<4);
//
//	/* MR0/1/2/3 Toggle CHECK THIS SHIT */
//	LPC_CT32B0->EMR |= ((0x3<<4)|(0x3<<6)|(0x3<<8)|(0x3<<10));
//
//	/* Tell match control register Interrupt and Reset on MR0 */
//	LPC_CT32B0->MCR = 3;
//
//	NVIC_SetPriority(TIMER_32_0_IRQn,3);
//
//	 /* Enables timer interrupt */
//	 NVIC_EnableIRQ(TIMER_32_0_IRQn);
//
//	 /* Enables timer32_0*/
//	 LPC_CT32B0->TCR = 1;
//
//	 return;
//
// }
//
//
//int main(void) {
//
//	SystemCoreClockUpdate();
//
////	CLKOUT_Setup (CLKOUTCLK_SRC_MAIN_CLK);
////	LPC_IOCON->PIO0_1 &= ~0x07;
////	LPC_IOCON->PIO0_1 |= 0x01;		/* CLK OUT */
////
//
//	/* Enable AHB clock to the GPIO domain. */
//	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
//	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
//
//	interval = SystemCoreClock/10000 - 1;
////	init_timer16(1, interval);
////	enable_timer16(1);
//
//	TIMERInit();
//	UARTInit(9600);
//
//	GPIOSetDir( 0, 9, 1 );
//
////	UARTSend( (uint8_t *)"\r\n--Lock Menu--\r\n",17);
////	UARTSend( (uint8_t *)"1. Lock OPEN\r\n",14);
////	UARTSend( (uint8_t *)"2. Lock CLOSE\r\n",15);
//
//	while (1){
//		if (character){
//			character = 0;
//			switch (UARTBuffer[0]){
//			case '1':
//				UARTBuffer[0] = 0;
//				UARTCount = 0;
//				if (state){
//					UARTSend( (uint8_t *)"\r\nAlready OPEN\r\n",16);
//				}else{
//					lockControl(1);
//					state = 1;
//					UARTSend( (uint8_t *)"\r\nOPEN\r\n",8);
//				}
//				break;
//			case '2':
//				UARTBuffer[0] = 0;
//				UARTCount = 0;
//				if (!state){
//					UARTSend( (uint8_t *)"\r\nAlready CLOSED\r\n",18);
//				}else{
//					lockControl(0);
//					state = 0;
//					UARTSend( (uint8_t *)"\r\nCLOSED\r\n",10);
//				}
//				break;
//			default:
//				UARTBuffer[0] = 0;
//				UARTCount = 0;
//				UARTSend( (uint8_t *)"\r\nGo away\r\n",11);
//			}
////			UARTSend( (uint8_t *)"\r\n--Lock Menu--\r\n",17);
////			UARTSend( (uint8_t *)"1. Lock OPEN\r\n",14);
////			UARTSend( (uint8_t *)"2. Lock CLOSE\r\n",15);
//		}
//	}
//
//}
