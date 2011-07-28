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

#pragma once

#include "ofMain.h"
#include "ofxFTGLFont.h"
#include <set>

typedef struct
{
    string word;
    ofRectangle box;
} WordWithSize;

class ofxWordPalette : public ofBaseHasTexture
{
  public:    
    ofxWordPalette();
    ~ofxWordPalette();
    
	void setup(int paletteWidth, int paletteHeight, string fontPath, int fontSize, float padding = 5);

	void setWords(string filePath); //search for words in the file, separated by whitespace
	void setWords(vector<string> newWords);
	
    //use this if you are going to draw alot of words to avoid binding/unbinding
    void bindPalette();
    void drawWord(string word, ofVec2f point, float scale = 1.0);
	void drawWord(WordWithSize& word, ofVec2f point, float scale = 1.0);

    void unbindPalette(); //must call after done drawing if manually binding
   
	void drawTypePalette(ofVec2f point);
    
    void getBoundingTextureCoordsForWord(string word, ofVec2f coords[4]);
    //fun helper functions
    WordWithSize& getRandomWord();
    //draws the word with the closest width
    WordWithSize& getWordMatchingWidth(float width);
	WordWithSize& getShortestWord();
    WordWithSize& getLongestWord();

	virtual ofTexture & getTextureReference();
	virtual void setUseTexture(bool bUseTex);
	
  protected:
    bool isSetup;
    int maxLineHeight;
    bool isBound;
    
	set<string> sourceWords;
	
    int paletteWidth;
    int paletteHeight;
    float padding;
	
    vector<WordWithSize> sortedwords; //sorted by length
    map<string, WordWithSize> words; //accessed through 
    
    ofxFTGLFont font;
    ofFbo typePalette;
    
};