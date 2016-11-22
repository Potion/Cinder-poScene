#include "PlayerView.h"

using namespace po::scene;

PlayerViewRef PlayerView::create()
{
    PlayerViewRef node (new PlayerView());
    node->setup();
    return node;
}

PlayerView::PlayerView() : mTexture(NULL)
{}

void PlayerView::setup()
{
    //  create and add the video displayer
    //ci::fs::path moviePath = ci::app::getAssetPath("phoebe.mp4");
	ci::fs::path moviePath = ci::app::getAssetPath("test.mp4");
	mVideoDisplayer = VideoViewGl::create();
    
    try {
        ci::qtime::MovieGlRef movieRef;
        movieRef = ci::qtime::MovieGl::create(moviePath);
        mVideoDisplayer->setMovieRef(movieRef);
        mVideoDisplayer->getMovieRef()->play();
    } catch (...) {
        ci::app::console() << "PlayerView::setup: Failed to load movie" << std::endl;
    }
	//mTexture->reset();

    getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&PlayerView::clickVideo, this));
    addChild(mVideoDisplayer);
}

void PlayerView::update()
{   
	if (!mVideoDisplayer->getMovieRef())
	{
		ci::app::console() << "PlayerView::Failed to get movie ref" << std::endl;
		return;
	}
	
    //  when movie finishes, stop and go back to the beginning
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        if (mVideoDisplayer->getMovieRef()->isDone()) {
            mVideoDisplayer->getMovieRef()->stop();
            mVideoDisplayer->getMovieRef()->seekToStart();
        }
    }
}

void PlayerView::clickVideo()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->stop();
    } else {
        mVideoDisplayer->getMovieRef()->play();
    }
}