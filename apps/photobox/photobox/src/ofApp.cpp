#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofxXmlSettings XML;
    if( XML.loadFile("mySettings.xml") ){
		cout << "mySettings.xml loaded!" << endl;
	}else{
		cout << "unable to load mySettings.xml check data/ folder" << endl;
    }
    
    _delay = XML.getValue("SETTINGS:IMAGEDELAY", 200);
    _frequency = XML.getValue("SETTINGS:FREQUENCY", 200);
	_captureTime = ("SETTINGS:CAPTURETIME", 2000);
    _resX = ("SETTINGS:RESX", 320);
    _resY = ("SETTINGS:RESY", 240);
    
    _imageDir = XML.getValue("SETTINGS:IMAGEDIR", "images0000");

    
	//setup directory
    ofDirectory dir;
	if (!dir.doesDirectoryExist(_imageDir))
	{
		dir.createDirectory(_imageDir);
	}
    _hasPictures = false;
    
    setupCamera();
    setupProjector();
    setupCaptureProcessor();
    
    //camera stuff
    _isDrawingCamera = false;
    
    if (_dir.listDir(_imageDir) >0) {
        _hasPictures = true;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
	updateCamera();
    
    
    if (!_isDrawingCamera) {
        updateProjector();
    }
    
    updateCaptureProcessor();
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (_isDrawingCamera && _hasPictures)
    {
        ofSetColor(255, 255, 255);
        _colorImageCam.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    }
    else if (_hasPictures)
    {
        ofSetColor(255, 255, 255);
        _projectorImage.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
        case ' ':
			shot();
			break;
        case 'c':
			if (_isDrawingCamera)
                _isDrawingCamera = false;
            else
                _isDrawingCamera = true;
			break;
        case 'd':
            _nSeq = _dir.listDir(_imageDir);
            break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(!_isShooting && button == 0) shot();
    else if (button == 1 || button == 2) changeImageDir();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::shot() {
    initCaptureProcessor();
    cout<< "init capture " <<endl;
}

void ofApp::setupCamera(){
    
    _vidGrabber.listDevices();
	_vidGrabber.setVerbose(true);
	_vidGrabber.initGrabber(_resX,_resY);
	_colorImageCam.allocate(_resX,_resY);
    
}

void ofApp::updateCamera(){
    _vidGrabber.update();
    
	
	if (_vidGrabber.isFrameNew()){
		_colorImageCam.setFromPixels(_vidGrabber.getPixels(), _resX,_resY);
	}
}

void ofApp::setupProjector(){
	_nSeq = 0;
	_nPic = 0;
	_stepSeq = 0;
	_stepPic = 0;
	_lastPlayed = ofGetElapsedTimeMillis();
	_image.allocate(_resX,_resY, OF_IMAGE_COLOR);
	_cvImage.allocate(_resX, _resY);
	_projectorImage.allocate(_resX, _resY);
}

void ofApp::updateProjector(){
    if (_hasPictures) {
        if(ofGetElapsedTimeMillis() - _lastPlayed > _delay)
        {
            _nSeq = _dir.listDir(_imageDir);
            
            if (_stepPic >= _nPic)
            {
                _stepSeq++;
                if (_stepSeq < _nSeq)
                {
                    _stepPic = 0;
                    _file = _imageDir + "/";
                    _file += ofToString(_stepSeq);
                    _nPic = _dir.listDir(_file);
                }
                else
                {
                    _stepSeq = 0;
                    _stepPic = 0;
                    _file = _imageDir + "/";
                    _file += ofToString(_stepSeq);
                    _nPic = _dir.listDir(_file);
                }
            }
            _file = _imageDir + "/";
            _file += ofToString(_stepSeq);
            _file += "/";
            _file += ofToString(_stepPic);
            _file += ".jpg";
            _image.loadImage(_file);
            _stepPic++;
            _lastPlayed = ofGetElapsedTimeMillis();
        }
        _cvImage.setFromPixels(_image.getPixels(), _image.getWidth(), _image.getHeight());
        _projectorImage = _cvImage;
    }
}

void ofApp::setupCaptureProcessor(){
	_lastShot = 0;
	_isShooting = false;
	_startTime = 0;
    _image.allocate(_resX,_resY, OF_IMAGE_COLOR);
}

void ofApp::initCaptureProcessor(){
    ofDirectory dir;
	_nSeqCap = dir.listDir(_imageDir + "/");
	_nPicCap = 0;
	string nameDir = _imageDir + "/";
	nameDir += ofToString(_nSeqCap);
	ofDirectory::createDirectory(nameDir);
	_isShooting = true;
	_startTime = ofGetElapsedTimeMillis();
    //cout<< "init capture started" <<endl;
}

void ofApp::updateCaptureProcessor(){
    
	//check if to stop
	if (_isShooting) {
		//check is to stop
		if (ofGetElapsedTimeMillis() - _startTime > _captureTime) {
			_isShooting = false;
		}
		else {
			//check if new photo
			if (ofGetElapsedTimeMillis() - _lastShot > _frequency) {
				saveCaptureProcessor();
                if (!_hasPictures) {
                    _hasPictures = true;
                }
			}
		}
        
	}
}

void ofApp::saveCaptureProcessor(){
    string file = ofToDataPath(_imageDir + "/", true);
	file.append(ofToString(_nSeqCap));
	file.append("/");
	file.append(ofToString(_nPicCap));
	file.append(".jpg");
	//_image.setFromPixels(_inputDevice->getColorImage()->getPixelsRef());
	_image.setFromPixels(_colorImageCam.getPixelsRef());
	_image.saveImage(file);
    //cout<< "pic saved " << file <<endl;
	_lastShot = ofGetElapsedTimeMillis();
	++_nPicCap;
}

void ofApp::changeImageDir(){
    int n = ofToInt(_imageDir.substr(6,9));
    _imageDir = "images" + ofToString(n+1);
    _hasPictures = false;
    
    ofxXmlSettings XML;
    if( XML.loadFile("mySettings.xml") ){
		cout << "mySettings.xml loaded!" << endl;
	}else{
		cout << "unable to load mySettings.xml check data/ folder" << endl;
	}
    
	XML.setValue("SETTINGS:IMAGEDIR", _imageDir);
    XML.save("mySettings.xml");

    
}

