
#ifndef UARTHandler
  #define UARTHandler
	
	
	#define UART0_ClockMask 0x400
	#define BaudGenClockSource_MCGFLL 0x04000000
	#define Baudrate_115200H 0x0
	#define Baudrate_115200L 0xC
	#define Disable_TxRx 0
	#define Enable_TxRx 0x0C
	#define Oversampling_16x 0x0F
	#define Config_8N1 0x00
	#define Config_NFI 0x00
	#define ClockGating_PortA 0x0200
	#define PinMux_UART 0x0200
	
	#ifndef commands_type_
	#define commands_type_

	typedef enum {IDLE, LINESCAN, SPEED, STEERING,ERROR} Commands;

	#endif // commands_type_
	volatile char charPtr = 0;
	volatile char currentChar = 0;
	volatile char strBuf[32];
	extern volatile char commandVal;
	extern volatile Commands currentCommand;
  void initialiseUART0(void);

  void sendString(char* string);
#endif
