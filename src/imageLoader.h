#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "location.h"


class Job {
    public:
    std::string fullPath;
    std::string name;
    std::string user;
    std::string datetime;
    std::string gid;

    Job(std::string fullPath){
        this->fullPath = fullPath;
        this->name     = ofFilePath::getFileName(fullPath);

        std::vector<std::string> parts = ofSplitString(ofFilePath::getFileName(fullPath), ".");
        this->gid = std::string(parts[0], 0, 4);
        this->user = std::string(parts[0], 4, parts[0].size() - 1);
        this->datetime = parts[1];

    };

    // typedef std::vector<Job> JobList;
    // typedef std::map<std::string, JobList> LocationList;
};


class ImageLoader {
public:

    // typedef std::map<std::string, std::string> LocationDict;
    // typedef std::map<std::string, LocationDict> Locations;



    ImageLoader(const std::string& folderPath);
    void load();
	void draw(ofFbo & fbo);

    std::string toString() const{
        std::ostringstream str;
        str << "ImageLoader" << "\n";
        str << "Path:" << dir.getAbsolutePath() << "\n";
        str << "Images:" << "\n";
        for (auto const& x : images)
            str << "\t" <<  x.first << "\n";

        str << "Masks:" << "\n";
        for (auto const& x : mask_images)
            str << "\t" <<  x.first << "\n";

        // str << "Json:" << "\n";
        // for (auto const& x : metadata){
        //     str << "\t" <<  x.first << "\n";
        //     str <<  x.second->toStyledString() << "\n";

        // }
        return str.str();
    }

    void drawList();
    void drawFileList(const std::vector<Job>& jobs);
    void drawDetails();
    void drawTitle();


    void iterate(int N){
        first = second;
        if(N > 0) {
            for(int i = 0; i < N; ++i) {
                std::advance(second, 1);
                if (second == images.end()) {
                    second = images.begin();
                }
            }
        } else if(N < 0) {
            for(int i = 0; i < -N; ++i) {
                if (second == images.begin()) {
                    second = images.end();
                }
                std::advance(second, -1);
            }
        }
        resetFade();
    }
    void iterateMask(int N){
        if(N > 0) {
            for(int i = 0; i < N; ++i) {
                std::advance(mask_itr, 1);
                if (mask_itr == mask_images.end()) {
                    mask_itr = mask_images.begin();
                }
            }
        } else if(N < 0) {
            for(int i = 0; i < -N; ++i) {
                if (mask_itr == mask_images.begin()) {
                    mask_itr = mask_images.end();
                }
                std::advance(mask_itr, -1);
            }
        }
        resetFade();
    }

    void change(std::string name){
        if(name == second->first) return;

        first  = second;
        second = images.find(name);
        resetFade();
    }

    void changeMask(std::string name){
        if(name == mask_itr->first) return;
        mask_itr = mask_images.find(name);
    }

    float getFadeF(){
        float f = (float)(ofGetElapsedTimeMillis() - lastChangeTime) / 2500;
        return ofClamp(f, 0.0, 1.0 );
    }

    void resetFade(){
        lastChangeTime = ofGetElapsedTimeMillis();
    }
    
    std::map<std::string, ofImage> images, mask_images;
    std::map<std::string, std::shared_ptr<ofxJSONElement>> metadata;
    
private:
    ofDirectory dir;
    Job job;


	std::map<std::string, ofImage>::iterator first;
	std::map<std::string, ofImage>::iterator second;
	std::map<std::string, ofImage>::iterator mask_itr;

	uint64_t lastChangeTime;

    ofShader shader;

    ofTrueTypeFont  font_large_regular , font_large_bold,
                    font_normal_regular, font_normal_bold,
                    font_small_regular , font_small_bold;

};
