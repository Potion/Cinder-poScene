#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "AlignmentSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class AlignmentSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef mScene;
	AlignmentSampleViewControllerRef mViewController;
};

void AlignmentSampleApp::setup()
{
	mViewController = AlignmentSampleViewController::create();
    mScene = Scene::create(mViewController);
}

void AlignmentSampleApp::update()
{
    mScene->update();
}

void AlignmentSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( AlignmentSampleApp, RendererGl )
