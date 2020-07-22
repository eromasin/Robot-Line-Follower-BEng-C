// Cameron Coats / Group 3
// EN0627 / NXP Car
// 
// Periodic Interrupt Timer Driver
#include "MKL25Z4.h"
#include "PITDriver.h"
extern void startImageCapture(void);

void initPIT(){

  
  // Disable all interrupts before configuring
  __disable_irq();   
	
	// Disable IRQ 22 (PIT)
	NVIC->ICER[0] = 1 << 22; 

	// Enable PIT Clock
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Turn on PIT
	PIT_MCR = 0x00;	
	
  // Set start value - doesn't matter as we're using autoexposure
	// needs to be longer than ~5ms so that the foreground task will run
	// (image capture takes 5ms)
	PIT_LDVAL0 = 419400;
	
	// Enable clock & freeze timer when debugging
	PIT_MCR = PIT_MCR_FRZ_MASK;
	
	// Enable interrupt & Start timer
	PIT_TCTRL0 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
 
	// Enable IRQ 22 (PIT) once configured
	NVIC->ISER[0] |= 1u<<22; 
		
	// Enable all interrupts
  __enable_irq();
	
}


void PIT_IRQHandler(){
	// Disable PIT
	PIT_TCTRL0 = 0; 
	// Clear timer overflow interrupt
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
	
	startImageCapture();
	runMainLoop = true;
	
	// Start timer again
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
}
