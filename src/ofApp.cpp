#include "ofApp.h"


void ofApp::setup() {
    ofDisableArbTex(); ofHideCursor();
    font.load("fonts/HKGrotesk-Bold.ttf", 32, true, true);

    populateJobs();
}

void ofApp::setupSecondWindow(shared_ptr<ofAppBaseWindow> window) {

}

void ofApp::keyPressed(ofKeyEventArgs& key){
    if(key.key == OF_KEY_LEFT){
        iterateJob(-1);
    } else if(key.key == OF_KEY_RIGHT) {
        iterateJob(1);
    }else if(key.key == OF_KEY_UP) {
        iterateLocation(-1);
    }else if(key.key == OF_KEY_DOWN) {
        iterateLocation(1);
    }
}
void ofApp::mousePressed(ofMouseEventArgs& mouse){
    if(mouse.button == OF_MOUSE_BUTTON_LEFT){
        loader->change("out");
    } else if(mouse.button == OF_MOUSE_BUTTON_RIGHT) {
        loader->change("in_image");
    } else if(mouse.button == OF_MOUSE_BUTTON_MIDDLE) {
        iterateJob(1);
    } else if(mouse.button == OF_MOUSE_BUTTON_4) {
        iterateLocation(-1);
    } else if(mouse.button == OF_MOUSE_BUTTON_5) {
        iterateLocation(1);
    }
}
void ofApp::mouseScrolled(ofMouseEventArgs& mouse){
    if(mouse.scrollY > 0.0){
        iterateJob(-1);
    } 
    else if(mouse.scrollY < 0.0) {
        iterateJob(1);
    } 
    // if(mouse.scrollX > 0){
    //     loader->iterate(-1);
    // } 
    // else if(mouse.scrollX < 0){
    //     loader->iterate(1);
    // }
}

void ofApp::update() {
    // Update logic if needed
}

void ofApp::draw() {
    ofBackground(ofColor::black);

    float planeSize = min(ofGetWidth(), ofGetHeight());
    planeSize *= 0.8;
    loaderFbo.allocate(planeSize, planeSize, GL_RGBA);
    loader->draw(loaderFbo); 

    ofPushMatrix();
        ofTranslate(0.5 * ofGetWidth(), 0.5 * ofGetHeight());
        loaderFbo.draw( -0.5 * loaderFbo.getWidth(),  -0.5 * loaderFbo.getHeight());
    ofPopMatrix();


    ofPushStyle();
    ofSetColor(ofColor::white);
    ofPushMatrix();
        ofTranslate(0.0, 0.5 * (ofGetHeight() - planeSize ));
        loader->drawDetails(); 
    ofPopMatrix();


    // ofPushMatrix();
    //     ofTranslate(0.5 * (planeSize + ofGetWidth()), 0.5 * (ofGetHeight() - planeSize ));
    //     loader->drawList(); 
    // ofPopMatrix();

    ofPushMatrix();
        ofTranslate(0.5 * (planeSize + ofGetWidth()), 0.5 * (ofGetHeight() - planeSize ));
        loader->drawFileList(location_itr->second); 
    ofPopMatrix();

    ofPushMatrix();
        ofTranslate(0.5 * ofGetWidth(), ofGetHeight() + 0.5 * (planeSize - ofGetHeight() ));
        loader->drawTitle(); 
    ofPopMatrix();
    ofPopStyle();

    // Lines
    // ofPushStyle();
    // ofSetColor(ofColor::white);
    // ofDrawLine( glm::vec2(ofGetMouseX(), 0.0), glm::vec2(ofGetMouseX(), ofGetHeight()));
    // ofDrawLine( glm::vec2(0.0, ofGetMouseY()), glm::vec2(ofGetWidth(), ofGetMouseY()));
    // ofPopStyle();

    // Title
    // ofPushStyle();
    // ofSetColor(ofColor::black);
    // auto rect = font.getStringBoundingBox(job_itr->name, 0, 0);
    // font.drawString( job_itr->name, (0.5 * ofGetWidth()) - 0.5 * rect.width, 50 );
    // ofPopStyle();


}

void ofApp::drawSecondWindow(ofEventArgs &args) {

}