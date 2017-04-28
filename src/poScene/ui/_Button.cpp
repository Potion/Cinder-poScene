#include "poScene/ui/Button.h"

using namespace std::placeholders;

namespace po
{
	namespace scene
	{
		namespace ui
		{
			ButtonRef Button::create()
			{
				ButtonRef ref( new Button() );
				ref->setup();
				return ref;
			}

			Button::Button()
				: mCurState( State::NORMAL )
				, mPressId( -1 )
			{
				setSize( ci::vec2( 50, 50 ) );


			}

			void Button::setup()
			{
				mTitleTextView = po::scene::TextView::create();
				mBackgroundImageView = po::scene::ImageView::create();
				mImageView = po::scene::ImageView::create();

				addSubview( mBackgroundImageView );
				addSubview( mImageView );
				addSubview( mTitleTextView );

				// Add event listeners
				mConnections += getSignal( po::scene::TouchEvent::BEGAN_INSIDE ).connect( std::bind( &Button::touchBeganInside, this, ::_1 ) );
				mConnections += getSignal( po::scene::TouchEvent::MOVED ).connect( std::bind( &Button::touchMoved, this, ::_1 ) );
				mConnections += getSignal( po::scene::TouchEvent::ENDED ).connect( std::bind( &Button::touchEnded, this, ::_1 ) );
			}

			void Button::layoutSubviews()
			{
			}

			void Button::setState( State state )
			{
				// Update text + images
				if( mTintColors.count( state ) != 0 ) {
					mBackgroundImageView->setFillColor( mTintColors[state] );
					mImageView->setFillColor( mTintColors[state] );
					mTitleTextView->setFillColor( mTintColors[state] );
				}

				if( mBackgroundImages.count( state ) != 0 ) {
					mBackgroundImageView->setTexture( mBackgroundImages[state] );
				}

				if( mImages.count( state ) != 0 ) {
					mImageView->setTexture( mImages[state] );
				}

				if( mTitles.count( state ) != 0 ) {
					mTitleText.setText( mTitles[state] );
					mTitleTextView->setCiTextBox( mTitleText );
				}

				// Set cur state
				mCurState = state;
			}


			void Button::setTint( ci::Color color, State forState )
			{
				mTintColors[forState] = color;
				setState( mCurState );
			}


			void Button::setBackgroundImage( ci::gl::TextureRef image, State forState )
			{
				mBackgroundImages[forState] = image;
				setState( mCurState );
			}

			void Button::setImage( ci::gl::TextureRef image, State forState )
			{
				mImages[forState] = image;
				setState( mCurState );
			}

			void Button::setTitle( std::string title, State forState )
			{
				mTitles[forState] = title;
				setState( mCurState );
			}



			// Event listeners
			void Button::touchBeganInside( po::scene::TouchEvent& event )
			{
				if( mPressId == -1 ) {
					mPressId = event.getId();
					mPressStartPos = event.getWindowPos();
					setState( State::HIGHLIGHTED );
				}
			}

			void Button::touchMoved( po::scene::TouchEvent& event )
			{
				if( mPressId == event.getId() ) {
					if( pointInside( event.getWindowPos() ) ) {
						setState( State::HIGHLIGHTED );
					}
					else {
						setState( State::NORMAL );
					}
				}
			}

			void Button::touchEnded( po::scene::TouchEvent& event )
			{
				if( mPressId == event.getId() ) {
					if( pointInside( event.getWindowPos() ) ) {
						mSignalPressed.emit( std::dynamic_pointer_cast<Button>( shared_from_this() ) );
					}
				}

				setState( State::NORMAL );
				mPressId = -1;
			}
		}
	}
}