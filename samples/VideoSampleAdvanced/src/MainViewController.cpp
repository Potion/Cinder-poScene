#include "MainViewController.h"
#include "cinder/Log.h"

using namespace po::scene;

namespace sample
{
	ViewControllerRef ViewController::create()
	{
		return ViewControllerRef( new ViewController() );
	}

	ViewController::ViewController()
		: mNumMovies( 3 )
		, mIsControllerInPosition( false )
	{
	}

	void ViewController::viewDidLoad()
	{
		//  create and add the main player
		mPlayerController = PlayerController::create();
		mPlayerController->setAlignment( po::scene::Alignment::TOP_CENTER );
		mPlayerController->setPosition( ci::app::getWindowWidth() / 2, -mPlayerController->getHeight() / 2 ); // centered, just above screen
		mPlayerController->setAlpha( 0.0f );
		getView()->addSubview( mPlayerController );

		//  set location for top/center of primary display
		mPrimaryDisplayerPosition = ci::vec2( ci::app::getWindowWidth() / 2, 50 );

		try {
			//  load the three videos
			ci::fs::path moviePath[mNumMovies];
			moviePath[0] = ci::app::getAssetPath( "Placeholder_Video-RH3i7qONrT4.mp4" );
			moviePath[1] = ci::app::getAssetPath( "Placeholder_Video-ScMzIvxBSi4.mp4" );
			moviePath[2] = ci::app::getAssetPath( "Video-lBP2Ij86Ua4.mp4" );

			//  load the movies, create po::scene movie references, then create MovieThumb objects
			for( int i = 0; i < mNumMovies; i++ ) {
				auto qtMovie = ci::qtime::MovieGl::create( moviePath[i] );
				auto poVideoView = VideoViewGl::create();
				poVideoView->setMovieRef( qtMovie );
				mMoviePlayers.push_back( MovieThumb::create( poVideoView ) );
				//mMovies[i]->setDrawBounds(true);
				getView()->addSubview( mMoviePlayers[i] );
			}

			setUpMovies();

		}
		catch( ... ) {
			std::cout << "Videos did not load successfully";
		}        
	}

    void ViewController::update()
    {
        for( int i = 0; i < mMoviePlayers.size(); i++ ) {
            // due to how the videoView's current implementation, movieView(inside mMoviewPlayer) size remains at 0 on movie load
            // enforcing alignment here
            
            mMoviePlayers[i]->setAlignment( po::scene::Alignment::TOP_CENTER );
            mMoviePlayers[i]->setDrawBounds( true );
        }
    }
	void ViewController::setUpMovies()
	{
		float thumbnailScale = 0.2f;
		float screenInterval = ci::app::getWindowWidth() / ( mNumMovies * 2 );

		for( int i = 0; i < mMoviePlayers.size(); i++ ) {
			//  set scale of movie so it plays at width of 640 px (same as mPlayer width)
			float actualWidth = mMoviePlayers[i]->getUnderlyingMovie()->getMovieRef()->getWidth();
			float scale = mPlayerController->getWidth() / actualWidth;
            
			mMoviePlayers[i]->setPlayerScale( ci::vec2( scale, scale ) );

			//  set position based on its height
			float yOffsetForPlayer = ( mMoviePlayers[i]->getUnderlyingMovie()->getHeight() * scale ) * 0.5f;
			ci::vec2 playerPosition( mPrimaryDisplayerPosition.x, mPrimaryDisplayerPosition.y + yOffsetForPlayer );
			mMoviePlayers[i]->setPlayerPos( playerPosition );

			//  calculate the thumbnail scale, then set appropriate variable in mMovie object
			mMoviePlayers[i]->setThumbnailScale( mMoviePlayers[i]->getPlayerScale() * thumbnailScale );
			mMoviePlayers[i]->setScale( mMoviePlayers[i]->getThumbnailScale() );

			//  calculate the thumbnail position, then set appropriate variable in mMovie object
			float xPos = ( ( i * 2 ) + 1 ) * screenInterval;
			mMoviePlayers[i]->setThumbnailPos( ci::vec2( xPos, ci::app::getWindowHeight() * 0.8f ) );
            mMoviePlayers[i]->setPosition( mMoviePlayers[i]->getThumbnailPos() );

			//  add listeners
			mMoviePlayers[i]->getSignal( MouseEvent::Type::DOWN_INSIDE ).connect( std::bind( &ViewController::onThumbnailClick, this, std::placeholders::_1 ) );
			mMoviePlayers[i]->getSignalAnimationComplete().connect( std::bind( &ViewController::onAnimationComplete, this, std::placeholders::_1 ) );
		}
	}

	void ViewController::onThumbnailClick( MouseEvent& event )
	{
		ViewRef view = event.getSource();
		MovieThumbRef playerView = std::static_pointer_cast<MovieThumb>( view );

		for( int i = 0; i < mNumMovies; i++ ) {

			if( mMoviePlayers[i] == playerView ) {

				//  begin animation to primary displayer position, adjusted for center alignment
				mMoviePlayers[i]->animateToPlayer();
				animateControllerToPos( mMoviePlayers[i] );

				//  move primary movie to top position
				getView()->moveSubviewToFront( mMoviePlayers[i] );
			}
			else if( !mMoviePlayers[i]->getIsHome() ) {

				//  move other movies back if they're not in their home positions
				mMoviePlayers[i]->animateOutOfPlayerPosition();
			}
		}
	}

	void ViewController::onAnimationComplete( MovieThumbRef thumbnail )
	{
		mPlayerController->setPrimaryMovie( thumbnail->getUnderlyingMovie() );
	}

	void ViewController::animateControllerToPos( MovieThumbRef movie )
	{
		//  animate player controller to 50 px below the movie

		float x = mPlayerController->getPosition().x;
		//  find the height of the new movie when fully expanded
		float movieHeight = movie->getUnderlyingMovie()->getHeight() * movie->getPlayerScale().y;
		//  push the controller to 50 px below that
		float y = mPrimaryDisplayerPosition.y + movieHeight + 50;
		ci::vec2 newPos( x, y );

		if( !mIsControllerInPosition ) {
			ci::app::timeline().apply( &mPlayerController->getPositionAnim(), newPos, 2.f, ci::EaseOutBounce() );
		}
		else {
			ci::app::timeline().apply( &mPlayerController->getPositionAnim(), newPos, 2.f );
		}

		//  fade player in if it's transparent
		if( !mIsControllerInPosition ) {
			ci::app::timeline().apply( &mPlayerController->getAlphaAnim(), 1.f, 2.f );
			mIsControllerInPosition = true;
		}
	}
}
