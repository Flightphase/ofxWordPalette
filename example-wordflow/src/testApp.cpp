/*
 *  ofxWordPalette
 *
 * Created by James George, http://www.jamesgeorge.org @ Flightphase http://www.flightphase.com 
 * for the National Maritime Musuem
 * requires ofxFTGL : https://github.com/Flightphase/ofxFTGL
 *
 **********************************************************
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 * ofxWordPalette lets you draw lots and lots of text efficiently by rendering
 * a set number of words once into an FBO and then drawing them as textures
 *
 * ofxWordPalette also has helper functions to do fun stuff involving the length
 * of words
 */

#include "testApp.h"
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	ofEnableAlphaBlending();

	words.setup(2048, 1024, "verdana.ttf", 8);
	words.setWords("poe.txt");
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(IMAGE_WIDTH,IMAGE_HEIGHT);

	colorImage.allocate(IMAGE_WIDTH,IMAGE_HEIGHT);
	thisGrayImage.allocate(IMAGE_WIDTH,IMAGE_HEIGHT);
    lastGrayImage.allocate(IMAGE_WIDTH,IMAGE_HEIGHT);
	opticalFlow.allocate(IMAGE_WIDTH, IMAGE_HEIGHT);
	
	firstFrame = true;
}

//--------------------------------------------------------------
void testApp::update(){
	vidGrabber.grabFrame();
	if (vidGrabber.isFrameNew()){		
		colorImage.setFromPixels(vidGrabber.getPixels(), IMAGE_WIDTH,IMAGE_HEIGHT);
		
		lastGrayImage = thisGrayImage;
		thisGrayImage = colorImage;
		if(!firstFrame){
			opticalFlow.calc(lastGrayImage, thisGrayImage, 3);
		}
		firstFrame = false;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255);

	for(int y = 0; y < IMAGE_HEIGHT; y += 5){
		for(int x = 0; x < IMAGE_WIDTH; x += 5){
			ofVec2f flow = opticalFlow.flowAtPoint(x, y);
			ofVec2f direction = flow.normalized();
			float length = flow.length();
			if (length > 10) {
				WordWithSize& w = words.getWordMatchingWidth(length);
				ofPushMatrix();
				ofTranslate(x, y);
				ofRotate( atan2(direction.y, direction.x) * RAD_TO_DEG);
				words.drawWord(w, ofVec2f(0,0) );
				ofPopMatrix();
			}
		}	
	}
	ofPopMatrix();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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