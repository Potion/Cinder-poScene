#include "poShape.h"

#include "VideoPlayer.h"
#include "poVideo.h"

VideoPlayerRef VideoPlayer::create() {
    VideoPlayerRef node(new VideoPlayer());
    node->setup();
    return node;
}


void VideoPlayer::setup() {
	po::VideoGlRef player = po::VideoGl::create();
	addChild(player);
	player->setAlignment(po::Alignment::CENTER_CENTER);
	player->setRotation(20);
	
	try {
		ci::fs::path moviePath = ci::app::getAssetPath("trailer_480p.mov");
		ci::app::console() << "path: " << moviePath << std::endl;
		ci::qtime::MovieGlRef movieRef = ci::qtime::MovieGl::create(moviePath);
		player->setMovieRef(movieRef);
		player->getMovieRef()->play();
	} catch(...) {
		ci::app::console() << "Couldn't load movie" << std::endl;
	}
}
