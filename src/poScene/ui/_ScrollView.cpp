#include "poScene/ui/ScrollView.h"

using namespace std::placeholders;

namespace po
{
	namespace scene
	{
		namespace ui
		{
			ScrollViewRef ScrollView::create()
			{
				ScrollViewRef ref( new ScrollView() );
				ref->setup();
				return ref;
			}

			ScrollView::ScrollView()
				: po::scene::View( "UI Scroll View" )
				, mTouchId( -1 )
				, mMaxAccel( 0.1 )
				, mDecel( 0.25 )
				, mHorizontalScrollingEnabled( false )
				, mVerticalScrollingEnabled( false )
			{
			}

			void ScrollView::setup()
			{
				// Create content view
				mContentView = po::scene::View::create( "UI Scroll View Content View" );
				addSubview( mContentView );

				// Subscribe to events
				mConnections += mContentView->getSignal( po::scene::TouchEvent::BEGAN_INSIDE ).connect( std::bind( &ScrollView::touchBeganInside, this, ::_1 ) );
				mConnections += mContentView->getSignal( po::scene::TouchEvent::MOVED ).connect( std::bind( &ScrollView::touchMoved, this, ::_1 ) );
				mConnections += mContentView->getSignal( po::scene::TouchEvent::ENDED ).connect( std::bind( &ScrollView::touchEnded, this, ::_1 ) );
			}

			void ScrollView::update()
			{
				mHorizontalScrollingEnabled = mContentView->getWidth() > getWidth();
				mVerticalScrollingEnabled = mContentView->getHeight() > getHeight();

				if( !mIsScrolling ) {
					ci::vec2 newPos = ( mScrollTargetPos - mContentView->getPosition() ) * mDecel;
					mContentView->setPosition( mContentView->getPosition() + newPos );
				}
			}

			void ScrollView::setContentOffset( ci::vec2 offset, bool bAnimate )
			{
				mScrollTargetPos = offset;
				if( !bAnimate)
					mContentView->setPosition( offset );
			}

			ci::vec2 ScrollView::getSnapPos( ci::vec2 pos )
			{
				ci::vec2 maxPos = ci::vec2( 0.f );
				ci::vec2 minPos = getSize() - mContentView->getSize();

				pos.x = ci::clamp<float>( pos.x, minPos.x, maxPos.x );
				pos.y = ci::clamp<float>( pos.y, minPos.y, maxPos.y );
				return pos;
			}

			// Event handlers
			void ScrollView::touchBeganInside( po::scene::TouchEvent& event )
			{
				if( mTouchId == -1 ) {
					mTouchId = event.getId();
					mIsScrolling = true;
					mStartTouchPos = event.getWindowPos();
					mCurTouchPos = event.getWindowPos();
					mPrevTouchPos = event.getWindowPos();
				}
			}

			void ScrollView::touchMoved( po::scene::TouchEvent& event )
			{

				if( event.getId() == mTouchId ) {
					ci::vec2 diff = event.getWindowPos() - mPrevTouchPos;
					mPrevTouchPos = mCurTouchPos;
					mCurTouchPos = event.getWindowPos();

					ci::vec2 newPos = mContentView->getPosition();

					if( mHorizontalScrollingEnabled ) {
						newPos.x += diff.x;
					}

					if( mVerticalScrollingEnabled ) {
						newPos.y += diff.y;
					}

					mContentView->setPosition( newPos );
				}
			}

			void ScrollView::touchEnded( po::scene::TouchEvent& event )
			{
				if( event.getId() == mTouchId ) {
					ci::vec2 accel = mCurTouchPos - mPrevTouchPos;
					// Get normalized based on maximum distance you can scroll
					// Maybe should be based on the size of this view vs window?
					accel = accel / ci::vec2( ci::app::getWindowSize() );

					// Get the direction then set accel to an abs value for mapping
					ci::vec2 direction( accel.x < 0 ? -1 : 1, accel.y < 0 ? -1 : 1 );
					accel = ci::vec2( fabs( accel.x ), fabs( accel.y ) );

					// Calculate how far to throw
					ci::vec2 maxThrowDistance = getSize() * ci::vec2( 2.0 );
					ci::vec2 throwDistance = ci::lmap<ci::vec2>( accel, ci::vec2( 0.0f ), ci::vec2( mMaxAccel ), ci::vec2( 0.0f ), maxThrowDistance );

					// Apply direction to throw distance
					throwDistance *= direction;

					// Set our target pos, considering snapping so we don't throw out of the view
					ci::vec2 targetPos = mContentView->getPosition() + throwDistance;
					mScrollTargetPos = getSnapPos( targetPos );

					// Cleanup
					mIsScrolling = false;
					mTouchId = -1;
					
					
					if( !mDelegate.expired() ) {
						ScrollViewRef self = std::dynamic_pointer_cast<ScrollView>( shared_from_this() );
						mDelegate.lock()->didFinishScrolling( self );
					}
				}
			}
		}
	}
}
