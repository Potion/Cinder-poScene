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
    gl::SaveFramebufferBinding bindingSaver;
    
    scene = po::Scene::create(RenderToFBO::create());
    ci::gl::Fbo::Format format;
    format.setSamples(4);
    format.setColorInternalFormat(GL_RGBA);
    mFbo = gl::Fbo( scene->getRootNode()->getWidth(), scene->getRootNode()->getHeight(), format);
    
}

void RenderToFBOApp::update()
{
    
    gl::SaveFramebufferBinding bindingSaver;
    
    //scene->update();
    scene->update();
    
    gl::setViewport(mFbo.getBounds());
    // in the app class:
    ci::CameraOrtho mCamera;
    
    // in setup() and/or resize():
    mCamera.setOrtho( 0, mFbo.getWidth(), mFbo.getHeight(), 0, -1, 1 );
    
    // in draw():
    gl::setMatrices( mCamera );
    mFbo.bindFramebuffer();
    gl::clear(Color(255,0,0));
    scene->draw();
}

void RenderToFBOApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
   scene->draw();
    mFbo.bindTexture();
    gl::color(255,0,255);
//    gl::scale(.5f,.5f,1.f);
    float ratio = (float)mFbo.getHeight()/(float)mFbo.getWidth();
    gl::draw(mFbo.getTexture(), Rectf(0,0,app::getWindowWidth(), app::getWindowWidth() * ratio));
    mFbo.unbindTexture();
    
}

CINDER_APP_NATIVE( RenderToFBOApp, RendererGl )
