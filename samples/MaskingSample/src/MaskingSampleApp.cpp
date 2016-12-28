#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "MaskingSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class MaskingSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef mScene;
	ViewControllerRef mViewController;
};

void MaskingSampleApp::setup()
{
	setWindowSize(683, 1024);
	mViewController = ViewController::create();
    mScene = Scene::create(mViewController);
	mViewController->getView()->addChild(MaskingSample::create());
}

void MaskingSampleApp::update()
{
    mScene->update();
}

void MaskingSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( MaskingSampleApp, RendererGl )
