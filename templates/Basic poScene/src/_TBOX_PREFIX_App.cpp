#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "_TBOX_PREFIX_.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class _TBOX_PREFIX_App
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void _TBOX_PREFIX_App::setup()
{
    scene = Scene::create(_TBOX_PREFIX_::create());
}

void _TBOX_PREFIX_App::update()
{
    scene->update();
}

void _TBOX_PREFIX_App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( _TBOX_PREFIX_App, RendererGl )
