#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "ViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BoundsSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    po::scene::SceneRef mScene;
	sample::ViewControllerRef mViewController;
};

void BoundsSampleApp::setup()
{
    setWindowSize(800, 600);
	mViewController	= sample::ViewController::create();
    mScene = po::scene::Scene::create(mViewController);
}

void BoundsSampleApp::update()
{
    mScene->update();
}

void BoundsSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.0, 0.0f, 0.0 ) );
    mScene->draw();
}

CINDER_APP( BoundsSampleApp, RendererGl )
