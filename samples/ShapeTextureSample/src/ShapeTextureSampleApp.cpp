#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"
#include "poScene/ViewController.h"
#include "poScene/Scene.h"

#include "ShapeTextureSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class ShapeTextureSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
	
	SceneRef mScene;
	ViewControllerRef mViewController;
};

void ShapeTextureSampleApp::setup()
{
    setWindowSize(1024, 768);
	mViewController = ViewController::create();
	mScene = Scene::create(mViewController);
	mViewController->getView()->addChild(ShapeTextureSample::create());
}

void ShapeTextureSampleApp::update()
{
    mScene->update();
}

void ShapeTextureSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( ShapeTextureSampleApp, RendererGl )
