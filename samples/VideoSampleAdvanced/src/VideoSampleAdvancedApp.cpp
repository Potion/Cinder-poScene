#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene.h"
#include "VideoSampleAdvanced.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class VideoSampleAdvancedApp
: public App
{
public:
    void setup();
    void update();
    void draw();
    
    SceneRef scene;
};

void VideoSampleAdvancedApp::setup()
{
    setWindowSize(1024, 768);
    scene = Scene::create(VideoSampleAdvanced::create());
}

void VideoSampleAdvancedApp::update()
{
    scene->update();
}

void VideoSampleAdvancedApp::draw()
{
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP( VideoSampleAdvancedApp, RendererGl )
