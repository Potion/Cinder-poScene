#include "poScene/ui/ButtonSet.h"

using namespace std::placeholders;

namespace po
{
	namespace scene
	{
		namespace ui
		{
			ButtonSetRef ButtonSet::create()
			{
				return ButtonSetRef( new ButtonSet() );
			}

			ButtonSet::ButtonSet()
				: mType( Type::RADIO )
			{

			}

			// Add Buttons
			void ButtonSet::addButton( ButtonRef button, bool addToView )
			{
				button->setType( Button::Type::TOGGLE );
				mButtons.push_back( button );
				mButtonConnections[button] += button->getSignalToggled().connect( std::bind( &ButtonSet::buttonToggledHandler, this, ::_1 ) );

				if( addToView ) {
					getView()->addSubview( button );
				}
			}

			void ButtonSet::addButtons( const std::vector<ButtonRef>& buttons, bool addToView )
			{
				for( auto& button : buttons ) {
					addButton( button, addToView );
				}
			}

			// Remove Buttons
			void ButtonSet::removeButton( ButtonRef button )
			{
				std::vector<ButtonRef>::iterator iter = std::find( mButtons.begin(), mButtons.end(), button );

				if( iter != mButtons.end() ) {
					mButtons.erase( iter );
					mButtonConnections.erase( *iter );

					if( button->getSuperview() == getView() ) {
						button->removeFromSuperview();
					}
				}
			}

			void ButtonSet::removeButtons( const std::vector<ButtonRef>& buttons )
			{
				for( auto& button : buttons ) {
					removeButton( button );
				}
			}

			// Return a list of selected buttons
			// If radio should only be one, checkbox could be multiple
			std::vector<ButtonRef> ButtonSet::getSelectedButtons()
			{
				std::vector<ButtonRef> selectedButtons;

				for( auto& button : mButtons ) {
					if( button->getState() == Button::State::SELECTED ) {
						selectedButtons.push_back( button );
					}
				}

				return selectedButtons;
			}

			void ButtonSet::selectButton( ButtonRef selectedButton )
			{
				if( mType == Type::RADIO ) {
					for( auto& button : mButtons ) {
						if( button->getState() == Button::State::SELECTED && button != selectedButton ) {
							button->setState( Button::State::NORMAL );

							if( !mDelegate.expired() ) {
								ButtonSetRef self = std::dynamic_pointer_cast<ButtonSet>( shared_from_this() );
								mDelegate.lock()->buttonSetDidDeselectButton( self, button );
							}
						}
					}
				}

				if( selectedButton->getState() != Button::State::SELECTED ) {
					selectedButton->setState( Button::State::SELECTED );
				}

				if( !mDelegate.expired() ) {
					ButtonSetRef self = std::dynamic_pointer_cast<ButtonSet>( shared_from_this() );
					mDelegate.lock()->buttonSetDidSelectButton( self, selectedButton );
				}
			}
			
			
			void ButtonSet::deselectButton( ButtonRef deSelectedButton )
			{
				deSelectedButton->setState( Button::State::NORMAL );
				
				if( !mDelegate.expired() ) {
					ButtonSetRef self = std::dynamic_pointer_cast<ButtonSet>( shared_from_this() );
					mDelegate.lock()->buttonSetDidDeselectButton( self, deSelectedButton );
				}
			}
			
			void ButtonSet::selectAllButtons()
			{
				CI_ASSERT_MSG( mType == Type::CHECKBOX, "You can not select all buttons in a radio button set." );

				for( auto& button : mButtons ) {
					selectButton( button );
				}
			}

			void ButtonSet::deselectAllButtons()
			{
				for( auto& button : mButtons ) {
					deselectButton( button );
				}
			}

			// -------------------------------
			// Event listeners
			void ButtonSet::buttonToggledHandler( ButtonRef button )
			{
				switch( mType ) {
					case Type::RADIO:

						// Don't allow buttons to be de-selected on their own
						if( button->getState() == Button::State::NORMAL ) {
							button->setState( Button::State::SELECTED );
						}

						// Previously unselected button selected
						else {
							selectButton( button );
						}

						return;
				}
			}
		}
	}
}
