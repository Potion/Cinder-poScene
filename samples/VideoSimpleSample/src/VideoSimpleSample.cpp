#include "VideoSimpleSample.h"
#include "poShape.h"

using namespace po::scene;

VideoSimpleSampleRef VideoSimpleSample::create() 
{
    VideoSimpleSampleRef node(new VideoSimpleSample());
    node->setup();
    return node;
}

VideoSimpleSample::VideoSimpleSample()
{
}

void VideoSimpleSample::setup() 
{
    //  We've built very simple play/stop commands into the player itself, so we just create and add it
    mPlayer = PlayerNode::create();
    mPlayer->setAlignment(po::scene::Alignment::CENTER_CENTER);
    mPlayer->setPosition(ci::app::getWindowCenter());
    addChild(mPlayer);
    
    //  For the spinner, in contrast, we connect the mouse signal from here
    ci::gl::TextureRef spinnerTex = ci::gl::Texture::create(loadImage(ci::app::loadAsset("spinArrows.png")));
    ShapeRef spinner = Shape::create(spinnerTex);
    spinner->setAlignment(po::scene::Alignment::CENTER_CENTER);
    spinner->setPosition(ci::app::getWindowWidth() * 0.15, ci::app::getWindowHeight() / 2);
    spinner->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&VideoSimpleSample::spinPlayer, this));
    addChild(spinner);
}

void VideoSimpleSample::spinPlayer()
{
    float currentRot = mPlayer->getRotation();
    if (currentRot > 359.0) {
        mPlayer->setRotation(0.f);
    }
    
    ci::app::timeline().apply(&mPlayer->getRotationAnim(), 360.f, 2.0f);
}
