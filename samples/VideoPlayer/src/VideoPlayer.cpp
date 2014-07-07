#include "poShape.h"

#include "VideoPlayer.h"

VideoPlayerRef VideoPlayer::create() {
    VideoPlayerRef node(new VideoPlayer());
    node->setup();
    return node;
}


void VideoPlayer::setup() {

}
