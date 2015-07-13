#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

    void shot();
    
    void setupCamera();
    void updateCamera();
    
    void setupProjector();
    void updateProjector();
    
    void setupCaptureProcessor();
    void updateCaptureProcessor();
    void shootCaptureProcessor();
    void initCaptureProcessor();
    void saveCaptureProcessor();
    void changeImageDir();
    
private:
    
    bool        _isDrawingCamera;
    
    
    //camera
    ofVideoGrabber			_vidGrabber;
    ofxCvColorImage			_colorImageCam;
    
    //projector
    float _delay;
    int _nPic;
    int _nSeq;
    int _stepSeq;
    int _stepPic;
    
    string _file;
    string _imageDir;
    ofDirectory _dir;
    ofImage _image;
    
    ofxCvColorImage _cvImage;
    ofxCvGrayscaleImage _projectorImage;
    
    int _lastPlayed;
    
    //capture processor
    int _nPicCap;
    int _nSeqCap;
    
    int _captureTime;
    int _frequency;
    
    int _startTime;
    int _lastShot;
    bool _isShooting;
    
    bool _hasPictures;
    
    int _resX, _resY;
    
    ofImage _saveImage;



};

