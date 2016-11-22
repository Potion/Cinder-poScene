#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "VideoSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class VideoSampleApp
: public App
{
public:
    void setup();
    void update();
    void draw();
    
    SceneRef scene;
};

void VideoSampleApp::setup()
{
    setWindowSize(1024, 768);
    scene = Scene::create(VideoSample::create());
}

void VideoSampleApp::update()
{
    scene->update();
}

void VideoSampleApp::draw()
{
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP( VideoSampleApp, RendererGl )
