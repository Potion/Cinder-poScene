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
				, mEventId( -1 )
				, mId( 0 )
				, mType( Type::NORMAL )
				, mPropagationEnabled( false )
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
				mConnections += getSignal( po::scene::MouseEvent::DOWN_INSIDE ).connect( std::bind( &Button::mouseDownInside, this, ::_1 ) );
				mConnections += getSignal( po::scene::MouseEvent::DRAG ).connect( std::bind( &Button::mouseDrag, this, ::_1 ) );
				mConnections += getSignal( po::scene::MouseEvent::UP ).connect( std::bind( &Button::mouseUp, this, ::_1 ) );

				mConnections += getSignal( po::scene::TouchEvent::BEGAN_INSIDE ).connect( std::bind( &Button::touchBeganInside, this, ::_1 ) );
				mConnections += getSignal( po::scene::TouchEvent::MOVED ).connect( std::bind( &Button::touchMoved, this, ::_1 ) );
				mConnections += getSignal( po::scene::TouchEvent::ENDED ).connect( std::bind( &Button::touchEnded, this, ::_1 ) );
			}

			void Button::layoutSubviews()
			{
			}

			void Button::setState( State state )
			{
				// Set cur state
				mState = state;

				// Update text + images

				// Background image
				{
					// Tint, offset, scale
					setTintAndOffsetForState( mBackgroundImageView, mBackgroundImageTints, mBackgroundImageOffsets, state );
					setScaleForState( mBackgroundImageView, mBackgroundImageScales, state );

					// Image
					ci::gl::TextureRef backgroundTexture;
					getItemForState<ci::gl::TextureRef>( backgroundTexture, mBackgroundImages, state );
					mBackgroundImageView->setTexture( backgroundTexture );
					mBackgroundImageView->setVisible( backgroundTexture != nullptr ? true : false );
				}

				// Image
				{
					// Tint, offset, scale
					setTintAndOffsetForState( mImageView, mImageTints, mImageOffsets, state );
					setScaleForState( mImageView, mImageScales, state );

					// Image
					ci::gl::TextureRef imageTexture = nullptr;
					getItemForState<ci::gl::TextureRef>( imageTexture, mImages, state );

					mImageView->setTexture( imageTexture );
					mImageView->setVisible( imageTexture != nullptr ? true : false );
				}

				// Title
				{
					// Tint + Offset
					setTintAndOffsetForState( mTitleTextView, mTitleTints, mTitleOffsets, state );

					// Text
					std::string title = "";
					getItemForState<std::string>( title, mTitles, state );
					mTitleText.setText( title );

					ci::Font font = font.getDefault();
					getItemForState<ci::Font>( font, mTitleFonts, state );
					mTitleText.setFont( font );

					ci::vec2 size = mTitleText.getSize();
					getItemForState<ci::vec2>( size, mTitleSizes, state );
					mTitleText.setSize( size );

					mTitleTextView->setCiTextBox( mTitleText );
					mTitleTextView->setVisible( title != "" ? true : false );
				}
			}

			void Button::setTintAndOffsetForState( ViewRef view, std::map<State, ci::Color> tints, std::map<State, ci::vec2> offsets, State state )
			{
				ci::Color color( 1.f, 1.f, 1.f );
				getItemForState<ci::Color>( color, tints, state );
				view->setFillColor( color );

				ci::vec2 offset( 0.f );
				getItemForState<ci::vec2>( offset, offsets, state );
				view->setPosition( offset );
			}

			void Button::setScaleForState( ViewRef view, std::map<State, ci::vec2> scales, State state )
			{
				ci::vec2 scale( 1.f );
				getItemForState<ci::vec2>( scale, scales, state );
				view->setScale( scale );
			}


			// Image getters
			ci::gl::TextureRef Button::getBackgroundImage( State forState )
			{
				if( mBackgroundImages.count( forState ) == 0 ) {
					return nullptr;
				}

				return mBackgroundImages[forState];
			}

			ci::gl::TextureRef Button::getImage( State forState )
			{
				if( mImages.count( forState ) == 0 ) {
					return nullptr;
				}

				return mImages[forState];
			}

			// Image, text, color and offset setters
			void Button::setBackgroundImage( ci::gl::TextureRef image, State state ) { setItemForState<ci::gl::TextureRef>( image, mBackgroundImages, state ); }
			void Button::setBackgroundImageOffset( ci::vec2 offset, State state ) { setItemForState<ci::vec2>( offset, mBackgroundImageOffsets, state ); }
			void Button::setBackgroundImageScale( ci::vec2 scale, State state ) { setItemForState<ci::vec2>( scale, mBackgroundImageScales, state ); }
			void Button::setBackgroundImageTint( ci::Color color, State state ) { setItemForState<ci::Color>( color, mBackgroundImageTints, state ); }

			void Button::setImage( ci::gl::TextureRef image, State state ) { setItemForState<ci::gl::TextureRef>( image, mImages, state ); }
			void Button::setImageOffset( ci::vec2 offset, State state ) { setItemForState<ci::vec2>( offset, mImageOffsets, state ); }
			void Button::setImageScale( ci::vec2 scale, State state ) { setItemForState<ci::vec2>( scale, mImageScales, state ); }
			void Button::setImageTint( ci::Color color, State state ) { setItemForState<ci::Color>( color, mImageTints, state ); }

			void Button::setTitle( std::string title, State state ) { setItemForState < std::string >( title, mTitles, state ); }
			void Button::setTitleFont( ci::Font font, State state ) { setItemForState < ci::Font >( font, mTitleFonts, state ); }

			void Button::setTitleOffset( ci::vec2 offset, State state ) { setItemForState<ci::vec2>( offset, mTitleOffsets, state ); }
			void Button::setTitleTint( ci::Color color, State state ) { setItemForState<ci::Color>( color, mTitleTints, state ); }

			void Button::setTint( ci::Color color, State state )
			{
				setBackgroundImageTint( color, state );
				setImageTint( color, state );
				setTitleTint( color, state );
			}

			void Button::setTitleSize( ci::vec2 size, State forState )
			{
				setItemForState<ci::vec2>( size, mTitleSizes, forState );
			}

			void Button::setTitleWidth( float width, State forState )
			{
				setTitleSize( mTitleText.getSize() + ci::ivec2( width, 0 ), forState );
			}

			// Event listeners
			void Button::eventBeganInside( int id, ci::vec2 windowPos )
			{
				if( mEventId == -1 ) {
					mEventId = id;
					mEventStartPos = windowToLocal( windowPos );
					mEventStartState = mState;
					setState( State::HIGHLIGHTED );
				}
			}

			void Button::eventMoved( int id, ci::vec2 windowPos )
			{
				if( mEventId == id ) {
					if( pointInside( windowPos ) ) {
						setState( State::HIGHLIGHTED );
					}
					else {
						setState( State::NORMAL );
					}
				}
			}

			void Button::eventEnded( int id, ci::vec2 windowPos )
			{
				if( mEventId == id ) {
					if( pointInside( windowPos ) ) {
						// Normal just send pressed
						if( mType == Type::NORMAL ) {
							setState( State::NORMAL );
							mSignalPressed.emit( std::dynamic_pointer_cast<Button>( shared_from_this() ) );
						}

						// Toggle switch State
						else {
							setState( mEventStartState == State::NORMAL ? State::SELECTED : State::NORMAL );
							mSignalToggled.emit( std::dynamic_pointer_cast<Button>( shared_from_this() ) );
						}
					}

					mEventId = -1;
				}
			}

			void Button::mouseDownInside( po::scene::MouseEvent& event )
			{
				event.setPropagationEnabled( mPropagationEnabled );
				eventBeganInside( 0, event.getWindowPos() );
			}

			void Button::mouseDrag( po::scene::MouseEvent& event )
			{
				event.setPropagationEnabled( mPropagationEnabled );
				eventMoved( 0, event.getWindowPos() );
			}

			void Button::mouseUp( po::scene::MouseEvent& event )
			{
				event.setPropagationEnabled( mPropagationEnabled );
				eventEnded( 0, event.getWindowPos() );
			}

			void Button::touchBeganInside( po::scene::TouchEvent& event )
			{
				event.setPropagationEnabled( mPropagationEnabled );
				eventBeganInside( event.getId(), event.getWindowPos() );
			}

			void Button::touchMoved( po::scene::TouchEvent& event )
			{
				event.setPropagationEnabled( mPropagationEnabled );
				eventMoved( event.getId(), event.getWindowPos() );
			}

			void Button::touchEnded( po::scene::TouchEvent& event )
			{
				event.setPropagationEnabled( mPropagationEnabled );
				eventEnded( event.getId(), event.getWindowPos() );
			}
		}
	}
}
