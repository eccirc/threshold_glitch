#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    //custom thresholding function
    ofPixels imgGlitch(int w, int h, int channels, ofPixels _in, ofPixels _out);

    //get some video from the webcam
    ofVideoGrabber wCam;
    //create some ofPixels objects to have some fun with the video stream
    ofPixels pixIn, pixOut;
    //write the result to an ofTexture
    ofTexture textural;



    int camWidth, camHeight;

    //These are the necessary's for setting up Maxim in OF
    //This is the setup for OF 0.10.0
            void audioOut(ofSoundBuffer &outBuffer);
            //setup ofSoundStream
            ofSoundStream soundStream;
            //ofSoundBuffer lastBuffer;

    //PUT MAXIM OBJECTS HERE
    maxiOsc osc1, modOsc, modOsc2;


    //adjust the amount of image waaarping
    int warpAmt;

    double drawBuffer[512];




    //PUT OTHER STUFF HERE

    //outputs
    double oscOut, mixOut;









};
