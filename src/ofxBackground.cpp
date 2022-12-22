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
	allocateImages(w,h); //redo everything if you change the size! and this will be triggered first time round
	scaleHigh(scalehigh);
	scaleLow(scalelow);
	ImaskAVG = cvCreateImage( cvGetSize(inputCopy.getCvImage()), IPL_DEPTH_8U, 1 );
	ImaskAVGCC = cvCreateImage( cvGetSize(inputCopy.getCvImage()), IPL_DEPTH_8U, 1 );
	cvSet(ImaskAVG,cvScalar(255));
		//CODEBOOK METHOD ALLOCATION:
	yuvImage = cvCloneImage(inputCopy.getCvImage());
	ImaskCodeBook = cvCreateImage( cvGetSize(inputCopy.getCvImage()), IPL_DEPTH_8U, 1 );
	ImaskCodeBookCC = cvCreateImage( cvGetSize(inputCopy.getCvImage()), IPL_DEPTH_8U, 1 );
	cvSet(ImaskCodeBook,cvScalar(255));
	imageLen = inputCopy.getCvImage()->width*inputCopy.getCvImage()->height;
	cB = new codeBook [imageLen];
	for(int f = 0; f<imageLen; f++)
	{
		cB[f].numEntries = 0;
	}
	for(int nc=0; nc<nChannels;nc++)
	{
		cbBounds[nc] = 10; //Learning bounds factor
	}
	ch[0] = true; //Allow threshold setting simultaneously for all channels
	ch[1] = true;
	ch[2] = true;		
	
	_width = w;
	_height = h;
	bAllocated = true;
	
	timeStartedLearning = ofGetElapsedTimeMillis(); //for safetly? TODO: question
	bStatsDone = false;
}

	//--------------------------------------------------------------------------------
void ofxBackground::clear() {
	
	if (bAllocated == true){
		inputCopy.clear();
		yuvImage.clear(); //yuvImage is for codebook method
		
		backgroundAverage.clear();
		backgroundAverageConnectedComponents.clear();
		backgroundCodebook.clear();
		backgroundCodeBookConnectedComponents.clear();	
		
		bAllocated = false;
	}
}

void ofxBackground::reset(int w, int h) {
	clear();
	deallocateImages();	//be good	
	allocate(w,h);
}

void ofxBackground::allocateImages(int w, int h){
	
	IavgF = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 3 );
	IdiffF = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 3 );
	IprevF = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 3 );
	IhiF = cvCreateImage( cvSize(w,h), IPL_DEPTH_