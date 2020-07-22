
#ifndef TFCHandler
  #define TFCHandler
	void setLEDs(unsigned char);
	#define PB_A (GPIOC_PDIR & 0x0002000)
	
	void initPorts(void);
#endif
