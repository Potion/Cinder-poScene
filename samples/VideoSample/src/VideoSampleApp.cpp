#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "ViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoSampleApp
: public App
{
public:
    void setup();
    void update();
    void draw();
	
	po::scene::SceneRef mScene;
	sample::ViewControllerRef mViewController;
};

void VideoSampleApp::setup()
{
    setWindowSize(1024, 768);
	mViewController = sample::ViewController::create();
	mScene = po::scene::Scene::create(mViewController);
}

void VideoSampleApp::update()
{
    mScene->update();
}

void VideoSampleApp::draw()
{
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( VideoSampleApp, RendererGl )
