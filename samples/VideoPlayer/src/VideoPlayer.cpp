#include "poShape.h"

#include "VideoPlayer.h"
#include "poVideoPlayer.h"

VideoPlayerRef VideoPlayer::create() {
    VideoPlayerRef node(new VideoPlayer());
    node->setup();
    return node;
}


void VideoPlayer::setup() {
    po::VideoPlayerRef player = po::VideoPlayer::create();
    player->load(ci::app::loadAsset("big_buck_bunny.mp4"));
    player->play();
    addChild(player);
    player->setAlignment(po::Alignment::CENTER_CENTER);
    player->setRotation(90);
}
