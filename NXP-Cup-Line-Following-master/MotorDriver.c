// Aldo Hasani / Group 3
// EN0627 / NXP Car
// 
// Motor Driver

#include "MKL25Z4.h"
#include "MotorDriver.h"
 
 void initMotors(void){
				// MOTOR A - C1 (PWM), C2
				// MOTOR B - C3 (PWM),C4
				// ENABLE - E21
				// FAULT - E20
				// Motor B In2 -> GPIO
				PORTC->PCR[4] = 0x100;
				// Motor A In2 -> GPIO
				PORTC->PCR[2] = 0x100;
				// Enable Pin -> GPIO
				PORTE->PCR[21] = 0x100;
				// Motor A In2 -> TPM0CH2
				PORTC->PCR[3] = 1<<10;
				// Motor B In2 -> TPM0CH0
				PORTC->PCR[1] = 1<<10;			

				// TIMER 0 COMPARE 0 -> Motor A
				 // Edge-aligned PWM
				 // Mode = 1,0 , ELS = 0,1 , 
        TPM0->CONTROLS[0].CnSC = (1<<5) | (1<<2);
        // Ensure overflow flag is not set
        TPM0->CONTROLS[0].CnSC |= 1<<7;
        // Preload output compare
        TPM0->CONTROLS[0].CnV = POT1_Value * 4;
				
				// TIMER 0 COMPARE 2 -> Motor B
        // Toggle pin on output compare
        TPM0->CONTROLS[2].CnSC =  (1<<5) | (1<<2);
        // Ensure 0verflow flag is not set
        TPM0->CONTROLS[2].CnSC |= 1<<7;
        // Preload output compare
        TPM0->CONTROLS[2].CnV = POT1_Value * 4;
				
        // Enable timer 0 once configured
        TPM0->SC |= 1<<3; // Set CMOD = 0,1 "LPTPM counter increments on every LPTPM counter clock"
				
				FPTC->PDDR |= 1<<2 | 1<<4; // PTC2,4 output (In2 both motors)
				FPTE->PDDR |= 1<<21;	// PTE21 output (Enable)
				FPTC->PCOR |= 1<<2 | 1<<4; // C1,3 low
				FPTE->PCOR |= 1<<21; // Enable low at startup
}
 
void enableMotors(){
	FPTE->PSOR |= 1<<21;
	sendString("Motors Enabled!");
}
	
void disableMotors() {
	FPTE->PCOR |= 1<<21;
		sendString("Motors Disabled!");
}

void setTorqueVectors(int linePos){
	// DISABLED TORQUE VECTORING HERE
	if(/*linePos > -3 && linePos < 3*/ true){
// LEFT MOTOR
TPM0->CONTROLS[0].CnV = POT1_Value * 4;
// RIGHT MOTOR
TPM0->CONTROLS[2].CnV = POT1_Value * 4;
	}
	else
	{ 
	TPM0->CONTROLS[2].CnV = (POT1_Value * 3) - (linePos * TORQUEVECTOR_AMT);
	TPM0->CONTROLS[0].CnV = (POT1_Value * 3) + (linePos * TORQUEVECTOR_AMT);
	}
}
void setSpeed(char speed){
	TPM0->CONTROLS[2].CnV = (speed * 163);
	TPM0->CONTROLS[0].CnV = (speed * 163);
}
