// Cameron Coats / Group 3
// EN0627 / NXP Car
// 
// Linescan Camera Handler (High Level)

#include "CameraHandler.h"
#include "MKL25Z4.h"
#include <stdbool.h>
#include <stdio.h>


int getLineOffset(void){
	unsigned short int i,k,maxIndex,minIndex;
	short int diff[110];
	
	// ensure that buffers are not swapped while calculating diffs
	// as this would give an unpredictable result
	CalcInProgress = true;
  // calculate differences
	k=0;
	for(i=10;i<118;i++){
	  diff[k] = image[safeBuffer][i] - image[safeBuffer][i+1];
	  k++;
	}
	// we are now done with the image buffer so the buffers can now be swapped
	CalcInProgress = false;
	// if we missed the buffer swap in interrupt (due to inProgress flag set),
	// swap the buffers now that processing has finished
	
	if(swapAfterProcessing){
	swapBuffers();
	swapAfterProcessing = false;
	}
	
  // get min
	minValue = diff[0];
	minIndex = 0;
	for(i=105;i;i--){
	  if(diff[i] < minValue){
	    minValue = diff[i];
	    minIndex = i;
	  }
	}
	
	// get max
	maxValue = diff[0];
	maxIndex = 0;
	for(i=105;i;i--){
	  if(diff[i] > maxValue){
	    maxValue = diff[i];
	    maxIndex = i;
	  }
	}				
	
	if( (maxValue - minValue) < FAULT_THRESHOLD){
	cameraFault = true;
	}
	else
	{
		cameraFault = false;
	}
	
	
	currentValue =  (minIndex + ((maxIndex - minIndex) / 2)) - 53;
	
	// low pass filter
	// if change is greater than 15, half change
	if(currentValue < previousValue - 15 || currentValue > previousValue + 15) currentValue = (currentValue - previousValue) / 2;
	
	// moving average filter
	// first form array of past values
	pastValues[currentIndex] = currentValue;
	
	// handle index iterating
	if(currentIndex == 7)currentIndex = 0;
		else currentIndex++;
	
	// then calculate mean
	average = 0;
	for(i=0;i<8;i++){
	average += pastValues[i];
	}
	// using 8 values as this is just bit shifting
	average = average / 8;
	// update previous value for LPF
	previousValue = currentValue;
	
	
	return average;
}

int getExposureScore(void){
	unsigned short int i,k;
	short int diff[110];
	
	// ensure that buffers are not swapped while calculating diffs
	// as this would give an unpredictable result
	CalcInProgress = true;
  // calculate differences
	k=0;
	for(i=10;i<118;i++){
	  diff[k] = image[safeBuffer][i] - image[safeBuffer][i+1];
	  k++;
	}
	// we are now done with the image buffer so the buffers can now be swapped
	CalcInProgress = false;
  // get min
	minValue = diff[0];
	for(i=0;i<106;i++){
	  if(diff[i] < minValue){
	    minValue = diff[i];
	  }
	}
	
	// get max
	maxValue = diff[0];
	for(i=0;i<106;i++){
	  if(diff[i] > maxValue){
	    maxValue = diff[i];
	  }
	}				

  // get center point
	return maxValue - minValue;		
}
