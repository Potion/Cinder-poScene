#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "ViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class TextSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
	SceneRef mScene;
	ViewControllerRef mViewController;
};

void TextSampleApp::setup()
{
	mViewController = sample::ViewController::create();
	mScene = Scene::create(mViewController);
}

void TextSampleApp::update()
{
    mScene->update();
}

void TextSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( TextSampleApp, RendererGl )
