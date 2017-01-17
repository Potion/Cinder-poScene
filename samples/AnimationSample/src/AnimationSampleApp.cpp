#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "AnimationSampleViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class AnimationSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef mScene;
	ViewControllerRef mViewController;
};

void AnimationSampleApp::setup()
{
	mViewController = AnimationSampleViewController::create();
    mScene = Scene::create( mViewController );
}

void AnimationSampleApp::update()
{
    mScene->update();
}

void AnimationSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( AnimationSampleApp, RendererGl )
