#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    po::SceneRef scene;
};

void ImageTestApp::setup()
{
    ci::gl::enableAlphaBlending();
    
    poTextBoxAppRef app = poTextBoxApp::create();
    scene = po::Scene::create(app);
}

void ImageTestApp::mouseDown( MouseEvent event )
{
}

void ImageTestApp::update()
{
}

void ImageTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( ImageTestApp, RendererGl )
