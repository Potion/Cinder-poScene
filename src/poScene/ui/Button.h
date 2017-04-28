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
					enum State {
						NORMAL = 0,
						HIGHLIGHTED,
						SELECTED
					};

					static ButtonRef create();

					void layoutSubviews() override;

					ci::TextBox& getTitleTextBox() { return mTitleText; }

					ButtonSignal& getSignalPressed() { return mSignalPressed; }

					void setBackgroundImage( ci::gl::TextureRef image, State forState = State::NORMAL );
					void setImage( ci::gl::TextureRef image, State forState = State::NORMAL );
					void setTitle( std::string title, State forState = State::NORMAL );
					void setTint( ci::Color tintColor, State forState = State::NORMAL );

				protected:
					Button();
					void setup() override;

					void setState( State state );
					State mCurState;

					// Tint color
					std::map<State, ci::Color> mTintColors;

					// Images
					po::scene::ImageViewRef mBackgroundImageView;
					std::map<State, ci::gl::TextureRef> mBackgroundImages;

					po::scene::ImageViewRef mImageView;
					std::map<State, ci::gl::TextureRef> mImages;

					// Titles
					ci::TextBox mTitleText;
					po::scene::TextViewRef mTitleTextView;
					std::map<State, std::string> mTitles;

					// Event listeners
					int mPressId;
					ci::vec2 mPressStartPos;

					ButtonSignal mSignalPressed;

					void touchBeganInside( po::scene::TouchEvent& event );
					void touchMoved( po::scene::TouchEvent& event );
					void touchEnded( po::scene::TouchEvent& event );
			};
		}
	}
}