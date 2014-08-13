#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "poScene.h"
#include "poImageTestApp.h"

class ImageTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    void keyDown(KeyEvent event);
    
    po::SceneRef scene;
    
    Font mFont;
};

void ImageTestApp::setup()
{
    ci::gl::enableAlphaBlending();
    
    scene = po::Scene::create(poImageTestApp::create());
    
}

void ImageTestApp::mouseDown( MouseEvent event )
{
}

void ImageTestApp::update()
{
    if(scene) scene->update();
}

void ImageTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if(scene) scene->draw();
    
    gl::pushMatrices();
    gl::translate(1,1);
    gl::drawString( "Framerate: " + toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color(0,0,0), mFont );
    gl::popMatrices();
    
    gl::enableAlphaBlending();
    gl::drawString( "Framerate: " + toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color(1,0,0), mFont );
}

void ImageTestApp::keyDown(cinder::app::KeyEvent event) {
    poImageTestAppRef app = poImageTestApp::create();
    scene = po::Scene::create(app);
}

CINDER_APP_NATIVE( ImageTestApp, RendererGl )
