#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"

class MovieThumb;
typedef std::shared_ptr<MovieThumb> MovieThumbRef;

class MovieThumb
: public po::scene::NodeContainer
{
public:
    
    typedef boost::signals2::signal<void(MovieThumbRef)> MovieThumbSignal;
    
    static MovieThumbRef create(po::scene::VideoGlRef movie);
    void setup(po::scene::VideoGlRef movie);
    
    void animateToPlayer();
    void animateOutOfPlayerPosition();
    void slideUpToHomePosition();
    

    void setPlayerPos(ci::Vec2f pos) { mPlayerPos = pos; };
    ci::Vec2f &getPlayerPos() { return mPlayerPos; };
    
    void setThumbnailPos(ci::Vec2f pos) { mThumbnailPos = pos; };
    ci::Vec2f &getThumbnailPos() { return mThumbnailPos; };

    void setPlayerScale (ci::Vec2f scale) { mPlayerScale = scale; };
    ci::Vec2f &getPlayerScale() { return mPlayerScale; };
    
    void setThumbnailScale (ci::Vec2f scale) { mThumbnailScale = scale; };
    ci::Vec2f &getThumbnailScale() { return mThumbnailScale; };
    
    bool getIsHome() { return mIsAtHome; };
    
    po::scene::VideoGlRef getUnderlyingMovie() { return mMovie; };
    
    
    
    MovieThumbSignal &getSignalAnimationComplete() { return mAnimationCompleteSignal; };
    
protected:
    MovieThumb();
    
private:
    ci::Vec2f               mPlayerPos;
    ci::Vec2f               mThumbnailPos;
    ci::Vec2f               mPlayerScale;
    ci::Vec2f               mThumbnailScale;
    
    po::scene::VideoGlRef   mMovie;
    bool                    mIsAtHome;

    void                    finishAnimationToPlayer();
    void                    finishAnimationToHome();

    MovieThumbSignal        mAnimationCompleteSignal;
};