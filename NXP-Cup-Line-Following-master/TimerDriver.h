
#ifndef TimerDriver
  #define TimerDriver
  
	#define FRQ_MCGFLLCLK 20971520
	#define ClockGating_Timer2 0x01<<26
	#define ClockGating_Timer1 0x01<<25
	#define ClockGating_Timer0 0x01<<24
	#define ClockSource_MCGFLL 0x01<<24
	#define TOF_Mask 0xBF
	
	void initTimer0(void);
	void Timer0_DelayuS(int);
	
#endif

