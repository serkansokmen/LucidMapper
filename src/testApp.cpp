#include "testApp.h"

int camWidth = 256;
int camHeight = 192;

//--------------------------------------------------------------
void testApp::setup(){

    ofSetBackgroundAuto(true);
	ofEnableAlphaBlending();
	ofBackground(ofColor::white);
	ofSetFrameRate(50);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
	
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
    
    finder.setup("haar/haarcascade_eye.xml");
    
    imagesDirectory.listDir("images/");
	imagesDirectory.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    // allocate the vector to have as many ofImages as files
    if(imagesDirectory.size()){
		images.assign(imagesDirectory.size(), ofImage());
	}
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)imagesDirectory.size(); i++){
		images[i].loadImage(imagesDirectory.getPath(i));
	}
	currentImage = 0;
    bSwitchImage = false;
    
    fluid.allocate(camWidth, camHeight, 0.5);
    fluid.dissipation = 0.99;
    fluid.velocityDissipation = 0.99;
    
    fluid.setGravity(ofVec2f(0.0,0.0));
    //    fluid.setGravity(ofVec2f(0.0,0.0098));
    
    //  Set obstacle
    //
    fluid.begin();
    ofSetColor(255);
    ofCircle(camWidth*0.5, camHeight*0.35, 40);
    fluid.end();
    
    // Adding constant forces
    //
    fluid.addConstantForce(ofPoint(camWidth*0.5, camHeight*0.85), ofPoint(0,-2), ofFloatColor(0.5,0.1,0.0), 10.f);
    
    setupGui();
}

//--------------------------------------------------------------
void testApp::setupGui(){
    
    gui = new ofxUICanvas();
    gui->addLabel("LUCID MAPPER");
    gui->addSpacer();
    
    gui->addFPSSlider("FPS");
    
    gui->addToggle("MIRROR X", true);
    gui->addToggle("SWITCH IMAGE", &bSwitchImage);
    gui->addSpacer();
    gui->addButton("CLEAR BACKGROUND", false);
    
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    gui->loadSettings("GUI/guiSettings.xml");
    gui->setVisible(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()) {
        colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
    }
    grayImage = colorImg;
    
    finder.findHaarObjects(grayImage);
    
    ofPoint m = ofPoint(mouseX,mouseY);
    ofPoint d = (m - oldM)*10.0;
    oldM = m;
    ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
    c.normalize();
    fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5),3.0f);
    
    fluid.update();
    
    if (bSwitchImage){
        currentImage++;
		currentImage %= imagesDirectory.size();
        bSwitchImage = false;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    float x = (ofGetWidth() - images[currentImage].getWidth()) /2;
    float y = (ofGetHeight() - images[currentImage].getHeight()) /2;
    images[currentImage].draw(x, y);
    
    for(int i = 0; i < finder.blobs.size(); i++) {
        ofPushMatrix();
        ofTranslate(10, 400);
		ofPushStyle();
        ofSetColor(ofColor::white);
        grayImage.draw(0, 0, camWidth, camHeight);
        ofSetColor(ofColor::greenYellow);
        ofNoFill();
        ofRectangle cur = finder.blobs[i].boundingRect;
		ofRect(cur.x, cur.y, cur.width, cur.height);
        ofPopStyle();
        ofPopMatrix();
	}
    
    fluid.draw();
    
    if (gui->isVisible()) {
        gui->draw();
    }
}

void testApp::mousePressed(int x, int y){

}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key=='s') {
        gui->toggleVisible();
    }
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}
