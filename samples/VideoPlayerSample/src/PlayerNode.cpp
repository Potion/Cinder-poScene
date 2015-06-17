#include "PlayerNode.h"
#include "poShape.h"

using namespace po::scene;

PlayerNodeRef PlayerNode::create()
{
    PlayerNodeRef node (new PlayerNode());
    node->setup();
    return node;
}

PlayerNode::PlayerNode()
: mIsPlaying(false)
{}

void PlayerNode::setup()
{
    //  add video player without adding video
    mVideoDisplayer = VideoGl::create();
    addChild(mVideoDisplayer);
    
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
    
    mPlayButton->getButtonSignal().connect(std::bind(&PlayerNode::getPlaySignal, this));
    mPauseButton->getButtonSignal().connect(std::bind(&PlayerNode::getPauseSignal, this));
    
    setDrawBounds(true);
    
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
    //  stop video if one's already there
    if (mVideoDisplayer->getMovieRef()) {
        if (mVideoDisplayer->getMovieRef() != video->getMovieRef()) {
            mVideoDisplayer->getMovieRef()->stop();
        }
    }
    mVideoDisplayer->setMovieRef(video->getMovieRef());
    mPlayButton->setVisible(true);
    mPauseButton->setVisible(true);
    mPlayButton->setPosition(getWidth() / 2 - getWidth() * 0.05, mVideoDisplayer->getHeight() + 50);
    mPauseButton->setPosition(getWidth() / 2 + getWidth() * 0.05, mVideoDisplayer->getHeight() + 50);
}

void PlayerNode::getPlaySignal()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    if (!mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->play();
    }
}

void PlayerNode::getPauseSignal()
{
    if (!mVideoDisplayer->getMovieRef()) return;
    if (mVideoDisplayer->getMovieRef()->isPlaying()) {
        mVideoDisplayer->getMovieRef()->stop();
    }
}
