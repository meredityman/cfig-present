#pragma once

#include "ofMain.h"

#include "imageLoader.h"
#include "location.h"


class ofApp : public ofBaseApp{

	public:
	void setup();
	void update();
	void draw();

    void loadLocations(const std::string& filename);

    void populateJobs(){
        location_jobs.clear();
        ofDirectory backup_examples = ofDirectory("backup-jobs/present");
        ofDirectory examples = ofDirectory("current-jobs/present");
        
        if(!examples.exists()){
            ofLogWarning("Using backup jobs");
            examples = backup_examples;
        }
        
        examples.listDir();

        for(auto& dir : examples){
            if(!dir.isDirectory()) continue;
            if(ofDirectory(dir) == examples) continue;
            std::string location = ofFilePath::getBaseName(dir);

            ofDirectory subdir = ofDirectory(dir);
            for(auto& f : subdir){  
                if (!location_jobs.count(location)   ){
                    location_jobs[location] = std::vector<Job>();
                }       
                location_jobs[location].push_back(f.getAbsolutePath());

            }
        }
        


        location_itr = location_jobs.begin();
        job_itr = location_itr->second.begin();
        setupLoader();
    }


    

    void setupLoader(){
        loader = std::make_shared<ImageLoader>(job_itr->fullPath);
        loader->load();   
        // loader->change("in_image");
    }
    void iterateJob(int N){
        if(N > 0) {
            for(int i = 0; i < N; ++i) {
                std::advance(job_itr, 1);
                if (job_itr == location_itr->second.end()) {
                    job_itr = location_itr->second.begin();
                }
            }
        } else if(N < 0) {
            for(int i = 0; i < -N; ++i) {
                if (job_itr == location_itr->second.begin()) {
                    job_itr = location_itr->second.end();
                }
                std::advance(job_itr, -1);
            }
        }
        setupLoader();
    }

    void iterateLocation(int N){
        if(N > 0) {
            for(int i = 0; i < N; ++i) {
                std::advance(location_itr, 1);
                if (location_itr == location_jobs.end()) {
                    location_itr = location_jobs.begin();
                }
            }
        } else if(N < 0) {
            for(int i = 0; i < -N; ++i) {
                if (location_itr == location_jobs.begin()) {
                    location_itr = location_jobs.end();
                }
                std::advance(location_itr, -1);
            }
        }
        job_itr = location_itr->second.begin();
        setupLoader();
    }

    void keyPressed(ofKeyEventArgs& key);
    void mousePressed(ofMouseEventArgs& mouse);
    void mouseScrolled(ofMouseEventArgs& mouse);
    void setupSecondWindow(shared_ptr<ofAppBaseWindow> window);
    void drawSecondWindow(ofEventArgs &args);

    std::shared_ptr<ImageLoader> loader;
 
    std::map<std::string, std::vector<Job>> location_jobs;
	std::map<std::string, std::vector<Job>>::iterator location_itr;
	std::vector<Job>::iterator job_itr;

    ofFbo loaderFbo;
    
    ofTrueTypeFont  font;
};
