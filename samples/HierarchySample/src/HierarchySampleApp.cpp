#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"

#include "MainViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class HierarchySampleApp
	: public App
{
	public:
		void setup();
		void update();
		void draw();

		SceneRef mScene;
		sample::ViewControllerRef mViewController;
};

void HierarchySampleApp::setup()
{
	mViewController = sample::ViewController::create();
	mScene = Scene::create( mViewController );
}

void HierarchySampleApp::update()
{
	mScene->update();
}

void HierarchySampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	mScene->draw();
}

CINDER_APP( HierarchySampleApp, RendererGl )
