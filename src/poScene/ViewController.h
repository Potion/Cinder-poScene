#pragma once

#include <memory>

#include "poScene/View.h"

namespace po { namespace scene {
	typedef std::shared_ptr<class ViewController> ViewControllerRef;

	class ViewController : public std::enable_shared_from_this<ViewController> {
	public:
		static ViewControllerRef create();
		static ViewControllerRef create(ViewRef view );

		ViewController();
		ViewController(ViewRef view);

		virtual void setup() {};
		virtual void update() {};

		virtual void setView(ViewRef view) { mView = view; }; 
		virtual ViewRef getView() { return mView; };

	protected:
		ci::signals::ScopedConnection mUpdateConnection;

	private:
		ViewRef mView;
	};
}}