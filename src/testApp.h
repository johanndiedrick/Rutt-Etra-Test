#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVideoGrabber 		vidGrabber;
    int 				camWidth;
    int 				camHeight;
    
    ofLight             light;
    ofEasyCam cam;
    
    
    //sound stuff
    
    ofSoundStream soundStream;
    void audioIn(float * input, int bufferSize, int nChannels); 
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    
    //playing video + rutt etra stuff
    ofVideoPlayer videoPlayer;
    
    int yStep;
    
    int xStep;
    
    ofPixels vidPixels;
    
    ofMesh mesh;
    
    //fft test
    
    
    
    //fft stuff
    
    ofSoundPlayer 		mp3;

    
    float 				* fftSmoothed;
    // we will draw a rectangle, bouncing off the wall:
    float 				px, py, vx, vy;
    
    int nBandsToGet;
    float prevx, prevy;
    

    
};
