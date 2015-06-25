#include "MovieThumb.h"

using namespace po::scene;

MovieThumbRef MovieThumb::create(po::scene::VideoGlRef movie)
{
    MovieThumbRef ref (new MovieThumb());
    ref->setup(movie);
    return ref;
}

MovieThumb::MovieThumb()
: mIsAtHome(true)
{}

void MovieThumb::setup(po::scene::VideoGlRef movie)
{
    mMovie = movie;
    addChild(mMovie);
}

void MovieThumb::animateToPlayer()
{
    mIsAtHome = false;
    ci::app::timeline().apply(&getPositionAnim(), mPlayerPos, 1.5f, ci::EaseOutBack());
    ci::app::timeline().apply(&getScaleAnim(), mPlayerScale, 1.5f, ci::EaseInExpo()).finishFn(std::bind(&MovieThumb::finishAnimationToPlayer, this));
}

void MovieThumb::animateOutOfPlayerPosition()
{
    float yPos = getUnderlyingMovie()->getHeight() / 2;
    yPos *= mThumbnailScale.y * -1;
    ci::Vec2f aboveScreen( getPosition().x, yPos );
    ci::app::timeline().apply(&getPositionAnim(), aboveScreen, 1.5f, ci::EaseInExpo()).finishFn(std::bind(&MovieThumb::slideUpToHomePosition, this));
    ci::app::timeline().apply(&getScaleAnim(), mThumbnailScale, 1.5f, ci::EaseOutBack(2.f));
}

void MovieThumb::finishAnimationToPlayer()
{
    //  when arrive at player, send signal with a reference to itself
    mAnimationCompleteSignal(std::static_pointer_cast<MovieThumb>(shared_from_this()));
}

void MovieThumb::slideUpToHomePosition()
{
    float yPos = getUnderlyingMovie()->getHeight() / 2;
    yPos *= (mThumbnailScale.y) ;
    yPos += ci::app::getWindowHeight();
    ci::Vec2f belowScreen(getPosition().x, yPos);
    setPosition(belowScreen);

    setScale(0.0f, 0.0f);
    ci::app::timeline().apply(&getScaleAnim(), mThumbnailScale, 0.5f, ci::EaseOutBack());
    ci::app::timeline().apply(&getPositionAnim(), mThumbnailPos, 0.5f, ci::EaseOutBack()).finishFn(std::bind(&MovieThumb::finishAnimationToHome, this));
}

void MovieThumb::finishAnimationToHome()
{
    mIsAtHome = true;
}