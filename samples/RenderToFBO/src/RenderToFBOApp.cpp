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

    //Create FBO
    ci::gl::Fbo::Format format;
//    format.setSamples(4);
//    format.setColorInternalFormat(GL_RGBA);
    
    mFbo = gl::Fbo( scene->getRootNode()->getWidth(), scene->getRootNode()->getHeight());
    //mFbo = gl::Fbo( getWindowWidth(), getWindowHeight());
}

void RenderToFBOApp::update()
{
    //Save our buffer
    gl::SaveFramebufferBinding bindingSaver;
    
    //Update the scene
    scene->update();
    
    gl::setViewport( mFbo.getBounds() );
    CameraOrtho cam;
    cam.setOrtho( 0, mFbo.getWidth(), mFbo.getHeight(), 0, -1, 1 );
    gl::setMatrices(cam);
    
    // Draw our scene; it takes care of it's own Ortho camera settings
    mFbo.bindFramebuffer();
    gl::clear(Color(255,0,0));
    scene->draw();
    
    gl::setViewport( getWindowBounds() );
}

void RenderToFBOApp::draw()
{
    //Draw our Scene
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::setMatrices(scene->getCamera());
    scene->draw();
    
    //Draw the FBO
    mFbo.bindTexture();
    gl::color(255,255,255);

    float ratio = (float)mFbo.getHeight()/(float)mFbo.getWidth();
    gl::Texture tex = mFbo.getTexture();
    tex.setFlipped(true);
    gl::draw(tex, Rectf(0,0,app::getWindowWidth(), (app::getWindowWidth() * ratio)));
    //gl::draw(mFbo.getTexture(), Rectf(0,0,mFbo.getWidth(), mFbo.getHeight()));
    mFbo.unbindTexture();
    
}

CINDER_APP_NATIVE( RenderToFBOApp, RendererGl )
