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
    ci::fs::path moviePath = ci::app::getAssetPath("catVideo_640x480.mov");
    mVideoDisplayer = VideoGl::create();
    
    try {
        ci::qtime::MovieGlRef movieRef;
        movieRef = ci::qtime::MovieGl::create(moviePath);
        mVideoDisplayer->setMovieRef(movieRef);
    } catch (...) {
        ci::app::console() << "PlayerNode::setup: Failed to load movie" << std::endl;
    }

    getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&PlayerNode::onMouseClick, this));
    addChild(mVideoDisplayer);
    
    setDrawBounds(true);
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

void PlayerNode::onMouseClick()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->stop();
    } else {
        mVideoDisplayer->getMovieRef()->play();
    }
}

