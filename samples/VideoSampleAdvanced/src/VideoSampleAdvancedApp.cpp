#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "VideoSampleAdvanced.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class VideoSampleAdvancedApp
: public AppNative
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

CINDER_APP_NATIVE( VideoSampleAdvancedApp, RendererGl )
