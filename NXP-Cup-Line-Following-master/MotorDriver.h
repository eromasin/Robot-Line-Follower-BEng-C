
#include <stdbool.h>
#ifndef MotorDriver
  #define MotorDriver
	void initMotors(void);
	void disableMotors(void);
	void enableMotors(void);
	void setTorqueVectors(int);
	void setSpeed(char);
	extern unsigned short int POT1_Value;
	#define TORQUEVECTOR_AMT 60
	extern void sendString(char *);
#endif
