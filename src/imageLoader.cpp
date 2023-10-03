#include "imageLoader.h"

ImageLoader::ImageLoader(const std::string& folderPath) : 
    job(folderPath) {

    shader.load("shaders/shader");

    font_small_regular.load( "fonts/HKGrotesk-Regular.ttf", 14, true, true);
    font_small_bold.load(    "fonts/HKGrotesk-Bold.ttf"   , 14, true, true);
    font_normal_regular.load("fonts/HKGrotesk-Regular.ttf", 24, true, true);
    font_normal_bold.load(   "fonts/HKGrotesk-Bold.ttf"   , 24, true, true);
    font_large_regular.load( "fonts/HKGrotesk-Regular.ttf", 32, true, true);
    font_large_bold.load(    "fonts/HKGrotesk-Bold.ttf"   , 32, true, true);
}

void ImageLoader::load() {

    images.clear();
    mask_images.clear();

    dir = ofDirectory(job.fullPath);
    if(dir.exists()) {
        dir.listDir();
        for(auto& file: dir.getFiles()) {
            ofLogNotice("ImageLoader") << file.getFileName();

            if(file.getExtension() == "jpg") {
                ofImage image;
                std::string fileName = ofSplitString(file.getFileName(), ".")[2];
                if(image.load(file.getAbsolutePath())) {
                    images[fileName] = image;
                } else {
                    ofLogError("ImageLoader") << "Failed to load image: " << file.getAbsolutePath();
                }
            } else if(file.getExtension() == "bmp") {
                ofImage image;
                std::string fileName = ofSplitString(file.getFileName(), ".")[2];
                if(image.load(file.getAbsolutePath())) {
                    mask_images[fileName] = image;
                } else {
                    ofLogError("ImageLoader") << "Failed to load image: " << file.getAbsolutePath();
                }
            } else if(file.getExtension() == "json") {
                std::string fileName = file.getFileName();
                if(fileName == "meta.json"){
                    fileName = "meta";
                } else {
                    fileName = ofSplitString(fileName, ".")[2]; 
                }
                metadata[fileName] = std::make_shared<ofxJSONElement>();
                if(!metadata[fileName]->open(file.getAbsolutePath())) {
                    ofLogError("ImageLoader") << "Failed to load JSON: " << file.getAbsolutePath();
                }

            }
        }

        first    = images.find("in_image");
        second   = first;
        // first    = images.begin();
        // second   = images.begin();
        mask_itr = mask_images.begin();

        ofLogNotice("ImageLoader") << toString();


    } else {
        ofLogError("ImageLoader") << "Directory does not exist: " << job.fullPath;
    }
}


void ImageLoader::draw(ofFbo & fbo) {
    fbo.begin();
    ofClear(255,255,255, 0);

    ofTexture & tex1 = first->second.getTexture();
    ofTexture & tex2 = second->second.getTexture();
    ofTexture & tex3 = mask_itr->second.getTexture();

    ofPlanePrimitive plane;

	plane.set(fbo.getWidth(),fbo.getHeight(), 4, 4);
	plane.mapTexCoords(0, 0, 1.0, 1.0);
    
    ofPushMatrix();
        ofTranslate(0.5 * fbo.getWidth(), 0.5 * fbo.getHeight());
        ofPushMatrix();
        // ofScale(0.8);

        shader.begin();


        // Pass textures and uniform to the shader
        shader.setUniformTexture("tex1", tex1, 1);
        shader.setUniformTexture("tex2", tex2, 2);
        shader.setUniformTexture("tex3", tex3, 3);
        shader.setUniform1f("u_fade", getFadeF());
        shader.setUniform1f("u_sefFade", 0.0);
        shader.setUniform2f("u_resolution", 512, 512);

        plane.draw();
        shader.end();

        // ofPushMatrix();
        //     ofTranslate(0.5 * planeSize, - 0.5 * planeSize);
        //     drawList();
        // ofPopMatrix();

        // ofPushMatrix();
        //     ofTranslate(- 0.5 * ofGetWidth(), - 0.5 * planeSize);
        //     drawDetails();
        // ofPopMatrix();
 

        ofPopMatrix();
    ofPopMatrix();

    fbo.end();
}

void ImageLoader::drawList(){

    float lh = font_normal_bold.getLineHeight();
    glm::vec2 offset{ 20, font_normal_bold.getLineHeight()};

    font_normal_bold.drawString( "IMAGES", offset.x, offset.y); offset.y += 1.5 * lh;
    for ( auto x = images.begin(); x != images.end(); x++){
        if(x == second ){
            font_normal_bold.drawString( x->first, offset.x, offset.y); offset.y += lh;
        } else {
            font_normal_regular.drawString( x->first, offset.x, offset.y); offset.y += lh;
        }
    }

    offset.y += 1.5 * lh;

    font_normal_bold.drawString( "MASKS", offset.x, offset.y); offset.y += 1.5 * lh;
    for ( auto x = mask_images.begin(); x != mask_images.end(); x++){
        if(x == mask_itr ){
            font_normal_bold.drawString( x->first, offset.x, offset.y); offset.y += lh;
        } else {
            font_normal_regular.drawString( x->first, offset.x, offset.y); offset.y += lh;
        }
    }

}

void ImageLoader::drawFileList(const std::vector<Job>& jobs){

    float lh = font_normal_bold.getLineHeight();
    glm::vec2 offset{ 20, font_normal_bold.getLineHeight()};

    font_normal_bold.drawString( "RESULTS:", offset.x, offset.y); offset.y += 1.5 * lh;
    for ( auto j = jobs.begin(); j != jobs.end(); j++){
        if( j->fullPath == this->job.fullPath ){
            font_small_bold.drawString( j->name, offset.x, offset.y); offset.y += lh;
        } else {
            font_small_regular.drawString( j->name, offset.x, offset.y); offset.y += lh;
        }
    }

}

void ImageLoader::drawTitle(){
    ofxJSONElement meta = *metadata["meta"];
    std::string location_key = meta["location"].asString( );
    auto location = Location::locations[ofToInt(location_key)-1];

    float lh = font_normal_bold.getLineHeight();

    ostringstream ss;
    tm time;
    char buffer [80];

    strptime(job.datetime.c_str(), "%Y%m%d%H%M%S", &time);


    if(second == images.find("out")){
        time.tm_year = ofToInt(location.future_date) - 1900;
    } 
    strftime(buffer, 80 ,"%A, %B %d, %Y %I:%M:%S %p", &time);
    ss << buffer;


    glm::vec2 offset{ 0, 20.0 + font_normal_bold.getLineHeight()};

    auto rect = font_normal_bold.getStringBoundingBox( ss.str(), offset.x, offset.y);
    offset.x -= 0.5 * rect.width;
    font_normal_bold.drawString( ss.str(), offset.x, offset.y);


}
void ImageLoader::drawDetails(){
    ofxJSONElement meta = *metadata["meta"];
    std::string location_key = meta["location"].asString( );
    auto location = Location::locations[ofToInt(location_key)-1];

    float lh = font_normal_bold.getLineHeight();
    glm::vec2 offset{ 10, font_normal_bold.getLineHeight()};

    ostringstream ss;

    ss << location.name << " (" << location.future_date << ")"; 
    font_large_bold.drawString( ss.str(), offset.x, offset.y); offset.y += 1.0 * lh;
    
    ss.str("");
    ss << "Group: " << this->job.gid << " [" << location.coords << "]";
    font_small_regular.drawString( ss.str(), offset.x, offset.y); offset.y += 1.5 * lh;
    
    std::string scenario = location.scenario;
    ofStringReplace(scenario, "<br>", "\n");
    scenario = "Some text.";
    font_normal_regular.drawString(scenario, offset.x, offset.y); offset.y += 1.5 * lh;

}
