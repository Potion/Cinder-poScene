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
				, mEventId( -1 )
				, mMaxAccel( 0.1 )
				, mDecel( 0.25 )
				, mHorizontalScrollingEnabled( false )
				, mVerticalScrollingEnabled( false )
				, mTouchEventsEnabled( true )
				, mMouseEventsEnabled( true )
				, mInitialized( false )
				, mIsScrolling( false )
			{
			}

			void ScrollView::setup()
			{
				// Create content view
				mContentView = po::scene::View::create( "UI Scroll View Content View" );
				addSubview( mContentView );

				// Subscribe to events
				if( mMouseEventsEnabled ) { enableMouseEvents(); }

				if( mTouchEventsEnabled ) { enableTouchEvents(); }

				mInitialized = true;
			}

			View& ScrollView::addSubview( ViewRef view, bool localize )
			{
				CI_ASSERT_MSG( !mInitialized, "Can not add subview directly to ScrollView, get the content view and add to it." );
				return po::scene::View::addSubview( view );
			}

			void ScrollView::enableMouseEvents()
			{
				mMouseConnections += getSignal( po::scene::MouseEvent::DOWN_INSIDE ).connect( std::bind( &ScrollView::mouseDownInside, this, ::_1 ) );
				mMouseConnections += getSignal( po::scene::MouseEvent::DRAG ).connect( std::bind( &ScrollView::mouseDrag, this, ::_1 ) );
				mMouseConnections += getSignal( po::scene::MouseEvent::UP ).connect( std::bind( &ScrollView::mouseUp, this, ::_1 ) );

				mMouseEventsEnabled = true;
			}

			void ScrollView::disableMouseEvents()
			{
				mMouseConnections.clear();
				mMouseEventsEnabled = false;
			}

			void ScrollView::enableTouchEvents()
			{
				mTouchConnections += getSignal( po::scene::TouchEvent::BEGAN_INSIDE ).connect( std::bind( &ScrollView::touchBeganInside, this, ::_1 ) );
				mTouchConnections += getSignal( po::scene::TouchEvent::MOVED ).connect( std::bind( &ScrollView::touchMoved, this, ::_1 ) );
				mTouchConnections += getSignal( po::scene::TouchEvent::ENDED ).connect( std::bind( &ScrollView::touchEnded, this, ::_1 ) );

				mTouchEventsEnabled = true;
			}

			void ScrollView::disableTouchEvents()
			{
				mTouchConnections.clear();

				mTouchEventsEnabled = false;
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

				if( !bAnimate ) {
					mContentView->setPosition( offset );
				}
			}

			ci::vec2 ScrollView::getSnapPos( ci::vec2 pos )
			{
				ci::vec2 maxPos = ci::vec2( 0.f );
				ci::vec2 minPos = getSize() - mContentView->getSize();

				//ci::app::console() << "----------------------------------------" <<  std::endl;
				//ci::app::console() << "Pos: " << minPos << std::endl;
				//ci::app::console() << "Scroll View Size: " << getSize() << std::endl;
				//ci::app::console() << "Scroll View Content Size: " << mContentView->getSize() << std::endl;

				pos.x = ci::clamp<float>( pos.x, minPos.x, maxPos.x );
				pos.y = ci::clamp<float>( pos.y, minPos.y, maxPos.y );

				//ci::app::console() << std::endl;
				//ci::app::console() << "Min Pos: " << minPos << std::endl;
				//ci::app::console() << "New Pos " << pos << std::endl;
				//ci::app::console() << std::endl;

				return pos;
			}

			// Event handlers

			void ScrollView::eventBeganInside( int id, ci::vec2 pos )
			{
				if( mEventId == -1 ) {
					mEventId = id;
					mIsScrolling = true;
					mStartEventPos = pos;
					mCurEventPos = pos;
					mPrevEventPos = pos;
				}

				if( !mDelegate.expired() ) {
					ScrollViewRef self = std::dynamic_pointer_cast<ScrollView>( shared_from_this() );
					mDelegate.lock()->didStartScrolling( self );
				}
			}

			void ScrollView::eventMoved( int id, ci::vec2 pos )
			{
				if( id == mEventId ) {
					ci::vec2 diff = pos - mCurEventPos;

					mPrevEventPos = mCurEventPos;
					mCurEventPos = pos;

					ci::vec2 newPos = mContentView->getPosition();

					if( mHorizontalScrollingEnabled ) {
						newPos.x += diff.x;
					}

					if( mVerticalScrollingEnabled ) {
						newPos.y += diff.y;
					}

					mContentView->setPosition( newPos );



					if( !mDelegate.expired() ) {
						ScrollViewRef self = std::dynamic_pointer_cast<ScrollView>( shared_from_this() );
						mDelegate.lock()->didScroll( self );
					}

					//ci::app::console() << "Drag Pos: " << pos << std::endl;
				}
			}

			void ScrollView::eventEnded( int id, ci::vec2 pos )
			{
				if( id == mEventId ) {
					ci::vec2 accel = pos - mPrevEventPos;

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

					/*
					ci::app::console() << "----------------------------------------" << std::endl;
					ci::app::console() << "Pos: " << pos << std::endl;
					ci::app::console() << "Prev Pos: " << mPrevEventPos << std::endl;
					ci::app::console() << "Direction: " << direction << std::endl;
					ci::app::console() << "Throw Distance: " << throwDistance << std::endl;
					ci::app::console() << "Scroll View Content Size: " << mContentView->getSize() << std::endl;
					*/

					// Cleanup
					mIsScrolling = false;
					mEventId = -1;

					if( !mDelegate.expired() ) {
						ScrollViewRef self = std::dynamic_pointer_cast<ScrollView>( shared_from_this() );
						mDelegate.lock()->didFinishScrolling( self );
					}
				}
			}


			void ScrollView::mouseDownInside( po::scene::MouseEvent& event )
			{
				eventBeganInside( 0, event.getLocalPos() );
			}

			void ScrollView::mouseDrag( po::scene::MouseEvent& event )
			{

				eventMoved( 0, event.getLocalPos() );
			}

			void ScrollView::mouseUp( po::scene::MouseEvent& event )
			{
				eventEnded( 0, event.getLocalPos() );
			}


			void ScrollView::touchBeganInside( po::scene::TouchEvent& event )
			{
				eventBeganInside( event.getId(), event.getLocalPos() );
			}

			void ScrollView::touchMoved( po::scene::TouchEvent& event )
			{
				eventMoved( event.getId(), event.getLocalPos() );
			}

			void ScrollView::touchEnded( po::scene::TouchEvent& event )
			{
				eventEnded( event.getId(), event.getLocalPos() );
			}
		}
	}
}
