//
//  PlayerNode.cpp
//  VideoSample
//
//  Created by Jennifer Presto on 6/10/15.
//
//

#include "PlayerNode.h"

using namespace po::scene;

PlayerNodeRef PlayerNode::create()
{
    PlayerNodeRef node (new PlayerNode());
    node->setup();
    return node;
}

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
    ci::gl::TextureRef arrowTex = ci::gl::Texture::create(loadImage(ci::app::loadAsset("skipArrows.png")));
    
    //  create and add the skip forward button
    mSkipForward = Shape::create(arrowTex);
    mSkipForward->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&PlayerNode::onClickForward, this));
    addChild(mSkipForward);
    
    //  create and add the skip reverse button
    mSkipBackward = Shape::create(arrowTex);
    mSkipBackward->setScale(-1, 1);
    mSkipBackward->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&PlayerNode::onClickBackward, this));
    addChild(mSkipBackward);
    
    //  position all the elements within the node so they line up with each other correctly
    //  Note that each one has, by default, a top-left alignment
    mSkipBackward->setPosition(ci::Vec2f(mSkipBackward->getWidth(), (mVideoDisplayer->getHeight() / 2) - mSkipBackward->getHeight() / 2 ) );
    mVideoDisplayer->setPosition(mSkipBackward->getWidth(), 0);
    mSkipForward->setPosition(ci::Vec2f(mSkipBackward->getWidth() + mVideoDisplayer->getWidth(), (mVideoDisplayer->getHeight() / 2) - mSkipForward->getHeight() / 2 ) );
    
    //  uncomment to see the boundaries of the entire video player node
    //setDrawBounds(true);
}

void PlayerNode::update()
{
    NodeContainer::update();
    if (!mVideoDisplayer->getMovieRef()) return;
    
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        if (mVideoDisplayer->getMovieRef()->isDone()) {
            mVideoDisplayer->getMovieRef()->stop();
            mVideoDisplayer->getMovieRef()->seekToStart();
        }
    }
}

void PlayerNode::onClickVideo()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->stop();
    } else {
        mVideoDisplayer->getMovieRef()->play();
    }
}

void PlayerNode::onClickForward()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    
    float time = mVideoDisplayer->getMovieRef()->getCurrentTime();
    time += 3.0f;
    //  if we go fast-forward past the end of the movie, go to the beginning
    if (time > mVideoDisplayer->getMovieRef()->getDuration()) {
        //mVideoDisplayer->getMovieRef()->stop();
        mVideoDisplayer->getMovieRef()->seekToStart();
    } else {
        mVideoDisplayer->getMovieRef()->seekToTime(time);
    }
    
    
    std::cout << "PlayerNode::onClickForward: Skipping forward" << std::endl;
}

void PlayerNode::onClickBackward()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    
    float time = mVideoDisplayer->getMovieRef()->getCurrentTime();
    time -= 3.0f;
    if (time < 0.f) {
        mVideoDisplayer->getMovieRef()->seekToStart();
    } else {
        mVideoDisplayer->getMovieRef()->seekToTime(time);
    }
    
    std::cout << "PlayerNode::onClickBackward: Skipping backward" << std::endl;
}
