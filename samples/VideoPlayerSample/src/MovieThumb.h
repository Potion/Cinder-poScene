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
    
    void animateToPlayer(ci::Vec2f pos);
    void animateToHomePosition();
    
    void setHomePos(ci::Vec2f pos) { mHomePos = pos; };
    ci::Vec2f &getHomePos() { return mHomePos; };
    
    void setPlayerScale (ci::Vec2f scale) { mPlayerScale = scale; };
    ci::Vec2f &getPlayerScale() { return mPlayerScale; };
    
    bool getIsHome() { return mIsAtHome; };
    
    po::scene::VideoGlRef getUnderlyingMovie() { return mMovie; };
    
    
    
    MovieThumbSignal &getSignalAnimationComplete() { return mAnimationCompleteSignal; };
    
protected:
    MovieThumb();
    
private:
    ci::Vec2f               mHomePos;
    ci::Vec2f               mPlayerScale;
    po::scene::VideoGlRef   mMovie;
    bool                    mIsAtHome;

    void                    finishAnimationToPlayer();
    void                    finishAnimationToHome();

    MovieThumbSignal        mAnimationCompleteSignal;
};