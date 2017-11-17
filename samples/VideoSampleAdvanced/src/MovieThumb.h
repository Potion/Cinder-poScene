#pragma once

#include "cinder/Signals.h"

#include "poScene/View.h"
#include "poScene/VideoView.h"

class MovieThumb;
typedef std::shared_ptr<MovieThumb> MovieThumbRef;

class MovieThumb
: public po::scene::View
{
public:
    
    typedef ci::signals::Signal<void(MovieThumbRef)> MovieThumbSignal;
    
    static MovieThumbRef create(po::scene::VideoViewGlRef movie);
    void setup(po::scene::VideoViewGlRef movie);
    
    void animateToPlayer();
    void animateOutOfPlayerPosition();
    void slideUpToHomePosition();
    

    void setPlayerPos(ci::vec2 pos) { mPlayerPos = pos; };
    ci::vec2 &getPlayerPos() { return mPlayerPos; };
    
    void setThumbnailPos(ci::vec2 pos) { mThumbnailPos = pos; };
    ci::vec2 &getThumbnailPos() { return mThumbnailPos; };

    void setPlayerScale (ci::vec2 scale) { mPlayerScale = scale; };
    ci::vec2 &getPlayerScale() { return mPlayerScale; };
    
    void setThumbnailScale (ci::vec2 scale) { mThumbnailScale = scale; };
    ci::vec2 &getThumbnailScale() { return mThumbnailScale; };
    
    bool getIsHome() { return mIsAtHome; };
    
    po::scene::VideoViewGlRef getUnderlyingMovie() { return mMovie; };
    
    
    
    MovieThumbSignal &getSignalAnimationComplete() { return mAnimationCompleteSignal; };
    
protected:
    MovieThumb();
    
private:
    ci::vec2                mPlayerPos;
    ci::vec2                mThumbnailPos;
    ci::vec2                mPlayerScale;
    ci::vec2                mThumbnailScale;
    
    po::scene::VideoViewGlRef   mMovie;
    bool                    mIsAtHome;

    void                    finishAnimationToPlayer();
    void                    finishAnimationToHome();

    MovieThumbSignal        mAnimationCompleteSignal;
};