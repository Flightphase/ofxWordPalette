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

#include "ofxWordPalette.h"

bool wordsort(WordWithSize a, WordWithSize b) {
    return a.box.width > b.box.width;
}

ofxWordPalette::ofxWordPalette(){
    isSetup = false;
    isBound = false;
    paletteWidth = -1;
    paletteHeight = -1;
    maxLineHeight = 0;
    padding = 5;
}

ofxWordPalette::~ofxWordPalette(){
	
}

void ofxWordPalette::setup(int _paletteWidth, int _paletteHeight, string fontPath, int fontSize, float padding){
	paletteWidth = _paletteWidth;
	paletteHeight = _paletteHeight;
	
    if(paletteWidth < 0 || paletteHeight < 0){
        ofLog(OF_LOG_ERROR, "ofxWordPalette -- Palette dimensions are incorrect, defaulting to 1024x1204 ");
        paletteWidth = 1024; 
        paletteHeight = 1024; 
    }
    
    if(!isSetup || typePalette.getWidth() != paletteWidth || typePalette.getHeight() != paletteHeight){
    	typePalette.allocate(paletteWidth, paletteHeight, GL_RGBA);    
    }
	
	if(!font.loadFont(fontPath, fontSize, true, false, false)){
        ofLog(OF_LOG_ERROR, "Couldn't load font " + fontPath);
        return;
    }
    
    isSetup = true;
}

//search for words in the file, separated by whitespace
void ofxWordPalette::setWords(string filePath){
	ofFile file;
	if(!file.open(filePath)){
		ofLog(OF_LOG_ERROR, "ofxWordPalette -- File " + filePath + " not found");
		return;
	}
	
	string rawString = file.readToBuffer().getText();
	
	vector<string> tokens = ofSplitString(rawString, " ", true, true);
	cout << "found " << tokens.size() << " words " << endl;
	setWords(tokens);
}

void ofxWordPalette::setWords(vector<string> newWords){
	
	ofPushStyle();
	
	sourceWords.clear();
	words.clear();

	for(int i = 0; i < newWords.size(); i++){
		sourceWords.insert( newWords[i] );
	}
    
  	set<string>::iterator wordit;
    maxLineHeight = 0;
	for(wordit = sourceWords.begin(); wordit != sourceWords.end(); wordit++){
        float lineHeight = font.getStringBoundingBox(*wordit, 0, 0 ).height;
        if(lineHeight > maxLineHeight){
            maxLineHeight = lineHeight;
        }
    }
    maxLineHeight += padding*2;
	
    //create sorted word list
	ofPoint pointInSpriteMap(0,0);    
    typePalette.begin();
    ofClear(0., 0., 0., 0.);	
	ofSetColor(0);
	for(wordit = sourceWords.begin(); wordit != sourceWords.end(); wordit++){
        WordWithSize w;
        w.word = *wordit;
        if(w.word == ""){
            continue;
        }
		
		w.box = font.getStringBoundingBox(w.word, pointInSpriteMap.x, pointInSpriteMap.y);
		w.box.y = pointInSpriteMap.y;
        w.box.width += padding*2;
        w.box.height = maxLineHeight;
        
        if(pointInSpriteMap.x + w.box.width > paletteWidth){
            //move to next line
            pointInSpriteMap.x = 0;
            pointInSpriteMap.y += maxLineHeight;
            w.box.y = pointInSpriteMap.y;
            w.box.x = 0;
            if(pointInSpriteMap.y + maxLineHeight > paletteHeight){
                ofLog(OF_LOG_ERROR, "Font is rendered outside of map");
            }
        }
		
        font.drawString(w.word, pointInSpriteMap.x+padding, pointInSpriteMap.y + maxLineHeight - padding);
        
		//cout << " word is " << w.word << " box w/h " << w.box.width << " " << w.box.height << endl;
		
        pointInSpriteMap.x += w.box.width;
        
        words[w.word] = w ;
    }
    
	typePalette.end();
	
	sortedwords.clear();
    for(map<string, WordWithSize>::iterator it = words.begin(); it != words.end(); it++){
        sortedwords.push_back( it->second );
    }
    sort(sortedwords.begin(), sortedwords.end(), wordsort);
	
	ofPopStyle();
}

WordWithSize& ofxWordPalette::getRandomWord(){
    
    //TODO can use sorted vector here to make it simpler...
    map<string, WordWithSize>::iterator item = words.begin();
    std::advance( item, int(ofRandom(words.size())));
    return item->second;
}

WordWithSize& ofxWordPalette::getWordMatchingWidth(float width){
    for(int i = 0; i < sortedwords.size(); i++){
        if(width >= sortedwords[i].box.width){
            return sortedwords[i];
        }
    }
	return sortedwords[sortedwords.size()-1];
}

void ofxWordPalette::getBoundingTextureCoordsForWord(string word, ofVec2f coords[4]){
    if(!isSetup) return;
        
    if(words.find(word) == words.end()){
        ofLog(OF_LOG_WARNING, "ofxWordPalette -- Word " + word + " not found in palette");
        return;
    }
    
    WordWithSize& wordToDraw = words[word];
   	coords[0].x = wordToDraw.box.x;
    coords[0].y = wordToDraw.box.y;
    coords[1].x = wordToDraw.box.x+wordToDraw.box.width;
    coords[1].y = wordToDraw.box.y;
    coords[2].x = wordToDraw.box.x+wordToDraw.box.width;
    coords[2].y = wordToDraw.box.y+maxLineHeight;
    coords[3].x = wordToDraw.box.x;
    coords[3].y = wordToDraw.box.y+maxLineHeight;    
}

WordWithSize& ofxWordPalette::getShortestWord(){
	return sortedwords[sortedwords.size()-1];
}

WordWithSize& ofxWordPalette::getLongestWord(){
    return sortedwords[0];
}

void ofxWordPalette::drawTypePalette(ofVec2f point){
    if(!isSetup) return;
    
    typePalette.draw(point.x, point.y);
    
    ofPushStyle();
    
    ofNoFill();
    
    map<string, WordWithSize>::iterator item = words.begin();
    
    while(item != words.end()){
		ofSetColor(255, 10, 0); 
        ofRect(item->second.box);        
        item++;
    }
    
    ofPopStyle();
}

ofTexture& ofxWordPalette::getTextureReference(){
	return typePalette.getTextureReference();
}

void ofxWordPalette::setUseTexture(bool bUseTex){
	ofLog(OF_LOG_WARNING, "ofxWordPalette -- Must used texture, setUseTexture is meaningless");
}

void ofxWordPalette::drawWord(string word, ofVec2f point, float scale){
    
    if(!isSetup) return;
    
    if(words.find(word) == words.end()){
        ofLog(OF_LOG_WARNING, "ofxWordPalette -- Word " + word + " not found in palette");
        return;
    }
	
	WordWithSize& wordToDraw = words[word];
	drawWord(wordToDraw, point, scale);
}

void ofxWordPalette::drawWord(WordWithSize& wordToDraw, ofVec2f point, float scale){

	if(!isSetup) return;

	//cout << "drawing word " << word << " at point " << point.x << " " << point.y <<  endl;
	
    bool alreadyBound = isBound;
    if(!alreadyBound){
        bindPalette();
    }
    
    //DRAW
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(point.x, point.y);
    ofScale(scale,scale,scale);
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(wordToDraw.box.x, wordToDraw.box.y);
    glVertex2f(0, 0);
	
    glTexCoord2f(wordToDraw.box.x+wordToDraw.box.width, wordToDraw.box.y);
    glVertex2f(wordToDraw.box.width, 0);
	
    glTexCoord2f(wordToDraw.box.x+wordToDraw.box.width, wordToDraw.box.y+maxLineHeight);
    glVertex2f(wordToDraw.box.width, maxLineHeight);
	
    glTexCoord2f(wordToDraw.box.x, wordToDraw.box.y+maxLineHeight);
    glVertex2f(0, maxLineHeight);
    
    glEnd();
    
    ofPopMatrix();
	ofPopStyle();
	
    if(!alreadyBound){
        unbindPalette();
    }
	
}

void ofxWordPalette::bindPalette(){
    if(!isSetup) return;
    
    typePalette.getTextureReference().bind();
    isBound = true;
}


void ofxWordPalette::unbindPalette(){
    if(!isSetup) return;
    
    typePalette.getTextureReference().unbind();
    isBound = false;
}

