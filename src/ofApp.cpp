/*This is a a basic image/ video manipulation program
 * based on colour range thresholding techniques, but
 * with data being fed from the audio buffer to manipulate the
 * pixels of the given image/ video stream.
 * Credit to Mick Grierson for thresholding techniques and of course
 * audio with Maximillian https://github.com/micknoise/Maximilian
 *
 *David Williams January 2019
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    warpAmt = 0;
    //video setup
    camWidth = 320;
    camHeight = 240;
    wCam.initGrabber(camWidth,camHeight);
    pixIn.allocate(camWidth,camHeight,3);
    pixOut.allocate(camWidth, camHeight,3);
    textural.allocate(camWidth,camHeight, GL_RGB);


    //load sample etc. before setting up soundcard

    //This is the necessary audio setup stuff for running Maxim with OF

    //Get and print a list of possible output devices

    auto devices = soundStream.getDeviceList();

    //cout << devices << endl;


    //This is the working setup for OF 0.10.0
    ofSoundStreamSettings settings;

    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 2;
    settings.setOutListener(this);
    //use the above 'devices' printout to get a list of audio devices
    settings.setOutDevice(devices[5]);
    soundStream.setup(settings);



}
//--------------------------------------------------------------
void ofApp::update(){
    wCam.update();

    //check if video has a new frame
    if(wCam.isFrameNew()){
        //get pixels from webcam feed
        pixIn = wCam.getPixels();
        //apply the effect
        pixOut = imgGlitch(camWidth,camHeight,3,pixIn,pixOut);
    }
    textural.allocate(pixOut);

}

//--------------------------------------------------------------
void ofApp::draw(){
    textural.draw(0,0, ofGetWidth(), ofGetHeight());


}
void ofApp::audioOut(ofSoundBuffer &outBuffer){


    //Put all sound related code within this for loop (This setup for OF 0.10.0)
    for(size_t i = 0; i < outBuffer.getNumFrames(); i ++){


        //some basic FM synthesis to test with the audio - image glitching
        oscOut = osc1.sinewave(20 + (modOsc.sinewave(1)* modOsc2.phasor(0.01,1,500)));

        mixOut = oscOut;

        //boost the values a bit for the drawbuffer
        double mixMap = ofMap(mixOut,-1,1,0,10);
        drawBuffer[i] = mixMap;


       //These are the audio stereo outs
        outBuffer.getSample(i, 0) = mixOut;
        outBuffer.getSample(i, 1) = mixOut;

    }

}
//--------------------------------------------------------------
ofPixels ofApp::imgGlitch(int w, int h, int channels, ofPixels _in, ofPixels _out){

    int totalSize = w * h * channels;

    int thresh = mouseX;

    for(int i = 0; i < totalSize; i ++){
        //get rgb valus
        int r = _in[i + 0];
        int g = _in[i + 1];
        int b = _in[i + 2];
        //simple greyscale by averaging
        int av = (r + g + b) / channels;

        //uncomment for b/w
        _in[i] = av;

        //scale the loop so that the drawBuffer array stays in its range
        int scale = ofMap(i,0, totalSize, 0, 512);
        double soundTest = drawBuffer[scale] * warpAmt;

        //accessing pixels out of range creates some fun glitching!

        //this is based on the basic thresholding technique -(credit to Mick Grierson) but feeding in some audio (numerically)
        //which accesses pixels out of range for some weird and wonderful colour glitching
        if(av > ofMap(thresh, 0, ofGetWidth(), 0, 255)){
            _out[i] = _in[i + soundTest] - _in[i - soundTest];
        }
        else{
            _out[i] = _in[i];
        }


    }


    return  _out;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    //increase/decrease the warp amount
    if(key == OF_KEY_PAGE_UP){
        warpAmt +=1;
    }
    if(key == OF_KEY_PAGE_DOWN){
        warpAmt -=1;
    }
}
