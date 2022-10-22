#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxBackground.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(i