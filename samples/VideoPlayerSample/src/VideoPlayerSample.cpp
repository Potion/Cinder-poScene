#include "VideoPlayerSample.h"

using namespace po::scene;

VideoPlayerSampleRef VideoPlayerSample::create() 
{
    VideoPlayerSampleRef node(new VideoPlayerSample());
    node->setup();
    return node;
}

VideoPlayerSample::VideoPlayerSample()
{
}

void VideoPlayerSample::setup() 
{
    //  create the nodes
    mMovie1 = VideoGl::create();
    mMovie2 = VideoGl::create();
    mMovie3 = VideoGl::create();
    
    //  load the three videos
    ci::fs::path moviePath1 = ci::app::getAssetPath("Placeholder_Video-RH3i7qONrT4.mp4");
    ci::fs::path moviePath2 = ci::app::getAssetPath("Placeholder_Video-ScMzIvxBSi4.mp4");
    ci::fs::path moviePath3 = ci::app::getAssetPath("Video-lBP2Ij86Ua4.mp4");
    
    try {
        ci::qtime::MovieGlRef movie1 = ci::qtime::MovieGl::create(moviePath1);
        ci::qtime::MovieGlRef movie2 = ci::qtime::MovieGl::create(moviePath2);
        ci::qtime::MovieGlRef movie3 = ci::qtime::MovieGl::create(moviePath3);
        
        mMovie1->setMovieRef(movie1);
        mMovie2->setMovieRef(movie2);
        mMovie3->setMovieRef(movie3);
        
    } catch (...) {
        std::cout << "Video refused to load";
    }
    
}

void VideoPlayerSample::setUpMovieThumbnails()
{
    
}