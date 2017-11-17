#include "PlayerController.h"

#include "cinder/ImageIo.h"
#include "cinder/qtime/QuickTime.h"

#include "poScene/ShapeView.h"

using namespace po::scene;

PlayerControllerRef PlayerController::create()
{
    PlayerControllerRef node (new PlayerController());
    node->setup();
    return node;
}

PlayerController::PlayerController()
: mCurrentDuration(0.f)
{}

void PlayerController::setup()
{
    //  create reference to video
    //  because will actually be playing thumbnails, don't need to add as a child,
    //  but still need a reference to the videos themselves
    mVideoReference = VideoViewGl::create();
    
    //  create and add the buttons
    ci::gl::TextureRef playTex = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("play.png")));
    ci::gl::TextureRef pauseTex = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("pause.png")));
    ShapeViewRef playShape = ShapeView::create(playTex);
    ShapeViewRef pauseShape = ShapeView::create(pauseTex);
    mPlayButton = PlayerButton::create(playShape);
    mPauseButton = PlayerButton::create(pauseShape);
    
    //  the width of player (and movies) when playing will be 640px
    //  place buttons with that in mind
    
    mPlayButton->setPosition((640 / 2) - 50 - mPlayButton->getWidth(), 0);
    mPauseButton->setPosition((640 / 2) + 50, 0);
    
    addSubview(mPlayButton);
    addSubview(mPauseButton);
    
    mPlayButton->getButtonSignal().connect(std::bind(&PlayerController::getPlaySignal, this));
    mPauseButton->getButtonSignal().connect(std::bind(&PlayerController::getPauseSignal, this));
    
    //  create and add the scrubber
    mScrubber = Scrubber::create();
    mScrubber->getScrubberSignal().connect(std::bind(&PlayerController::getScrubberSignal, this, std::placeholders::_1));
    mScrubber->setPosition(0, mPlayButton->getHeight() + 25);
    addSubview(mScrubber);
}

void PlayerController::update()
{
	po::scene::View::update();
    //  if we don't have a movie reference, stop
    if (!mVideoReference->getMovieRef()) return;

    //  while movie is playing, update the scrubber
    //  when movie finishes, stop and go back to the beginning
    if (mVideoReference->getMovieRef()->isPlaying()) {

        //  send signal to scrubber
        float currentTime = mVideoReference->getMovieRef()->getCurrentTime();
        float currentPct = currentTime / mCurrentDuration;
        mScrubber->setHandlePosition(currentPct);
        
        if (mVideoReference->getMovieRef()->isDone()) {
            mVideoReference->getMovieRef()->stop();
            mVideoReference->getMovieRef()->seekToStart();
            mScrubber->setHandlePosition(0.f);
        }
    }
}

void PlayerController::setPrimaryMovie(po::scene::VideoViewGlRef video)
{
    //  stop the current video if one's already there
    if (mVideoReference->getMovieRef()) {
        if (mVideoReference->getMovieRef() != video->getMovieRef()) {
            mVideoReference->getMovieRef()->stop();
        }
    }
    
    //  change the movie reference
    mVideoReference->setMovieRef(video->getMovieRef());
    mCurrentDuration = video->getMovieRef()->getDuration();
    
    //  update scrubber
    float currentTime = mVideoReference->getMovieRef()->getCurrentTime();
    float currentPct = currentTime / mCurrentDuration;
    mScrubber->setHandlePosition(currentPct);
}

void PlayerController::getPlaySignal()
{
    if (!mVideoReference->getMovieRef()) return;
    if (!mVideoReference->getMovieRef()->isPlaying()) {
        mVideoReference->getMovieRef()->play();
    }
}

void PlayerController::getPauseSignal()
{
    if (!mVideoReference->getMovieRef()) return;
    if (mVideoReference->getMovieRef()->isPlaying()) {
        mVideoReference->getMovieRef()->stop();
    }
}

void PlayerController::getScrubberSignal(float pct)
{
    if (!mVideoReference->getMovieRef()) return;
    float scrubPoint = pct * mCurrentDuration;
    mVideoReference->getMovieRef()->seekToTime(scrubPoint);
}
