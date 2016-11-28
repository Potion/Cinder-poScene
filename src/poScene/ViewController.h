#pragma once

#include <memory>

#include "poScene/View.h"

namespace po { namespace scene {
	typedef std::shared_ptr<class ViewController> ViewControllerRef;

	class ViewController {
	public:
		static ViewControllerRef create();

		virtual void setup() {};

		void setView(ViewRef view) { mView = view; }; 
		ViewRef getView() { return mView; };

	protected:
		ViewController();

		ViewRef mView;
	};
}}