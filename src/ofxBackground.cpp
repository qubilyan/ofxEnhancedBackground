/*
 *  ofxBackground.cpp
 *
 *  Created by Joel Gethin Lewis on 15/03/2010.
 *
 */

#include "ofxBackground.h"

ofxBackgroundLearningCompleteEvent ofxBackground::onLearningComplete;

	//--------------------------------------------------------------------------------
ofxBackground::ofxBackground() {
    _width = 0;
    _height = 0;
	
	nChannels = CHANNELS;
	imageLen = 0;
	
    ImaskAVG = 0, ImaskAVGCC = 0;
    ImaskCodeBook = 0, ImaskCodeBookCC = 0;
	
	maxMod[0] = 3;  //Set color thresholds to default values
	minMod[0] = 10;
	maxMod[1] = 1;
	minMod[1] = 1;
	maxMod[2] = 1;
	minMod[2] = 1;
	scalehigh = HIGH_SCALE_NUM;
	scalelow = LOW_SCALE_NUM;	
	
	bAllocated = false;
	
	timeStartedLearning = 0.f;
	
	bLearning = false;
}

	//--------------------------------------------------------------------------------
ofxBackground::~ofxBackground() {
	clear();
	deallocateImages();	//be good
}

	//--------------------------------------------------------------------------------
void ofxBackground::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLog(OF_LOG_WARNING, "in allocate, reallocating a ofxCvImage, within OfxBackground");
		clear();
	}
	
	inputCopy.allocate(w, h);
	yuvImage.allocate(w, h);
	
	backgroundAverage.allocate(w, h);
	backgroundAverageConnectedComponents.allocate(w, h);
    backgroundCodebook.allocate(w, h);
	backgroundCodeBookConnectedComponents.allocate(w, h);
	
		//AVG METHOD ALLOCATION
	allocateImages(w,h); //redo everything if you change the size