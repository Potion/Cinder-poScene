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
    mPlayer = PlayerNode::create();
    addChild(mPlayer);
    
    //  load the movies and create the thumbnails
    for (int i = 0; i < mNumMovies; i++) {
        mMovies[i] = VideoGl::create();
    }
    
    //  load the three videos
    ci::fs::path moviePath1 = ci::app::getAssetPath("Placeholder_Video-RH3i7qONrT4.mp4");
    ci::fs::path moviePath2 = ci::app::getAssetPath("Placeholder_Video-ScMzIvxBSi4.mp4");
    ci::fs::path moviePath3 = ci::app::getAssetPath("Video-lBP2Ij86Ua4.mp4");
    
    try {
        ci::qtime::MovieGlRef movie0 = ci::qtime::MovieGl::create(moviePath1);
        ci::qtime::MovieGlRef movie1 = ci::qtime::MovieGl::create(moviePath2);
        ci::qtime::MovieGlRef movie2 = ci::qtime::MovieGl::create(moviePath3);
        
        mMovies[0]->setMovieRef(movie0);
        mMovies[1]->setMovieRef(movie1);
        mMovies[2]->setMovieRef(movie2);
        
        setUpMovieThumbnails();
        
    } catch (...) {
        std::cout << "Video did not load successfully";
    }
}

void VideoPlayerSample::setUpMovieThumbnails()
{
    ci::Vec2f thumbnailScale = ci::Vec2f(0.2f, 0.2f);
    float screenInterval = ci::app::getWindowWidth() / mNumMovies;
    float thumbWidth = mMovies[0]->getWidth() * thumbnailScale.x; // all movies are same size
    float xOffset = (screenInterval - thumbWidth) / 2;
    
    for (int i = 0; i < mNumMovies; i++) {
        mMovies[i]->setScale(thumbnailScale);
        float xPos = (screenInterval * i) + xOffset;
        mMovies[i]->setPosition(ci::Vec2f(xPos, ci::app::getWindowHeight() * 0.8));
        addChild(mMovies[i]);
        mMovies[i]->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&VideoPlayerSample::onThumbnailClick, this, std::placeholders::_1));
    }
}

void VideoPlayerSample::onThumbnailClick(MouseEvent event)
{
    NodeRef node = event.getSource();
    VideoGlRef video = std::static_pointer_cast<VideoGl>(node);
    mPlayer->setPrimaryMovie(video);
    std::cout << "VideoPlayerSample::onThumbnailClick: mPlayer width: " << mPlayer->getWidth() << std::endl;
    
    float xOffset = (ci::app::getWindowWidth() - mPlayer->getWidth()) / 2;
    
    mPlayer->setPosition(xOffset, 50);
    
}