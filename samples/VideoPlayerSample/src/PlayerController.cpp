#include "PlayerController.h"
#include "poShape.h"

using namespace po::scene;

PlayerControllerRef PlayerController::create()
{
    PlayerControllerRef node (new PlayerController());
    node->setup();
    return node;
}

PlayerController::PlayerController()
: mIsPlaying(false)
, mCurrentDuration(0.f)
{}

void PlayerController::setup()
{
    //  create video player without adding video
    //  because will actually be playing thumbnails, don't need to add as a child,
    //  but still need a reference to the videos themselves
    mVideoReference = VideoGl::create();
    
    //  create and add the buttons
    ci::gl::TextureRef playTex = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("play.png")));
    ci::gl::TextureRef pauseTex = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("pause.png")));
    ShapeRef playShape = Shape::create(playTex);
    ShapeRef pauseShape = Shape::create(pauseTex);
    mPlayButton = PlayerButton::create(playShape);
    mPauseButton = PlayerButton::create(pauseShape);
    
    //  set positions so won't affect width of node when later resize for movies
    mPlayButton->setPosition(20, 20);
    mPauseButton->setPosition(20, 20);

    mPlayButton->setAlignment(Alignment::CENTER_CENTER);
    mPauseButton->setAlignment(Alignment::CENTER_CENTER);

    mPlayButton->setVisible(false);
    mPauseButton->setVisible(false);
    
    addChild(mPlayButton);
    addChild(mPauseButton);
    
    mPlayButton->getButtonSignal().connect(std::bind(&PlayerController::getPlaySignal, this));
    mPauseButton->getButtonSignal().connect(std::bind(&PlayerController::getPauseSignal, this));
    
    //  create and add the scrubber
    mScrubber = Scrubber::create();
    mScrubber->setVisible(false);
    mScrubber->getScrubberSignal().connect(std::bind(&PlayerController::getScrubberSignal, this, std::placeholders::_1));
    addChild(mScrubber);
    
}

void PlayerController::update()
{
    NodeContainer::update();
    
    if (!mVideoReference->getMovieRef()) return;
    
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

void PlayerController::setPrimaryMovie(po::scene::VideoGlRef video)
{
    //  stop the current video if one's already there
    if (mVideoReference->getMovieRef()) {
        if (mVideoReference->getMovieRef() != video->getMovieRef()) {
            mVideoReference->getMovieRef()->stop();
        }
    }
    mVideoReference->setMovieRef(video->getMovieRef());
    mCurrentDuration = video->getMovieRef()->getDuration();
    mPlayButton->setVisible(true);
    mPauseButton->setVisible(true);
    mScrubber->setVisible(true);
    mPlayButton->setPosition((mVideoReference->getWidth() / 2) - 50.f, mVideoReference->getHeight() + 50);
    mPauseButton->setPosition((mVideoReference->getWidth() / 2) + 50.f, mVideoReference->getHeight() + 50);
    mScrubber->setPosition(0.f, mPlayButton->getPosition().y + 50);
    
    //  send signal to scrubber
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
