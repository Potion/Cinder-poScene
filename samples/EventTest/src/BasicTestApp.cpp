#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"

#include "poScene.h"
#include "TestNode.h"

#include "poShape.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class BasicTestApp : public AppNative {
public:
	
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
    
    void mouseDown(MouseEvent event);
    void mouseDrag(MouseEvent event);
    void mouseUp(MouseEvent event);
    
    po::scene::SceneRef scene;
    
    ci::Font mFont;
};

void BasicTestApp::prepareSettings(cinder::app::AppBasic::Settings *settings)
{
    //settings->enableMultiTouch();
}

void BasicTestApp::setup()
{
    GLint texSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
    
    std::cout << texSize << std::endl;
    
    //This
//    scene = po::Scene::create();
//    scene->setRootNode(TestNode::create());
    
    //Or this
    scene = po::scene::Scene::create(TestNode::create());
    
    mFont = Font( "Geneva", 18.0f );
}

void BasicTestApp::update()
{
    //Update our scene
    scene->update();
}

void BasicTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    //Draw our scene
    scene->draw();
    
    gl::drawString( "Framerate: " + ci::toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

void BasicTestApp::mouseDown(ci::app::MouseEvent event)
{
//    std::cout << "Touch Down!" << std::endl;
//    std::vector<ci::app::TouchEvent::Touch> touches;
//    touches.push_back(ci::app::TouchEvent::Touch(event.getPos(), event.getPos(), 0, 0, NULL));
//    
//    ci::app::TouchEvent e(ci::app::getWindow(), touches);
//    ci::app::getWindow()->emitTouchesBegan(&e);
}

void BasicTestApp::mouseDrag(MouseEvent event)
{
//    std::vector<ci::app::TouchEvent::Touch> touches;
//    touches.push_back(ci::app::TouchEvent::Touch(event.getPos(), event.getPos(), 0, 0, NULL));
//    
//    ci::app::TouchEvent e(ci::app::getWindow(), touches);
//    ci::app::getWindow()->emitTouchesMoved(&e);
}

void BasicTestApp::mouseUp(MouseEvent event)
{
//    std::vector<ci::app::TouchEvent::Touch> touches;
//    touches.push_back(ci::app::TouchEvent::Touch(event.getPos(), event.getPos(), 0, 0, NULL));
//    
//    ci::app::TouchEvent e(ci::app::getWindow(), touches);
//    ci::app::getWindow()->emitTouchesEnded(&e);
}

CINDER_APP_NATIVE( BasicTestApp, RendererGl )
