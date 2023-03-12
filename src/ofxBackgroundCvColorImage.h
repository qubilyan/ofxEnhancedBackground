/*
 *  ofxBackgroundCvColorImage.h
 *  ofxBackground
 *
 *  Created by Joel Gethin Lewis on 16/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_BACKGROUND_CV_COLOR_IMAGE
#define OFX_BACKGROUND_CV_COLOR_IMAGE

#include "ofxCvColorImage.h"

class ofxBackgroundCvColorImage : public ofxCvColorImage {
public:
	virtual void  convertRgbToYuv(){
			cvCvtColor( cvImage, cvImageTemp, CV_BGR2YCrCb);
			swapTemp();
			flagImageCha