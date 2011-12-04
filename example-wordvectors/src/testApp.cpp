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

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	
	ofEnableAlphaBlending();

	words.setup(2048, 1024, "verdana.ttf", 18);
	words.setWords("poe.txt");
	
	bool oddRow = false;
	for(int y = -200; y < ofGetHeight()+200; y += 20){
		for(int x = -200; x < ofGetWidth()+200; x += 20){
			ofVec2f position = ofVec2f();
			position.x = x;
			if(oddRow){
				position.x += 10;
			}
			position.y = y;
			points.push_back( position );
		}
		oddRow = !oddRow;
	}
	
	shortestWordLength = words.getShortestWord().box.width;
	longestWordLength = words.getLongestWord().box.width;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255);
	
//	words.drawTypePalette(ofVec2f(0,0));

	//find the furthest and closest point
	ofVec2f mousePoint(ofGetMouseX(), ofGetMouseY());
	float greatestDistance = 0;
	float leastDistance = INT_MAX;
	for(int i = 0; i < points.size(); i++){
		float thisDistance = mousePoint.distance(points[i]);
		if(thisDistance > greatestDistance){
			greatestDistance = thisDistance;
		}
		if (thisDistance < leastDistance) {
			leastDistance = thisDistance;
		}
	}
	
	words.bindPalette();
	//draw the points, scaling longest words far away and shortest words close to the mouse
	//rotate the words to all point towards the mouse
	for(int i = 0; i < points.size(); i++){
		ofVec2f trajectory = mousePoint-points[i];
		ofVec2f direction = trajectory.normalized();
		float distanceToMouse = trajectory.length();
		float wordSize = ofMap(distanceToMouse, leastDistance, greatestDistance, shortestWordLength, longestWordLength);
		WordWithSize& w = words.getWordMatchingWidth(wordSize);
		
		ofPushMatrix();
		ofTranslate(points[i]);
		ofRotate( atan2(direction.y, direction.x) * RAD_TO_DEG);
		words.drawWord(w, ofVec2f(0,0) );
		ofPopMatrix();
	}
	
	words.unbindPalette();
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