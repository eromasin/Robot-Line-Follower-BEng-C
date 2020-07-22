
#include <stdbool.h>
#ifndef Main
  #define Main

	extern signed short getLineOffset(void);
	extern short int getExposureScore(void);
	extern void initCameraDriver(void);
	extern void initPIT(void);
	extern void initServo(void);
	extern void initMotors(void);
	extern void initTimer0(void);
	extern void initialiseUART0(void);
	extern void setLEDs(unsigned char);
	extern void setSpeed(char);
	
	extern void enableMotors(void);
	extern void disableMotors(void);
	
	extern void sendString(char*);
	extern void setTorqueVectors(int);
	extern void setSteeringAngle(int);
	extern void setCenter(void);
	
	extern void Timer0_DelaymS(int);
	int main(void);

	typedef enum {INIT, WAIT_PRESS, CAMERA_FAULT, START, STOP, RUN, WAIT_RUN, SEND_LINE, SET_STEERING,SET_SPEED,COMMAND_ERROR} MainState;
	
	#ifndef commands_type_
	#define commands_type_

	typedef enum {IDLE, LINESCAN, SPEED, STEERING, ERROR} Commands;

	#endif // commands_type_
	
	volatile char commandVal;
	extern bool cameraFault;
	bool doAutoExposure = false;
	extern bool autoexposureDone;
	short signed int linePos;
	
	extern int bestExposureTime;
	extern unsigned int bestScore;
	extern unsigned short int POT0_Value, POT1_Value, BATT_Value;
	extern short int maxValue,minValue;
	short int linePos;
	short int score;
	bool runMainLoop = false;
#endif
