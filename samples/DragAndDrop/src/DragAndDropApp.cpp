#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "poScene/Scene.h"
#include "poScene/DragAndDrop.h"

#include "ViewController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DragAndDropApp
: public App 
{
  public:
	void setup() override;
	void update() override;
	void draw() override;
    
    po::scene::SceneRef scene;
	sample::ViewControllerRef viewController;
};

void DragAndDropApp::setup()
{
	viewController = sample::ViewController::create();
    scene = po::scene::Scene::create(viewController);
}

void DragAndDropApp::update()
{
    scene->update();
}

void DragAndDropApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP( DragAndDropApp, RendererGl )
