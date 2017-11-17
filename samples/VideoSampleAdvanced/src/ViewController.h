#pragma once

#include "poScene/ViewController.h"
#include "poScene/ShapeView.h"
#include "poScene/VideoView.h"

#include "PlayerController.h"
#include "MovieThumb.h"

namespace sample {
	class ViewController;
	typedef std::shared_ptr<ViewController> ViewControllerRef;

	class ViewController
		: public po::scene::ViewController
	{
	public:
		static ViewControllerRef create();

		void viewDidLoad() override;

	protected:
		ViewController();

	private:

		PlayerControllerRef     mPlayer;
		MovieThumbRef           mMovies[3];

		const int               mNumMovies;
		void                    setUpMovies();
		void                    onThumbnailClick(po::scene::MouseEvent &event);
		void                    onAnimationComplete(MovieThumbRef thumbnail);
		void                    animateControllerToPos(MovieThumbRef movie);

		ci::vec2                mPrimaryDisplayerPosition;

		bool                    mIsControllerInPosition;
	};
}
