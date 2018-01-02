#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "poScene/Scene.h"
#include "poScene/ViewController.h"
#include "_TBOX_PREFIX_.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class _TBOX_PREFIX_App
: public App 
{
  public:
	void setup() override;
	void update() override;
	void draw() override;
    
    SceneRef mScene;
    ViewController mController;
};

void _TBOX_PREFIX_App::setup()
{
    mScene = Scene::create(_TBOX_PREFIX_::create());
}

void _TBOX_PREFIX_App::update()
{
    mScene->update();
}

void _TBOX_PREFIX_App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mScene->draw();
}

CINDER_APP( _TBOX_PREFIX_App, RendererGl )
