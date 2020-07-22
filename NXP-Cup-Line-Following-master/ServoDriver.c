// Glen Dale / Group 3
// EN0627 / NXP Car
//
// Servo Driver using Timer 1

#include "MKL25Z4.h"
#include "ServoDriver.h"

void initServo(void){
	SIM ->SCGC6 |= SIM_SCGC6_TPM1_MASK; // enable clock to TPM1
	SIM->SOPT2 |= 1U<<24;    // Use MCGFLICK as timer counter clock
	TPM1->SC = 0;         // Disable timer
	TPM1->CONTROLS[0].CnSC = 0x20 | 0x08;   // centre -> pulse high
	TPM1->MOD = 52425 ;   //controls the frequency to 50Hhz, value must be less than 65536. Sets 20ms timer
	TPM1->CONTROLS[0].CnV = CENTER_POSITION;  //Set the chanel Value for 7.5% Duty Cycle (Straight)
	TPM1->SC = 0x0A | 0x20 ;  //enable TPM1 with prescaler 8, center aligned
}

void setSteeringAngle(int linePos)
{
		TPM1->CONTROLS[0].CnV= CENTER_POSITION + (linePos * (POT0_Value / 128));
}
void setCenter(void){
	TPM1->CONTROLS[0].CnV= CENTER_POSITION;
}
