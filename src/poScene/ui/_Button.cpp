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
				: mState( State::NORMAL )
				, mPressId( -1 )
				, mId( 0 )
				, mType( Type::NORMAL )
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


				// Background image
				{
					ci::gl::TextureRef backgroundTexture = nullptr;

					if( mBackgroundImages.count( state ) != 0 ) {
						backgroundTexture = mBackgroundImages[state];
					}

					else if( mBackgroundImages.count( State::NORMAL ) != 0 ) {
						backgroundTexture = mBackgroundImages[State::NORMAL];
					}

					mBackgroundImageView->setTexture( backgroundTexture );
					mBackgroundImageView->setVisible( backgroundTexture != nullptr ? true : false );
				}

				// Image
				{
					ci::gl::TextureRef imageTexture = nullptr;

					if( mImages.count( state ) != 0 ) {
						imageTexture = mImages[state];
					}

					else if( mImages.count( State::NORMAL ) != 0 ) {
						imageTexture = mImages[State::NORMAL];
					}

					mImageView->setTexture( imageTexture );
					mImageView->setVisible( imageTexture != nullptr ? true : false );
				}

				// Title
				{
					std::string title = "";

					if( mTitles.count( state ) != 0 ) {
						title = mTitles[state];
					}


					else if( mTitles.count( State::NORMAL ) != 0 ) {
						title = mTitles[State::NORMAL];
					}

					mTitleText.setText( title );
					mTitleTextView->setCiTextBox( mTitleText );
					mTitleTextView->setVisible( title != "" ? true : false );
				}

				// Set cur state
				mState = state;
			}


			void Button::setTint( ci::Color color, State forState )
			{
				mTintColors[forState] = color;
				setState( mState );
			}


			void Button::setBackgroundImage( ci::gl::TextureRef image, State forState )
			{
				mBackgroundImages[forState] = image;
				setState( mState );
			}

			void Button::setImage( ci::gl::TextureRef image, State forState )
			{
				mImages[forState] = image;
				setState( mState );
			}

			void Button::setTitle( std::string title, State forState )
			{
				mTitles[forState] = title;
				setState( mState );
			}



			// Event listeners
			void Button::touchBeganInside( po::scene::TouchEvent& event )
			{
				if( mPressId == -1 ) {
					mPressId = event.getId();
					mPressStartPos = event.getWindowPos();
					mPressStartState = mState;
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
						// Normal just send pressed
						if( mType == Type::NORMAL ) {
							setState( State::NORMAL );
							mSignalPressed.emit( std::dynamic_pointer_cast<Button>( shared_from_this() ) );
						}

						// Toggle switch State
						else {
							setState( mPressStartState == State::NORMAL ? State::SELECTED : State::NORMAL );
							mSignalToggled.emit( std::dynamic_pointer_cast<Button>( shared_from_this() ) );
						}
					}


					mPressId = -1;
				}
			}
		}
	}
}