// Cameron Coats / Group 3
// EN0627 NXP Car
// 
// Main (Centerline detection only)

#include "MKL25Z4.h"
#include "stdio.h"
#include "main.h"
#include "TFCHandler.h"

// set initial state
MainState State = INIT;
volatile Commands currentCommand = IDLE;
int main(void){
	char stringBuffer[128];
	unsigned char sendLine = 0;
	while(1){
		switch(State){
			
			case INIT:
				// Enable ports etc
				initPorts();
				// start timers
				initTimer0();
				// Setup UART
				initialiseUART0();
			  // Setup Camera
				initCameraDriver();
				// Setup Motors
				initMotors();
				// setup PIT
			  initPIT();
				// setup servo
				initServo();
				setCenter();
				// Send power on message
				sendString("> Init\n");
				// Go to wait state
				State = WAIT_PRESS;
			break;
			
		  case WAIT_PRESS:
				// Wait for pushbutton A
				if((GPIOC_PDIR & 1<<13) != 0){
					// Do line detection
				  State = START;
					//sendString("Running");
				}
				// handle UART commands here - decode in interrupt
				switch(currentCommand){
					case LINESCAN:
						State = SEND_LINE;
					break;
					case STEERING:
						State = SET_STEERING;
					break;
					case SPEED:
						State = SET_SPEED;
					break;
					case ERROR:
						State = COMMAND_ERROR;
					break;
					case IDLE:
					default:
						// nop
					break;
				}
			break;
				
					
			case CAMERA_FAULT:
			disableMotors();
			setCenter();
			sendString("> Camera Fault \n");
			score = getExposureScore();
			State = WAIT_PRESS;
			break;
			
			case STOP:
				disableMotors();
				setCenter();
			sendString("> Stopped \n");
			State = WAIT_PRESS;
			break;
			
			case START:
				enableMotors();
				cameraFault = false;
				State = WAIT_RUN;
			break;
			
			case RUN:
				linePos = getLineOffset();
				setTorqueVectors(linePos);
				setSteeringAngle(linePos);
				runMainLoop = false;
				if(sendLine == 20){
					sprintf(stringBuffer,"Line: %d\nSteering Amt: %d \n",linePos,(POT0_Value/128));
				sendString(stringBuffer);
				sendLine=0;
				} else {sendLine++;}
				State = WAIT_RUN;
			break;
			case WAIT_RUN:
				if(runMainLoop) State = RUN;
				if((GPIOC_PDIR & 1<<17) != 0) State = STOP;
				if(cameraFault) State = CAMERA_FAULT;
			break;
			case SEND_LINE:
				linePos = getLineOffset();
				sprintf(stringBuffer,"Line Position: %d\n",linePos);
				sendString(stringBuffer);
				State = WAIT_PRESS;
				currentCommand = IDLE;
				commandVal = 0;
			break;
			case SET_SPEED:
				enableMotors();
				setSpeed(commandVal);
				State = WAIT_PRESS;
				currentCommand = IDLE;
				commandVal = 0;
				sendString("> Ok\n");
			break;
			case SET_STEERING:
				setSteeringAngle(commandVal);
				State = WAIT_PRESS;
				currentCommand = IDLE;
				commandVal = 0;
				sendString("> Ok\n");
			break;
			case COMMAND_ERROR:
				sendString("> Command not recognised\n");
				State = WAIT_PRESS;
			break;
			default:
				State = WAIT_PRESS;
			break;
		}
  }
}
