#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"

#include "poTextBoxApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TextBoxApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    po::SceneRef scene;
};

void TextBoxApp::setup()
{
    ci::gl::enableAlphaBlending();
    
    poTextBoxAppRef app = poTextBoxApp::create();
    scene = po::Scene::create(app);
    
}

void TextBoxApp::mouseDown( MouseEvent event )
{
}

void TextBoxApp::update()
{
    scene->update();
}

void TextBoxApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    scene->draw();
}

CINDER_APP_NATIVE( TextBoxApp, RendererGl )
