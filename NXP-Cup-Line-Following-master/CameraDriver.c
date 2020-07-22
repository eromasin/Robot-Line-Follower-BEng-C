// Cameron Coats / Group 3
// EN0627 / NXP Car
//
// Linescan Camera Driver (Low Level)

#include "MKL25Z4.h"
#include "CameraDriver.h"
#include <stdbool.h>

// set default state
CameraST CameraState = IDLE;

void initCamera(void){
		// Enable Port Clock
		SIM->SCGC5 |=(ClockGating_PortD | ClockGating_PortE);

		// D7 -> Cam0 SI
    PORTD->PCR[7] = PinMux_GPIO;
    PTD->PDDR |= (0x1<<7);

		// E1 -> Cam0 CLK
    PORTE->PCR[1] = PinMux_GPIO;
    PTE->PDDR |= (0x1<<1);

		// C5 -> Cam0 AO
		PORTD->PCR[5] = PinMux_ADC;

		// Enable ADC Clock
	  SIM->SCGC6 |= ClockGating_ADC0;

		// Disable Interrupts
		ADC0->SC1[0] &= ~EnableInterrupts;

		// Single Ended Set Channel 31 (Disable)
	  ADC0->SC1[0] |= SingleEnded | ADC_SC1_ADCH(31);

		// Software Trigger (When channel is set)
		ADC0->SC2 &= SoftTriggerMask;

		ADC0->CFG1 =(ClockDiv_4 | SampleTme_Long | SingleEndedBusClk);

		// B Side of Input Mux
		ADC0->CFG2 |= ADC0Mux_BSide;
}

void initCameraDriver(){
	// Disable all interrupts before configuring
  __disable_irq();

	// Disable IRQ 15 (Camera)
	NVIC->ICER[0] = 1 << 15;

	initCamera();

	// Enable IRQ 15 (Camera) once configured
	NVIC->ISER[0] |= 1u<<15;

	// Enable all interrupts
  __enable_irq();

	// Set initial state
	CameraState = IDLE;

}

void startImageCapture(void){
	unsigned short int l;
	short unsigned int delTimer;

	//Start Integrating
	SI_H;
	// Wait
	for(delTimer = 0;delTimer<64;delTimer++) {}
	// First Clock Cycle
	Clk_H;
	// Wait
	for(delTimer = 0;delTimer<64;delTimer++) {}
	// SI Falling edge
	SI_L;

	// Reset pixel count
	PixelCounter = 0;


	// B Side of Input Mux
	ADC0->CFG2 |= ADC0Mux_BSide;
	// Enable IRQ 15 (Camera) once configured
	NVIC->ISER[0] |= 1u<<15;
	// Enable coco interrupt & set channel to start conversion
	ADC0_SC1A=  EnableInterrupts | ADC_SC1_ADCH(CAMERA0_ADCCHANNEL);

	CameraState = LINESCAN;
}

void ADC0_IRQHandler(void){
	short unsigned int delTimer;

		switch(CameraState){
			case IDLE:
				// do nothing
			break;
			case POT0:

				// record pot value
		    POT0_Value = ADC0_RA;

				// Set PIT start value
			  // This is disabled because we now have autoexposure
				// this is POT 0 * 384, at a maximum value of 4096 and timer interval of 10ns this is ~0-200ms
				//PIT_LDVAL0 = POT0_Value * 384;

				// set next state
				CameraState = POT1;

				// change ADC Channel and start ADC
			  ADC0_SC1A =  EnableInterrupts | ADC_SC1_ADCH(POT1_ADCCHANNEL);

			break;
			case POT1:

			  // record pot value
		    POT1_Value = ADC0_RA;


				// set next state
				CameraState = BATT;

				// change ADC Channel and start ADC
			  ADC0_SC1A =  EnableInterrupts | ADC_SC1_ADCH(BAT_ADCCHANNEL);
			break;
			case BATT:
				// record pot value
		    BATT_Value = ADC0_RA;

				// Send VBAT to leds
				setLEDs(BATT_Value);
				// set next state
				CameraState = IDLE;


			break;
			case LINESCAN:
				if(PixelCounter<128)
	     {
				// record pixel value
		    image[volatileBuffer][PixelCounter] = ADC0_RA;

		    // go to next pixel
		    PixelCounter++;

			 // clock pulse to get next pixel from camera
	     Clk_L;
		   for(delTimer = 0;delTimer<64;delTimer++) {}
		   Clk_H;

			 // Start ADC conversion for next pixel
		   ADC0_SC1A=  EnableInterrupts | ADC_SC1_ADCH(CAMERA0_ADCCHANNEL);
	    }
			 else
			 {
			  // dump conversion result
				delTimer =  ADC0_RA;
					// need clock 129 for some reson
						Clk_H;

						for(delTimer = 0;delTimer<64;delTimer++){	}

						Clk_L;

						CameraState = POT0;

						// Block if handler is using buffers;
						// this is a quick progress so it *should* be ok
						// to block in the ISR
						if(!CalcInProgress){
						// swap buffers
						swapBuffers();
						}
						else
						{
						swapAfterProcessing = true;
						}
						// change ADC Channel and start ADC
			      ADC0_SC1A =  EnableInterrupts | ADC_SC1_ADCH(POT0_ADCCHANNEL);
			 }

			break;
			default:
				CameraState = IDLE;
			break;
		}


}

void swapBuffers(){
safeBuffer = !safeBuffer;
volatileBuffer = !volatileBuffer;
}
