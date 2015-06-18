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

void MovieThumb::animateToPlayer(ci::Vec2f pos)
{
    mIsAtHome = false;
    ci::app::timeline().apply(&getPositionAnim(), pos, 2.f);
    ci::app::timeline().apply(&getScaleAnim(), mPlayerScale, 2.f).finishFn(std::bind(&MovieThumb::finishAnimationToPlayer, this));
}

void MovieThumb::animateToHomePosition()
{
    ci::app::timeline().apply(&getPositionAnim(), mHomePos, 2.f);
    ci::app::timeline().apply(&getScaleAnim(), mPlayerScale * 0.2f, 2.f).finishFn(std::bind(&MovieThumb::finishAnimationToHome, this));
}

void MovieThumb::finishAnimationToPlayer()
{
    //  when arrive at player, send signal with a reference to itself
    mAnimationCompleteSignal(std::static_pointer_cast<MovieThumb>(shared_from_this()));
}

void MovieThumb::finishAnimationToHome()
{
    mIsAtHome = true;
}