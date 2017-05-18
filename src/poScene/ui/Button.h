#pragma once

#include "poScene/View.h"
#include "poScene/ImageView.h"
#include "poScene/TextView.h"

namespace po
{
	namespace scene
	{
		namespace ui
		{

			class Button;
			typedef std::shared_ptr<Button> ButtonRef;


			typedef ci::signals::Signal<void( ButtonRef )> ButtonSignal;

			class Button : public po::scene::View
			{
				public:
					enum class Type {
						NORMAL = 0,
						TOGGLE
					};

					enum class State {
						NORMAL = 0,
						HIGHLIGHTED,
						SELECTED
					};

					static ButtonRef create();

					void layoutSubviews() override;

					ci::TextBox& getTitleTextBox() { return mTitleText; }

					ButtonSignal& getSignalPressed() { return mSignalPressed; }
					ButtonSignal& getSignalToggled() { return mSignalToggled; }

					const Type& getType() { return mType; }
					void setType( Type type ) { mType = type; }

					const State& getState() { return mState; }
					void setState( State state );

					// Sets the tint for all views
					void setTint( ci::Color tintColor, State forState = State::NORMAL );

					void setBackgroundImage( ci::gl::TextureRef image, State forState = State::NORMAL );
					void setBackgroundImageOffset( ci::vec2 offset, State forState = State::NORMAL );
					void setBackgroundImageTint( ci::Color color, State forState = State::NORMAL );

					void setImage( ci::gl::TextureRef image, State forState = State::NORMAL );
					void setImageOffset( ci::vec2 offset, State forState = State::NORMAL );
					void setImageTint( ci::Color color, State forState = State::NORMAL );

					void setTitle( std::string title, State forState = State::NORMAL );
					void setTitleFont( ci::Font font, State forState = State::NORMAL );
					void setTitleOffset( ci::vec2 offset, State forState = State::NORMAL );
					void setTitleTint( ci::Color color, State forState = State::NORMAL );
					void setTitleSize( ci::vec2 size, State forState = State::NORMAL );
					void setTitleWidth( float width, State forState = State::NORMAL );

					ci::vec2 getTitleSize() { return mTitleTextView->getSize(); }

					void setId( int id ) { mId = id; }
					int getId() { return mId; }

				protected:
					Button();
					void setup() override;

					// Type + State tracking
					State mState;
					Type mType;

					// Template functions to update state vars
					template<typename T>
					void setItemForState( T item, std::map<State, T>& items, State state )
					{
						items[state] = item;
						setState( mState );
					}

					template<typename T>
					bool getItemForState( T& item, std::map<State, T>& items, State state )
					{
						if( items.count( state ) != 0 ) {
							item = items[state];
							return true;
						}

						else if( items.count( State::NORMAL ) != 0 ) {
							item = items[State::NORMAL];
							return true;
						}

						return false;
					}

					void setTintAndOffsetForState( ViewRef view, std::map<State, ci::Color> tints, std::map<State, ci::vec2> offsets, State state );

					// Background Images
					po::scene::ImageViewRef mBackgroundImageView;
					std::map<State, ci::gl::TextureRef> mBackgroundImages;
					std::map<State, ci::vec2> mBackgroundImageOffsets;
					std::map<State, ci::Color> mBackgroundImageTints;

					// Images
					po::scene::ImageViewRef mImageView;
					std::map<State, ci::gl::TextureRef> mImages;
					std::map<State, ci::vec2> mImageOffsets;
					std::map<State, ci::Color> mImageTints;

					// Titles
					ci::TextBox mTitleText;
					po::scene::TextViewRef mTitleTextView;
					std::map<State, std::string> mTitles;
					std::map<State, ci::Font> mTitleFonts;
					std::map<State, ci::vec2> mTitleOffsets;
					std::map<State, ci::Color> mTitleTints;
					std::map<State, ci::vec2> mTitleSizes;

					// Id
					int mId;

					// Event listeners
					int mPressId;
					ci::vec2 mPressStartPos;
					State mPressStartState;

					ButtonSignal mSignalPressed, mSignalToggled;

					void touchBeganInside( po::scene::TouchEvent& event );
					void touchMoved( po::scene::TouchEvent& event );
					void touchEnded( po::scene::TouchEvent& event );
			};
		}
	}
}
