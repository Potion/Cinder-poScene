#include "PlayerNode.h"

using namespace po::scene;

PlayerNodeRef PlayerNode::create()
{
    PlayerNodeRef node (new PlayerNode());
    node->setup();
    return node;
}

PlayerNode::PlayerNode()
: mIsReversed(false)
, mActiveArrowColor(ci::Color(122.f/255, 201.f/255, 67.f/255))
{}

void PlayerNode::setup()
{
    //  create and add the video displayer
    ci::fs::path moviePath = ci::app::getAssetPath("phoebe.mp4");
    mVideoDisplayer = VideoGl::create();
    
    try {
        ci::qtime::MovieGlRef movieRef;
        movieRef = ci::qtime::MovieGl::create(moviePath);
        mVideoDisplayer->setMovieRef(movieRef);
    } catch (...) {
        ci::app::console() << "PlayerNode::setup: Failed to load movie" << std::endl;
    }

    mVideoDisplayer->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&PlayerNode::onClickVideo, this));
    addChild(mVideoDisplayer);
    
    //  load the image we'll use for both button nodes
    ci::gl::TextureRef arrowTex = ci::gl::Texture::create(loadImage(ci::app::loadAsset("playArrow.png")));
    
    //  create and add the forward button
    mForwardArrow = Shape::create(arrowTex);
    mForwardArrow->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&PlayerNode::onClickForward, this));
    addChild(mForwardArrow);
    
    //  create and add the reverse button
    mBackwardArrow = Shape::create(arrowTex);
    mBackwardArrow->setScale(-1, 1);
    mBackwardArrow->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&PlayerNode::onClickBackward, this));
    addChild(mBackwardArrow);
    
    //  position all the elements within the node so they line up with each other correctly
    //  Note that each one has, by default, a top-left alignment
    mBackwardArrow->setPosition(ci::Vec2f(mBackwardArrow->getWidth(), (mVideoDisplayer->getHeight() / 2) - mBackwardArrow->getHeight() / 2 ) );
    mVideoDisplayer->setPosition(mBackwardArrow->getWidth(), 0);
    mForwardArrow->setPosition(ci::Vec2f(mBackwardArrow->getWidth() + mVideoDisplayer->getWidth(), (mVideoDisplayer->getHeight() / 2) - mForwardArrow->getHeight() / 2 ) );
    
    //  uncomment to see the boundaries of the entire video player node
    //setDrawBounds(true);
}

void PlayerNode::update()
{
    NodeContainer::update();
    
    if (!mVideoDisplayer->getMovieRef()) return;

    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        if (mVideoDisplayer->getMovieRef()->isDone()) {
            setColorsNotPlaying();
            mVideoDisplayer->getMovieRef()->stop();
            mVideoDisplayer->getMovieRef()->seekToStart();
            mIsReversed = false;
        }
    }
}

void PlayerNode::onClickVideo()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->stop();
        setColorsNotPlaying();
    } else {
        mVideoDisplayer->getMovieRef()->play();
        if (mIsReversed) {
            setColorsPlayingBackward();
        } else {
            setColorsPlayingForward();
        }
    }
}

void PlayerNode::onClickForward()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    
    mIsReversed = false;
    if (!mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->play();
    }

    mVideoDisplayer->getMovieRef()->setRate(1.f);
    setColorsPlayingForward();
}

void PlayerNode::onClickBackward()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    if (mVideoDisplayer->getMovieRef()->getCurrentTime() < 0.01f) return;
    
    mIsReversed = true;
    if (!mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->play();
    }
    
    mVideoDisplayer->getMovieRef()->setRate(-1.f);
    setColorsPlayingBackward();
}

void PlayerNode::setColorsPlayingForward()
{
    mForwardArrow->setFillColor(mActiveArrowColor);
    mBackwardArrow->setFillColor(ci::Color(1, 1, 1));
}

void PlayerNode::setColorsPlayingBackward()
{
    mForwardArrow->setFillColor(ci::Color(1, 1, 1));
    mBackwardArrow->setFillColor(mActiveArrowColor);
}

void PlayerNode::setColorsNotPlaying()
{
    mForwardArrow->setFillColor(ci::Color(1, 1, 1));
    mBackwardArrow->setFillColor(ci::Color(1, 1, 1));
}
