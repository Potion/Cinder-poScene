#include "VideoSample.h"

#include "cinder/qtime/QuickTimeGl.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

#include "poScene/ShapeView.h"

using namespace po::scene;

VideoSampleRef VideoSample::create()
{
    VideoSampleRef node(new VideoSample());
    node->setup();
    return node;
}

VideoSample::VideoSample()
{
}

void VideoSample::setup()
{
    //  create a player that loops video
    mVideo = po::scene::VideoViewGl::create();
    ci::fs::path moviePath = ci::app::getAssetPath("test.mp4");
    ci::app::console() << moviePath << std::endl;
    try {
        ci::qtime::MovieGlRef movieRef;
        
        movieRef = ci::qtime::MovieGl::create(moviePath);
        mVideo->setMovieRef(movieRef);
        mVideo->getMovieRef()->setLoop(true);
        mVideo->getMovieRef()->play();
    } catch (...) {
        ci::app::console() << "PlayerView::setup: Failed to load movie" << std::endl;
    }
    
    mVideo->setAlignment(po::scene::Alignment::CENTER_CENTER);
    mVideo->setPosition(ci::app::getWindowCenter());
    addChild(mVideo);
    
    //  create the spin button, and connect the mouse signal
    ci::gl::TextureRef spinnerTex = ci::gl::Texture::create(loadImage(ci::app::loadAsset("spinArrows.png")));
    ShapeViewRef spinner = ShapeView::create(spinnerTex);
    spinner->setAlignment(po::scene::Alignment::CENTER_CENTER);
    spinner->setPosition(ci::app::getWindowWidth() * 0.5, ci::app::getWindowHeight() * 0.85);
    spinner->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&VideoSample::spinPlayer, this));
    addChild(spinner);
}

void VideoSample::spinPlayer()
{
    ci::app::timeline().apply(&mVideo->getRotationAnim(), (float)M_PI * 2.0f, 2.0f).finishFn(std::bind(&VideoSample::resetPlayerRotation, this));
}

void VideoSample::resetPlayerRotation()
{
    mVideo->setRotation(0.0f);
}
