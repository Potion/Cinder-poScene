#include "PlayerNode.h"

using namespace po::scene;

PlayerNodeRef PlayerNode::create()
{
    PlayerNodeRef node (new PlayerNode());
    node->setup();
    return node;
}

PlayerNode::PlayerNode()
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
    
    addChild(mVideoDisplayer);
}

void PlayerNode::update()
{
    NodeContainer::update();
    
    if (!mVideoDisplayer->getMovieRef()) return;
    
    //  when movie finishes, stop and go back to the beginning
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        if (mVideoDisplayer->getMovieRef()->isDone()) {
            mVideoDisplayer->getMovieRef()->stop();
            mVideoDisplayer->getMovieRef()->seekToStart();
        }
    }
}

void PlayerNode::setPrimaryMovie(po::scene::VideoGlRef video)
{
    mVideoDisplayer = video;
}

