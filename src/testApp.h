#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvHaarFinder.h"
#include "ofxFX.h"
#include "ofxUI.h"


class testApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void windowResized(int w, int h);
    void mousePressed(int x, int y);
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void exit();
    
    ofVideoGrabber      vidGrabber;
    
    ofxCvHaarFinder     finder;
	ofxCvColorImage     colorImg;
	ofxCvGrayscaleImage grayImage;
    
    ofDirectory         imagesDirectory;
    vector<ofImage>     images;
    int                 currentImage;
    bool                bSwitchImage;
    
    ofxFluid            fluid;
    ofVec2f             oldM;
    ofxUICanvas         *gui;
};
