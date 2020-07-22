// Cameron Coats / Group 3
// EN0627 / NXP Car
// 
// Hardware Timer Driver

#include "MKL25Z4.h"
#include "TimerDriver.h"


void initTimer0(void){
	// Enable clock
  SIM->SCGC6 |= ClockGating_Timer0; 
	// Set clock source
	SIM->SOPT2 |= ClockSource_MCGFLL; 
	// Disable timer before configuring
	TPM0->SC = 0;
	// Maximum modulo until otherwise configured
	TPM0->MOD = 16384;
	// Clear Flag
	TPM0->SC &= TOF_Mask; 
}

void Timer0_DelayuS(int time)
{
  short unsigned int t;	
	
	//Figure out how many clocks we need for for the delay time
	t=(time*20) + TPM0_CNT; //MOD value for delay
	while(TPM0_CNT < t) { } // wait until the TOF is set
}
void Timer0_DelaymS(int time)
{
	int i;
	for(i = 0;i<time;i++){
	while((TPM0->SC & TOF_Mask) == 0) { } // wait until the TOF is set
	TPM0->SC &= !TOF_Mask;
}
}
