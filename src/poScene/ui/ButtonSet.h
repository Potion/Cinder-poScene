#pragma once

#include "poScene/ViewController.h"
#include "poScene/ui/Button.h"

namespace po
{
	namespace scene
	{
		namespace ui
		{
			class ButtonSet;
			typedef std::shared_ptr<ButtonSet> ButtonSetRef;

			class ButtonSetDelegate;
			typedef std::shared_ptr<ButtonSetDelegate> ButtonSetDelegateRef;

			class ButtonSetDelegate
			{
				public:
					virtual void buttonSetDidSelectButton( ButtonSetRef buttonSet, ButtonRef button ) = 0;
					virtual void buttonSetDidDeselectButton( ButtonSetRef buttonSet, ButtonRef button ) = 0;
			};

			class ButtonSet : public po::scene::ViewController
			{
				public:
					enum class Type {
						RADIO,
						CHECKBOX
					};

					static ButtonSetRef create();

					void setDelegate( ButtonSetDelegateRef delegate ) { mDelegate = delegate; };

					void addButton( ButtonRef button, bool andAddToView = true );
					void addButtons( const std::vector<ButtonRef>& buttons, bool andAddToView = true );

					void removeButton( ButtonRef button );
					void removeButtons( const std::vector<ButtonRef>& buttons );

					const std::vector<ButtonRef>& getButtons() { return mButtons; }

					void selectButton( ButtonRef button );
					void selectAllButtons();

					void deselectButton( ButtonRef button );
					void deselectAllButtons();

					std::vector<ButtonRef> getSelectedButtons();

					Type getType() { return mType; }
					void setType( Type Type );

				protected:
					ButtonSet();

					std::weak_ptr<ButtonSetDelegate> mDelegate;

					Type mType;

					// Buttons
					std::vector<ButtonRef> mButtons;

					std::map<ButtonRef, ci::signals::ConnectionList> mButtonConnections;

					// Event listeners
					void buttonToggledHandler( ButtonRef button );

			};
		}
	}
}