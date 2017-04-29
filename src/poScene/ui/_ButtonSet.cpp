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
				ButtonSetRef ref( new ButtonSet() );
				ref->setup();
				return ref;
			}

			ButtonSet::ButtonSet()
			{

			}

			void ButtonSet::setup()
			{

			}

			// Add Buttons
			void ButtonSet::addButton( ButtonRef button )
			{
				mButtons.push_back( button );
				mButtonConnections[button] += button->getSignalToggled().connect( std::bind( &ButtonSet::buttonToggledHandler, this, ::_1 ) );
			}

			void ButtonSet::addButtons( const std::vector<ButtonRef>& buttons )
			{
				for( auto& button : buttons ) {
					addButton( button );
				}
			}

			// Remove Buttons
			void ButtonSet::removeButton( ButtonRef button )
			{
				std::vector<ButtonRef>::iterator iter = std::find( mButtons.begin(), mButtons.end(), button );

				if( iter != mButtons.end() ) {
					mButtons.erase( iter );
					mButtonConnections.erase( *iter );
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

			void ButtonSet::selectButton( ButtonRef button )
			{
				if( button->getState() == Button::State::SELECTED ) {
					return;
				}

				if( mType == Type::RADIO ) {
					deselectAllButtons();
				}

				button->setState( Button::State::SELECTED );

				if( !mDelegate.expired() ) {
					mDelegate.lock()->buttonWasSelected( button );
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
					if( button->getState() == Button::State::SELECTED ) {
						button->setState( Button::State::NORMAL );

						if( !mDelegate.expired() ) {
							mDelegate.lock()->buttonWasDeselected( button );
						}
					}
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