#include "testApp.h"
#include "ofGLUtils.h"
#include "ofGLRenderer.h"
#include <iostream>
using namespace std;

//things to do
//work on fft
//be able to adjust brightness, contrast, saturation
//smooth out brightness so it doesn't peak
//add some special effects for when 1) brightness increases and 2) reaches a certain value

//--------------------------------------------------------------
void testApp::setup(){
    
    
     
	camWidth 		=  320;	// try to grab at this size. 
	camHeight 		=  240;
	
	//vidGrabber.initGrabber(camWidth,camHeight);
    mp3.loadSound("sounds/river.mp3");
    
    

    videoPlayer.loadMovie("sunset.MOV");
   // videoPlayer.loadMovie("sky_proxy_comp_1.mp4");

    videoPlayer.play();
    
    
    fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = videoPlayer.getHeight();

    
    
    //sound stuff
    
//    soundStream.setDeviceID(2);
//
//    soundStream.listDevices();
//    int bufferSize = 256;
//    
//    
//    left.assign(bufferSize, 0.0);
//	right.assign(bufferSize, 0.0);
//	volHistory.assign(400, 0.0);
//	
//	bufferCounter	= 0;
//	drawCounter		= 0;
//	smoothedVol     = 0.0;
//	scaledVol		= 0.0;
//    
//	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    videoPlayer.setVolume(0);
    mp3.play();
    yStep = 3;
    xStep = 1;

    
}

//--------------------------------------------------------------
void testApp::update(){
    
    //uncomment for video grabber
    //vidGrabber.update();
    
    // update the sound playing system:
	ofSoundUpdate();
    
    float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
		
	}
    
    
    videoPlayer.idleMovie();
    
    //update pixels for mesh
    
    
    //lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    
    //update pixels in vidpixels if we have a new video frame

    if(videoPlayer.isFrameNew()){
        mesh.clear();

    vidPixels = videoPlayer.getPixelsRef();
        
        for (int y = 0; y<videoPlayer.height; y+=yStep){
            ofNoFill();
          //  ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            
            for (int x = 0; x < videoPlayer.width; x += xStep){
                ofColor curColor = vidPixels.getColor(x, y);
                mesh.addColor(ofColor(curColor, 255));
                mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3 + fftSmoothed[y]*30.f));
                //curColor.getBrightness() * .3 + scaledVol));
            }
        }
        

    }
    

    
}

//--------------------------------------------------------------
void testApp::draw(){

   // light.enable();  
    ofBackground(0);
    cam.begin();
    
    glEnable(GL_DEPTH_TEST);
    
    
    ofScale(2, -2, 2);
    
    //uncomment for video grabber
    //  ofTranslate(-vidGrabber.width/2, -vidGrabber.height/2);
    
    //translate based on size of video
    ofTranslate(-videoPlayer.width/2, -videoPlayer.height/2);
    

    mesh.draw();

    cam.end();
    
    //draw framerate for fun
	ofSetColor(255);
	string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(msg, 10, 20);
}

//--------------------------------------------------------------

void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key=='y'){
        if(yStep<100)       
            yStep++;
        
    }
    
    if(key=='u'){
        if(yStep>1)
        yStep--;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}