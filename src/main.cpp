#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings settings;

    // Setup the main window
    settings.setSize(1920, 1080);
    settings.setPosition(glm::vec2(0, 0));
    settings.setGLVersion(3,2);
    settings.windowMode = OF_FULLSCREEN;
    settings.resizable = false;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    // Setup the second window
    // settings.setSize(1024, 768);
    // settings.setPosition(ofVec2f(ofGetScreenWidth(), 0));
    // settings.setGLVersion(3,2);
    // settings.resizable = true;
    // settings.shareContextWith = mainWindow;
    // shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);

    // Run the app with the shared context
    shared_ptr<ofApp> mainApp(new ofApp);
    // mainApp->setupSecondWindow(secondWindow);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

}
