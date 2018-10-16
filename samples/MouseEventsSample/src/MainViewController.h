#pragma once

#include "poScene/ViewController.h"
#include "Square.h"
#include "Indicator.h"

namespace sample
{
	class ViewController;
	typedef std::shared_ptr<ViewController> ViewControllerRef;

	class ViewController
		: public po::scene::ViewController
	{
		public:
			static ViewControllerRef create();

			void viewDidLoad() override;

		protected:

		private:
			//	Container to hold the indicators
			ViewRef mIndicatorContainer;

			//	Event names mapped to indicators
			std::vector<std::string> mIndicatorNames;
			std::map<std::string, IndicatorRef> mIndicators;

			//	Container to hold all the squares
			ViewRef mSquareContainer;

			//	Create indicators
			void createIndicators();

			//	Create squares
			void createSquares();

			//	Events on squares
			void onSquareMouseEvent( po::scene::MouseEvent& event, SquareRef square );

			//	Events of scene
			void onMouseEvent( po::scene::MouseEvent& event );

			//	Show the indicator for the events
			void showIndicator( std::string name );
	};
}
