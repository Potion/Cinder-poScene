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
{
}

void VideoPlayerSample::setup() 
{
    
    //  create and add the main player
    mPlayer = PlayerController::create();
    float xOffset = (ci::app::getWindowWidth() - mPlayer->getWidth()) / 2;
    mPlayer->setPosition(xOffset, 50);
    mPlayer->setAlpha(0.f);
    addChild(mPlayer);

    //  set location for primary display at same position
    mPrimaryDisplayerPosition = ci::Vec2f(xOffset, 50);
    
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
            
            //  set scale of movie so it plays at width of 640 px (same as controller width)
            float actualWidth = qtMovie[i]->getWidth();
            float scale = mPlayer->getWidth() / actualWidth;
            mMovies[i]->setPlayerScale(ci::Vec2f(scale, scale));
            addChild(mMovies[i]);
        }
        
        setUpMovieThumbnails();
        
    } catch (...) {
        std::cout << "Videos did not load successfully";
    }
}

void VideoPlayerSample::setUpMovieThumbnails()
{
    float thumbnailScale = 0.2f;
    float screenInterval = ci::app::getWindowWidth() / mNumMovies;
    float thumbWidth = mMovies[0]->getWidth() * thumbnailScale; // all movies are same size
    float xOffset = (screenInterval - thumbWidth) / 2;
    
    for (int i = 0; i < mNumMovies; i++) {
        mMovies[i]->setScale(mMovies[i]->getPlayerScale() * thumbnailScale);
        float xPos = (screenInterval * i) + xOffset;
        mMovies[i]->setPosition(ci::Vec2f(xPos, ci::app::getWindowHeight() * 0.8));
        mMovies[i]->setHomePos(ci::Vec2f(xPos, ci::app::getWindowHeight() * 0.8));
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
            mMovies[i]->animateToPlayer(mPrimaryDisplayerPosition);
            animateControllerToPos(mMovies[i]);
        } else if (!mMovies[i]->getIsHome()) {
            mMovies[i]->animateToHomePosition();
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
    ci::app::timeline().apply(&mPlayer->getPositionAnim(), newPos, 2.f);
    
    //  fade player in if it's transparent
    if (mPlayer->getAlpha() < 1.f) {
        ci::app::timeline().apply(&mPlayer->getAlphaAnim(), 1.f, 2.f);
    }
}