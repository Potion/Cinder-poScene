#pragma once

#include <memory>

#include "poScene/View.h"

namespace po
{
	namespace scene
	{
		typedef std::shared_ptr<class ViewController> ViewControllerRef;

		class ViewController : public std::enable_shared_from_this<ViewController>
		{
			public:
				static ViewControllerRef create();

				ViewController();

				virtual void update() {};

				virtual ViewRef getView();

			protected:
				ci::signals::ScopedConnection mUpdateConnection;

				// Connections storage
				ci::signals::ConnectionList mConnections;

				// View Events
				virtual void loadView();
				virtual void viewDidLoad() {};
				virtual void viewWillAppear() {};

				virtual void viewWillLayoutSubviews() {};
				virtual void viewDidLayoutSubviews() {};

				void connectViewEvents();
				ci::signals::ConnectionList mViewConnections;

				ViewRef mView;
		};
	}
}