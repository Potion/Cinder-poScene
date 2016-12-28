#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "poScene/ViewController.h"
#include "BoundsSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class BoundsSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef mScene;
	ViewControllerRef mViewController;
	
};

void BoundsSampleApp::setup()
{
    setWindowSize(800, 600);
	mViewController	= ViewController::create();
    mScene = Scene::create(mViewController);
	mViewController->getView()->addChild(BoundsSample::create());
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
