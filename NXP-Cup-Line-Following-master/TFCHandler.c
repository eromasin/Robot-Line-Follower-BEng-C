// Cameron Coats / Group 3
// EN0627 / NXP Car
// 
// TFC Shield Handler

#include "MKL25Z4.h"

void initPorts(void){
  // enable clock to PORT A, B, C, D, E 
  SIM->SCGC5 |= ((1U<<9)|(1U<<10)|(1U<<11)|(1U<<12)|(1U<<11));
	// SW1 / C13, GPIO
	PORTC->PCR[13]=(1U << 8);
	// SW2 / C17, GPIO
	PORTC->PCR[17]=(1U << 8); 
	// Disable interrupts (commented out), we will poll these
	//PORTC->ISFR &= (1U << 13);
	//PORTC->ISFR &= (1U << 17);
	// Set data direction
	GPIOC->PDDR &=  (0U << 13);
	GPIOC->PDDR &=  (0U << 17);
	
	// Servo - PTB0
	// TPM1CH0 -> ALT 3
	PORTB->PCR[0] = 1U<<8 | 1U<<9;
	GPIOB->PDDR |=  1U;
	
	// Shield LEDs - B8,9,10,11
	PORTB->PCR[8] = 1U<<8;
	PORTB->PCR[9] = 1U<<8;
	PORTB->PCR[10] = 1U<<8;
	PORTB->PCR[11] = 1U<<8;
	GPIOB->PDDR |= 1U<<8 | 1U<<9 | 1U<<10 | 1U<<11;
}

void setLEDs(unsigned short int value){
	// all LEDs off
	PTB->PCOR |= 1U<<8 | 1U<<9 | 1U<<10 | 1U<<11;
	
	// map battery voltage to LEDs
	

	if(value > 1625 && value < 1690){ PTB->PSOR |= 1U<<11;}
	else if(value > 1690 && value < 1765){ PTB->PSOR |= 1U<<11 | 1U<<10;}
	else if(value > 1765 && value < 1840){ PTB->PSOR |= 1U<<11 | 1U<<10 | 1U<<9;}
	else if(value >= 1840){ PTB->PSOR |= 1U<<8 | 1U<<9 | 1U<<10 | 1U<<11;}
}
