
#include <stdbool.h>
#ifndef CameraHandler
  #define CameraHandler
	
  #define FAULT_THRESHOLD 600
	
	int getLineOffset(void);
	int getExposureScore(void);
	extern void startImageCapture(void);
	extern void LED_SetOut(unsigned int);
	extern void Timer2_DelaymS(int);
	extern unsigned short int image[2][256];
	extern unsigned char safeBuffer;
	extern bool swapAfterProcessing;
	extern void swapBuffers(void);
	short int maxValue,minValue;
	bool CalcInProgress = false;
	bool cameraFault = false;
	int previousValue,currentValue,average;
	unsigned char currentIndex = 0;
	int pastValues[8] = {0,0,0,0,0,0,0,0};
#endif
