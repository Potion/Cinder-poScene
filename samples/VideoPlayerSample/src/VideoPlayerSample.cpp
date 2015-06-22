#include "VideoPlayerSample.h"

using namespace po::scene;

VideoPlayerSampleRef VideoPlayerSample::create() 
{
    VideoPlayerSampleRef node(new VideoPlayerSample());
    node->setup();
    return node;
}

VideoPlayerSample::VideoPlayerSample()
: mNumMovies(3)
, mIsControllerInPosition(false)
{
}

void VideoPlayerSample::setup() 
{
    
    //  create and add the main player
    mPlayer = PlayerController::create();
    mPlayer->setAlignment(po::scene::Alignment::TOP_CENTER);
    mPlayer->setPosition(ci::app::getWindowWidth() / 2, -mPlayer->getHeight() / 2); // centered, just above screen
    mPlayer->setAlpha(0.f);
    addChild(mPlayer);

    //  set location for top/center of primary display
    mPrimaryDisplayerPosition = ci::Vec2f(ci::app::getWindowWidth() / 2, 50);
    
    try {
        //  load the three videos
        ci::fs::path moviePath[3];
        moviePath[0] = ci::app::getAssetPath("Placeholder_Video-RH3i7qONrT4.mp4");
        moviePath[1] = ci::app::getAssetPath("Placeholder_Video-ScMzIvxBSi4.mp4");
        moviePath[2] = ci::app::getAssetPath("Video-lBP2Ij86Ua4.mp4");
        
        ci::qtime::MovieGlRef qtMovie[3];
 
        //  load the movies, create po::scene movie references, then create MovieThumb objects
        for (int i = 0; i < mNumMovies; i++) {
            qtMovie[i] = ci::qtime::MovieGl::create(moviePath[i]);
            VideoGlRef poMovie = VideoGl::create();
            poMovie->setMovieRef(qtMovie[i]);
            mMovies[i] = MovieThumb::create(poMovie);
            addChild(mMovies[i]);
        }
        
        setUpMovies();
        
    } catch (...) {
        std::cout << "Videos did not load successfully";
    }
}

void VideoPlayerSample::setUpMovies()
{
    float thumbnailScale = 0.2f;
    float screenInterval = ci::app::getWindowWidth() / (mNumMovies * 2);
    
    for (int i = 0; i < mNumMovies; i++) {

        mMovies[i]->setAlignment(po::scene::Alignment::CENTER_CENTER);

        //  set scale and position of movie when it's the main one being displayed
        
        //  set scale of movie so it plays at width of 640 px (same as mPlayer width)
        float actualWidth = mMovies[i]->getUnderlyingMovie()->getWidth();
        float scale = mPlayer->getWidth() / actualWidth;
        mMovies[i]->setPlayerScale(ci::Vec2f(scale, scale));

        //  set position based on its height
        float yOffsetForPlayer = (mMovies[i]->getUnderlyingMovie()->getHeight() * scale) * 0.5;
        ci::Vec2f playerPosition(mPrimaryDisplayerPosition.x, mPrimaryDisplayerPosition.y + yOffsetForPlayer);
        mMovies[i]->setPlayerPos(playerPosition);
        
        //  calculate the thumbnail scale, then set appropriate variable in mMovie object
        mMovies[i]->setThumbnailScale(mMovies[i]->getPlayerScale() * thumbnailScale);
        mMovies[i]->setScale(mMovies[i]->getThumbnailScale());
        
        //  calculate the thumbnail position, then set appropriate variable in mMovie object
        float xPos = ((i * 2) + 1) * screenInterval;
        mMovies[i]->setThumbnailPos(ci::Vec2f(xPos, ci::app::getWindowHeight() * 0.8));
        mMovies[i]->setPosition(mMovies[i]->getThumbnailPos());
        
        //  add listeners
        mMovies[i]->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&VideoPlayerSample::onThumbnailClick, this, std::placeholders::_1));
        mMovies[i]->getSignalAnimationComplete().connect(std::bind(&VideoPlayerSample::onAnimationComplete, this, std::placeholders::_1));
    }
}

void VideoPlayerSample::onThumbnailClick(MouseEvent &event)
{
    NodeRef node = event.getSource();
    MovieThumbRef thumbnail = std::static_pointer_cast<MovieThumb>(node);
    
    for (int i = 0; i < mNumMovies; i++) {

        if (mMovies[i] == thumbnail) {

            //  begin animation to primary displayer position, adjusted for center alignment
            mMovies[i]->animateToPlayer();
            animateControllerToPos(mMovies[i]);

            //  move primary movie to top position
            moveChildToFront(mMovies[i]);
        } else if (!mMovies[i]->getIsHome()) {

            //  move other movies back if they're not in their home positions
            mMovies[i]->animateOutOfPlayerPosition();
        }
    }
}

void VideoPlayerSample::onAnimationComplete(MovieThumbRef thumbnail)
{
    mPlayer->setPrimaryMovie(thumbnail->getUnderlyingMovie());
}

void VideoPlayerSample::animateControllerToPos(MovieThumbRef movie)
{
    //  animate player controller to 50 px below the movie
    
    float x = mPlayer->getPosition().x;
    //  find the height of the new movie when fully expanded
    float movieHeight = movie->getUnderlyingMovie()->getHeight() * movie->getPlayerScale().y;
    //  push the controller to 50 px below that
    float y = mPrimaryDisplayerPosition.y + movieHeight + 50 ;
    ci::Vec2f newPos(x, y);
    if (!mIsControllerInPosition) {
        ci::app::timeline().apply(&mPlayer->getPositionAnim(), newPos, 2.f, ci::EaseOutBounce());
    } else {
        ci::app::timeline().apply(&mPlayer->getPositionAnim(), newPos, 2.f);
    }
    
    //  fade player in if it's transparent
    if (!mIsControllerInPosition) {
        ci::app::timeline().apply(&mPlayer->getAlphaAnim(), 1.f, 2.f);
        mIsControllerInPosition = true;
    }
}