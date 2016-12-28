#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"

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
	
	SceneRef mScene;
	ViewControllerRef mViewController;
};

void VideoSampleAdvancedApp::setup()
{
    setWindowSize(1024, 768);
	mViewController = ViewController::create();
	mScene = Scene::create(mViewController);
    mViewController->getView()->addChild(VideoSampleAdvanced::create());
}

void VideoSampleAdvancedApp::update()
{
    mScene->update();
}

void VideoSampleAdvancedApp::draw()
{
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( VideoSampleAdvancedApp, RendererGl )
