#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"

#include "MainViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoSampleAdvancedApp
	: public App
{
	public:
		void setup();
		void update();
		void draw();

		po::scene::SceneRef mScene;
		sample::ViewControllerRef mViewController;
};

void VideoSampleAdvancedApp::setup()
{
	setWindowSize( 1024, 768 );
	mViewController = sample::ViewController::create();
	mScene = po::scene::Scene::create( mViewController );
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
