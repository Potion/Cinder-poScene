#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "poScene.h"
#include "RenderToFBO.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RenderToFBOApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
    
    po::SceneRef scene;
    gl::Fbo mFbo;
};


void RenderToFBOApp::setup()
{
    gl::enableAlphaBlending();
    //app::setWindowSize(2000, 2000);
    //gl::SaveFramebufferBinding bindingSaver;
    
    scene = po::Scene::create(RenderToFBO::create());
    scene->setAutoCam(false);
}

void RenderToFBOApp::update()
{
    scene->update();
}

void RenderToFBOApp::draw()
{
    //Draw our Scene
	gl::clear( Color( 0, 0, 0 ) );
    ci::gl::pushMatrices();
    ci::gl::setMatricesWindow(getWindowWidth(), getWindowHeight());
    //ci::gl::translate(ci::Vec2f(-200.0f, 200.0f));
    ci::gl::scale(0.8f, 0.8f);
    
    scene->draw();
    
    ci::gl::popModelView();
}

CINDER_APP_NATIVE( RenderToFBOApp, RendererGl )
