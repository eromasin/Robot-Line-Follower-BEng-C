
#include <stdbool.h>
#ifndef PITDriver
  #define PITDriver
void initPIT(void);

// autoexposure
extern int getExposureScore(void);
extern bool doAutoExposure;
bool autoexposureDone = false;
unsigned int exposureTime = 50000, bestExposureTime = 0;
int currentScore, bestScore = 0;
unsigned short int exposureIndex = 0;

extern bool runMainLoop;
#endif

