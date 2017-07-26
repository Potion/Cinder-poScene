/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define GLSL(src) "#version 110\n" #src

#if defined( CINDER_MSW )
#include <windows.h>
#undef min
#undef max
#include <gl/gl.h>
#elif defined( CINDER_COCOA_TOUCH )
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined( CINDER_MAC )
#include <OpenGL/gl.h>
#endif

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"

#include "poScene/View.h"
#include "poScene/ShapeView.h"
#include "poScene/Scene.h"

namespace po
{
	namespace scene
	{

		ci::gl::BatchRef View::mBackgroundBatch = nullptr;

		static uint32_t OBJECT_UID		= 0;
		static const int ORIGIN_SIZE	= 2;


		// Masking Shader
		static ci::gl::GlslProgRef mMaskShader = nullptr;

		static const char* maskVertShader = CI_GLSL( 150,
		                                    uniform mat4 ciModelViewProjection;
		                                    in vec4 ciPosition;
		                                    in vec2 ciTexCoord0;
		                                    out highp vec2 TexCoord;

		                                    void main()
		{
			TexCoord        = ciTexCoord0.st;
			gl_Position     = ciModelViewProjection * ciPosition;
		}
		                                           );

		static const char* maskFragShader = CI_GLSL( 150,
		                                    in highp vec2 TexCoord;

		                                    uniform sampler2D tex;
		                                    uniform sampler2D mask;

		                                    out vec4 color;

		                                    void main( void )
		{
			vec2 c0 = vec2( TexCoord.s, TexCoord.t );

			vec4 rgbValue       = texture( tex, c0 );
			vec4 alphaValue     = texture( mask, c0 );

			color.rgb     = rgbValue.rgb;
			color.a       = alphaValue.a * rgbValue.a;
		}
		                                           );

		ViewRef View::create( std::string name )
		{
			return std::shared_ptr<View>( new View( name ) );
		}

		ViewRef View::create( ci::vec2 size, std::string name )
		{
			ViewRef ref = create( name );
			ref->setSize( size );
			return ref;
		}

		View::View( std::string name )
			: mUid( OBJECT_UID++ )
			, mName( name )
			, mTag( -1 )
			, mDrawOrder( 0 )
			, mPosition( 0.f, 0.f )
			, mScale( 1.f, 1.f )
			, mRotation( 0 )
			, mOffset( 0.f, 0.f )
			, mAlpha( 1.f )
			, mAppliedAlpha( 1.f )
			, mPositionAnim( ci::vec2( 0.f, 0.f ) )
			, mScaleAnim( ci::vec2( 1.f, 1.f ) )
			, mRotationAnim( 0 )
			, mOffsetAnim( ci::vec2( 0.f, 0.f ) )
			, mAlphaAnim( 1.f )
			, mAlignment( Alignment::TOP_LEFT )
			, mMatrixOrder( MatrixOrder::TRS )
			, mBackgroundColor( ci::ColorA( 1.0f, 1.0f, 1.0f, 0.0f ) )
			, mFillColor( 1.f, 1.f, 1.f )
			, mFillColorAnim( ci::Color( 1.f, 1.f, 1.f ) )
			, mFillEnabled( true )
			, mStrokeColor( 255, 255, 255 )
			, mStrokeEnabled( false )
			, mPixelSnapping( false )
			, mIsPremultiplied( false )
			, mBounds( ci::Rectf::zero() )
			, mDrawBounds( false )
			, mUseElasticBounds( true )
			, mBoundsColor( 1.f, 0, 0 )
			, mSuperviewShouldIgnoreInBounds( false )
			, mBoundsDirty( true )
			, mFrameDirty( true )
			, mVisible( true )
			, mInteractionEnabled( true )
			, mHasScene( false )
			, mHasSuperview( false )
			, mIsMasked( false )
			, mMask( nullptr )
			, mNeedsLayout( false )
		{
			//	Initialize our animations
			initAttrAnimations();
		}

		View::~View()
		{
			//	Make sure to clear the fbo w/Cinder bug fix
			removeSuperview();
			removeScene();
		}


		//------------------------------------
		//	Update & Draw Trees
		//------------------------------------

		void View::updateTree()
		{
			//	Update our tween tie-in animations
			updateAttributeAnimations();

			if( mIsMasked ) { mMask->updateTree(); }

			// Layout subviews
			layoutIfNeeded();

			//	Call our update function
			update();

			//	Update subviews
			// TODO: I think this copy is a holdover from the switch to deque from vector
			// not sure if we still need it
			// -SV
			std::deque<ViewRef> subviews( mSubviews );

			for( ViewRef& subview : subviews ) {
				if( subview->mVisible && subview->hasSuperview() ) {
					subview->updateTree();
				}
			}
		}

		void View::drawBackground()
		{
			if( mBackgroundColor.a <= 0.0f ) {
				return;
			}

			if( mBackgroundBatch == nullptr ) {
				ci::gl::GlslProgRef shader = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
				mBackgroundBatch = ci::gl::Batch::create( ci::geom::Rect( ci::Rectf( 0, 0, 1, 1 ) ), shader );
			}

			ci::gl::ScopedColor color( mBackgroundColor );

			ci::gl::ScopedModelMatrix mModelView;
			ci::gl::scale( ci::vec2( getBounds().getSize() ) );
			mBackgroundBatch->draw();
		}

		void View::draw()
		{
			for( ViewRef& subview : mSubviews ) {
				subview->drawTree();
			}
		}

		void View::beginDrawTree()
		{
			//	Update our draw order
			if( hasScene() ) { mDrawOrder = mScene.lock()->getNextDrawOrder(); }

			//	Set applied alpha
			if( hasSuperview() ) {
				mAppliedAlpha = getSuperview()->getAppliedAlpha() * mAlpha;
			}
			else {
				mAppliedAlpha = mAlpha;
			}

			//	Push our Matrix
			ci::gl::pushModelView();
			setTransformation();
		}

		void View::drawTree()
		{
			if( mVisible ) {
				//  Draw
				beginDrawTree();
				drawBackground();

				if( !mIsMasked ) {
					draw();
					finishDrawTree();
				}
				else {
					captureMasked();
					finishDrawTree();
					drawMasked();
				}
			}
		}

		void View::finishDrawTree()
		{

			//	Draw bounds if necessary
			if( mDrawBounds ) { drawBounds(); }

			//	Pop our Matrix
			ci::gl::popModelView();
		}

		void View::matrixTree()
		{
			beginDrawTree();

			for( ViewRef& subview : mSubviews ) {
				subview->matrixTree();
			}

			finishDrawTree();
		}

		void View::calculateMatrices( bool calcluateSuperViews )
		{
			if( calcluateSuperViews && hasSuperview() ) {
				getSuperview()->calculateMatrices();
			}
			else {
				matrixTree();
			}
		}


		//------------------------------------
		//	Fbo Drawing
		//------------------------------------

		void View::captureMasked()
		{
			//	Save the window buffer
			{
				//  Draw ourself into FBO
				ci::gl::ScopedFramebuffer buffer( getScene()->getWindowFbo() );
				ci::gl::clear( ci::ColorA::zero() );

				ci::gl::ScopedModelMatrix();
				draw();
			}

			{
				//  Draw mask into Masking FBO (replace with Mask buffer in GLNext)
				ci::gl::ScopedFramebuffer buffer( getScene()->getMaskFbo() );
				ci::gl::clear( ci::ColorA::zero() );

				ci::gl::ScopedModelMatrix();
				mMask->drawTree();
			}
		}

		void View::drawMasked()
		{
			ci::gl::pushModelView();
			ci::gl::setMatricesWindow( ci::app::getWindowSize() );

			// Bind FBO textures
			ci::gl::ScopedTextureBind fboBind( getScene()->getWindowFbo()->getColorTexture(), 0 );
			ci::gl::ScopedTextureBind maskBind( getScene()->getMaskFbo()->getColorTexture(), 1 );

			//	Bind Shader
			ci::gl::ScopedGlslProg maskShader( mMaskShader );

			//	Set uniforms
			mMaskShader->uniform( "tex", 0 );
			mMaskShader->uniform( "mask", 1 );

			//	Draw
			ci::gl::ScopedBlendAlpha alphaBlendScoped;
			ci::gl::ScopedColor fillColorScoped( ci::ColorA( getFillColor(), getAppliedAlpha() ) );

			ci::gl::drawSolidRect( getScene()->getWindowFbo()->getBounds() );

			ci::gl::popModelView();
		}


		//------------------------------------
		//	Masking
		//------------------------------------

		//
		//	Apply the mask (as a shaperef)
		//
		void View::setMask( ShapeViewRef mask )
		{
			//	If successful, try to build the shader
			if( !mMaskShader ) {
				try {
					mMaskShader = ci::gl::GlslProg::create( maskVertShader, maskFragShader );
				}
				catch( ci::gl::GlslProgCompileExc e ) {
					ci::app::console() << "Could not load shader: " << e.what() << std::endl;
					return;
				}
			}

			//	Set our vars
			mMask       = mask;
			mIsMasked   = true;
		}

		//
		//	Remove the mask, and stop caching to FBO unless requested
		//
		ViewRef View::removeMask()
		{
			mIsMasked = false;

			ViewRef mask = mMask;
			mMask.reset();
			return mask;
		}


		//------------------------------------
		//	Texture Caching
		//------------------------------------

		ci::gl::TextureRef View::createTexture()
		{
			//	We have to be visible, so if we aren't temporarily turn it on
			bool visible = mVisible;
			setVisible( true );

			// Create an FBO to draw into
			ci::gl::Fbo::Format format;
			format.setSamples( 1 );
			format.enableDepthBuffer( false );

			//	Create and Bind the FBO
			ci::gl::FboRef fbo = ci::gl::Fbo::create( ( int )getWidth(), ( int )getHeight(), format );
			ci::gl::ScopedFramebuffer fboBind( fbo );

			//	Set the viewport
			ci::gl::ScopedViewport vp( ci::ivec2( 0 ), fbo->getSize() );

			//	Set Ortho camera to fbo bounds, save matrices and push camera
			ci::gl::pushMatrices();
			ci::gl::setMatricesWindow( fbo->getWidth(), fbo->getHeight() );

			//	Clear the FBO
			ci::gl::clear( ci::ColorA( 0.f, 0.f, 0.f, 0.f ) );

			//	Draw into the FBO
			float curAppliedAlpha = mAppliedAlpha;
			mAppliedAlpha = mAlpha;

			draw();

			mAppliedAlpha = curAppliedAlpha;
			//	Set the camera up for the window
			ci::gl::popMatrices();

			//	Return to previous visibility
			setVisible( visible );

			ci::gl::TextureRef tex = fbo->getColorTexture();
			return tex;
		}


		//------------------------------------
		//	Attributes
		//------------------------------------

		//
		//	Set the position
		//
		View& View::setPosition( float x, float y )
		{
			ci::vec2 newPos( x, y );

			mPositionAnim.stop();
			mPositionAnim = newPos;

			if( newPos != mPosition ) {
				mPosition = newPos;

				if( hasSuperview() ) {
					mSuperview.lock()->setNeedsLayout();
				}
			}

			return *this;
		}

		//
		//	Set the scale
		//
		View& View::setScale( float x, float y )
		{
			ci::vec2 newScale( x, y );

			mScaleAnim.stop();
			mScaleAnim = newScale;

			if( newScale != mScale ) {
				mScale = newScale;

				if( hasSuperview() ) {
					mSuperview.lock()->setNeedsLayout();
				}
			}

			return *this;
		}

		//
		//	Set the rotation
		//
		View& View::setRotation( float rotation )
		{
			if( rotation >= M_PI * 2 || rotation <= -M_PI * 2 ) {
				rotation = fmodf( rotation, ( float )( M_PI * 2 ) );
			}

			mRotationAnim.stop();
			mRotationAnim = rotation;

			if( mRotation != rotation ) {
				mRotation = rotation;

				if( hasSuperview() ) {
					mSuperview.lock()->setNeedsLayout();
				}
			}

			return *this;
		}

		//
		//	Set the alpha
		//
		View& View::setAlpha( float alpha )
		{
			mAlphaAnim.stop();
			mAlpha = ci::math<float>::clamp( alpha, 0.f, 1.f );
			mAlphaAnim = mAlpha;
			return *this;
		}

		//
		// Set the fill color
		View& View::setFillColor( ci::ColorA color )
		{
			setFillColor( ci::Color( color ) );
			setAlpha( color.a );
			return *this;
		}

		View& View::setFillColor( ci::Color color )
		{
			mFillColor = color;
			mFillColorAnim.stop();
			mFillColorAnim = mFillColor;
			return *this;
		}

		//
		//	Offset the whole View from the origin
		//
		View& View::setOffset( float x, float y )
		{
			mOffsetAnim.stop();
			mOffset = ci::vec2( x, y );
			mOffsetAnim = mOffset;
			mFrameDirty = true;

			//	If we are manually setting the offset, we can't have alignment
			setAlignment( Alignment::NONE );

			return *this;
		}

		//
		//	Check if we are visible, and up the scene graph
		//	Somewhat slow, could be better implementation (i.e. superviews set a var on their subviews like "superviewIsVisible")
		//
		bool View::isVisible()
		{
			if( !mVisible ) { return false; }

			ViewRef superview = getSuperview();

			while( superview ) {
				if( !superview->mVisible ) {
					return false;
				}

				superview = superview->getSuperview();
			}

			return true;
		}


		//------------------------------------
		//  Animation
		//------------------------------------

		void View::initAttrAnimations()
		{
			//	Initialize the isComplete() method of each tween
			mPositionAnim.stop();
			mScaleAnim.stop();
			mRotationAnim.stop();
			mOffsetAnim.stop();
			mAlphaAnim.stop();
			mFillColorAnim.stop();
		}

		//
		//	TODO: Find a better way to do this, probably some sort of map of active properties.
		//
		void View::updateAttributeAnimations()
		{
			bool frameChanged = ( mPosition != mPositionAnim.value() || mScale != mScaleAnim.value() || mRotation != mRotationAnim.value() || mOffset != mOffsetAnim.value() );

			mPosition	= mPositionAnim;
			mScale		= mScaleAnim;
			mRotation	= mRotationAnim;
			mOffset		= mOffsetAnim;
			mAlpha		= mAlphaAnim;
			mFillColor	= mFillColorAnim;

			if( frameChanged ) {
				if( mHasSuperview ) {
					mSuperview.lock()->setNeedsLayout();
				}
			}
		}


		//------------------------------------
		//  Alignment
		//------------------------------------

		View& View::setAlignment( Alignment alignment, bool preservePositioning )
		{
			mAlignment = alignment;

			ci::vec2 mPreviousOffset = mOffset;

			calculateOffset();

			if( preservePositioning ) {
				setPosition( getPosition() + ( ( mPreviousOffset - mOffset ) * mScale ) );
			}

			return *this;
		}

		View& View::calculateOffset()
		{
			if( mAlignment == Alignment::NONE ) { return *this; }

			ci::Rectf bounds = getBounds();

			switch( mAlignment ) {
				case Alignment::NONE:
				case Alignment::TOP_LEFT:
					mOffset = ci::vec2( 0, 0 );
					break;

				case Alignment::TOP_CENTER:
					mOffset = ci::vec2( -bounds.getWidth() / 2.f, 0 );
					break;

				case Alignment::TOP_RIGHT:
					mOffset = ci::vec2( -bounds.getWidth(), 0 );
					break;

				case Alignment::CENTER_LEFT:
					mOffset = ci::vec2( 0, -bounds.getHeight() / 2.f );
					break;

				case Alignment::CENTER_CENTER:
					mOffset = ci::vec2( -bounds.getWidth() / 2.f, -bounds.getHeight() / 2.f );
					break;

				case Alignment::CENTER_RIGHT:
					mOffset = ci::vec2( -bounds.getWidth(), -bounds.getHeight() / 2.f );
					break;

				case Alignment::BOTTOM_LEFT:
					mOffset = ci::vec2( 0, -bounds.getHeight() );
					break;

				case Alignment::BOTTOM_CENTER:
					mOffset = ci::vec2( -bounds.getWidth() / 2.f, -bounds.getHeight() );
					break;

				case Alignment::BOTTOM_RIGHT:
					mOffset = ci::vec2( -bounds.getWidth(), -bounds.getHeight() );
					break;
			}

			mOffsetAnim = mOffset;

			return *this;
		}


		//------------------------------------
		//  Transformation
		//------------------------------------

		void View::setTransformation()
		{
			switch( mMatrixOrder ) {
				case MatrixOrder::TRS:
					ci::gl::translate( mPixelSnapping ? round( mPosition ) : mPosition );
					ci::gl::rotate( mRotation );
					ci::gl::scale( mScale );
					break;

				case MatrixOrder::RST:
					ci::gl::rotate( mRotation );
					ci::gl::scale( mScale );
					ci::gl::translate( mPixelSnapping ? round( mPosition ) : mPosition );
					break;
			}

			ci::gl::translate( mPixelSnapping ? round( mOffset ) : mOffset );
			mMatrix.set( ci::gl::getModelMatrix(), ci::gl::getProjectionMatrix(), ci::Area( ci::gl::getViewport().first, ci::gl::getViewport().second ) );
		}


		ci::vec2 View::viewToLocal( const ci::vec2& point, ViewRef View )
		{
			return windowToLocal( View->localToWindow( point ) );
		}

		ci::vec2 View::localToView( const ci::vec2& point, ViewRef View )
		{
			return View->windowToLocal( localToWindow( point ) );
		}

		ci::vec2 View::sceneToLocal( const ci::vec2& scenePoint )
		{
			SceneRef scene = getScene();

			if( scene != nullptr ) {
				return scene->getRootView()->localToView( scenePoint, shared_from_this() );
			}

			return ci::vec2();
		}

		ci::vec2 View::localToScene( const ci::vec2& point )
		{
			SceneRef scene = getScene();

			if( scene != nullptr ) {
				return localToView( point, scene->getRootView() );
			}

			return ci::vec2();
		}

		ci::vec2 View::sceneToWindow( const ci::vec2& point )
		{
			SceneRef scene = getScene();

			if( scene != nullptr ) {
				return scene->getRootView()->localToWindow( point );
			}

			return point;
		}

		ci::vec2 View::windowToScene( const ci::vec2& point )
		{
			SceneRef scene = getScene();

			if( scene != nullptr ) {
				return scene->getRootView()->windowToLocal( point );
			}

			return ci::vec2();
		}

		ci::vec2 View::windowToLocal( const ci::vec2& windowPoint )
		{

			//calculateMatrices();
			return mMatrix.globalToLocal( windowPoint );
		}

		ci::vec2 View::localToWindow( const ci::vec2& scenePoint )
		{
			if( mHasScene ) {
				//calculateMatrices();
				return mMatrix.localToGlobal( scenePoint );
			}

			return ci::vec2();
		}

		//
		//  This is used for hit-testing all Views
		//  Override this function to do any type of custom
		//
		bool View::pointInside( const ci::vec2& point )
		{
			ci::vec2 pos = windowToLocal( point );
			return getBounds().contains( pos );
		}


		//------------------------------------
		//  Superview + Scene
		//------------------------------------

		void View::setScene( SceneRef scene )
		{
			//if( mHasScene && mScene.lock() == scene ) {
			//	return;
			//}

			mScene = scene;
			mHasScene = mScene.lock() ? true : false;

			if( hasScene() ) {
				mScene.lock()->trackView( shared_from_this() );

				for( ViewRef& subview : mSubviews ) {
					subview->setScene( scene );
				}

				mSignalWillAppear.emit( shared_from_this() );
			};
		}

		SceneRef View::getScene() { return mScene.lock(); }

		bool View::hasScene() { return mHasScene; }

		void View::removeScene()
		{
			SceneRef scene = mScene.lock();

			if( scene ) {
				scene->untrackView( shared_from_this() );

				for( ViewRef& subview : mSubviews ) {
					subview->removeScene();
				}
			}

			mScene.reset();
			mHasScene = false;
		}

		void View::setSuperview( ViewRef containerView )
		{
			mSuperview = containerView;
			mHasSuperview = mSuperview.lock() ? true : false;
		}

		ViewRef View::getSuperview() const
		{
			return mSuperview.lock();
		}

		bool View::hasSuperview() { return mHasSuperview; }

		void View::removeSuperview()
		{
			mSuperview.reset();
			mHasSuperview = false;
		}

		void View::setSubviewSuperviewAndScene( ViewRef view )
		{
			//	See if the View is already a subview of another View.
			if( view->getSuperview() != nullptr ) {
				view->getSuperview()->removeSubview( view );
			}

			//	Assign ourselves as the superview
			view->setSuperview( shared_from_this() );
			view->setScene( mScene.lock() );
		}

		View& View::removeFromSuperview()
		{
			if( getSuperview() != nullptr ) {
				getSuperview()->removeSubview( shared_from_this() );
			}

			return *this;
		}

		//------------------------------------
		//  Subviews
		//------------------------------------

		//
		//  Add Subview
		//

		View& View::addSubview( ViewRef view, bool localize )
		{
			CI_ASSERT_MSG( view != shared_from_this(), "po::Scene: Attempting to add view to self, not allowed." );

			if( localize ) {
				view->setPosition( viewToLocal( view->getPosition(), view->getSuperview() ) );
			}

			setSuperviewAndScene( view );
			mSubviews.push_back( view );

			subviewsChanged();

			return *this;
		}

		View& View::addSubviews( std::vector<ViewRef> views, bool localize )
		{
			for( auto& view : views ) {
				if( localize ) {
					view->setPosition( viewToLocal( view->getPosition(), view->getSuperview() ) );
				}

				setSuperviewAndScene( view );
				mSubviews.push_back( view );
			}

			subviewsChanged();

			return *this;
		}

		View& View::insertSubviewAt( int index, ViewRef view, bool localize )
		{
			if( localize ) {
				view->setPosition( viewToLocal( view->getPosition(), view->getSuperview() ) );
			}

			setSuperviewAndScene( view );
			mSubviews.insert( mSubviews.begin() + index, view );
			subviewsChanged();

			return *this;
		}

		View& View::insertSubviewBefore( ViewRef view, ViewRef before, bool localize )
		{
			if( localize ) {
				view->setPosition( viewToLocal( view->getPosition(), view->getSuperview() ) );
			}

			setSuperviewAndScene( view );
			mSubviews.insert( mSubviews.begin() + getIndexForSubview( before ), view );
			subviewsChanged();

			return *this;
		}

		View& View::insertSubviewAfter( ViewRef view, ViewRef after, bool localize )
		{
			if( localize ) {
				view->setPosition( viewToLocal( view->getPosition(), view->getSuperview() ) );
			}

			setSuperviewAndScene( view );
			mSubviews.insert( mSubviews.begin() + getIndexForSubview( after ) + 1, view );
			subviewsChanged();

			return *this;
		}

		void View::subviewsChanged()
		{
			calculateOffset();
			calculateMatrices();
			mNeedsLayout = true;
		}

		//	-------------------------------
		//  Layout Subviews

		void View::layoutSubviews()
		{
			mNeedsLayout = false;
			mSignalWillLayoutSubviews.emit( shared_from_this() );

			for( auto& subview : mSubviews ) {
				subview->layoutIfNeeded();
			}

			mSignalDidLayoutSubviews.emit( shared_from_this() );
		}

		void View::layoutIfNeeded()
		{
			if( mNeedsLayout ) {
				layoutSubviews();
			}
		}

		const std::deque<ViewRef>& View::getSubviews()
		{
			return mSubviews;
		};

		bool View::hasSubviews()
		{
			return ( mSubviews.size() != 0 );
		}

		bool View::hasSubview( ViewRef view )
		{
			return view->getSuperview()->getUID() == getUID();
		}

		int View::getIndexForSubview( const ViewRef& view )
		{
			std::deque<ViewRef>::iterator iter = std::find( mSubviews.begin(), mSubviews.end(), view );

			if( iter != mSubviews.end() ) { return ( int )std::distance( mSubviews.begin(), iter ); }

			return INVALID_SUBVIEW_INDEX;
		}

		ViewRef View::getSubviewForIndex( int index )
		{
			if( index < 0 || index >= mSubviews.size() ) { return ViewRef(); }

			return *( mSubviews.begin() + index );
		}

		ViewRef View::getSubviewForUID( uint32_t uid )
		{
			//	See if it is us
			// NOTE: I don't like this so commenting out - Steve
			//if (mUid == uid) return shared_from_this();

			//	Go through our tree to find any View with UID
			for( ViewRef& view : mSubviews ) {
				if( view->getUID() == uid ) {
					return view;
				}
				else {
					ViewRef foundView = view->getSubviewForUID( uid );

					if( foundView ) { return foundView; }
				}
			}

			//	Not found
			return nullptr;
		}

		ViewRef View::getSubviewForName( const std::string& name )
		{
			for( ViewRef& view : mSubviews ) {
				if( view->getName() == name ) { return view; }
			}

			return nullptr;
		}

		ViewRef View::getFirstSubview()
		{
			if( mSubviews.empty() ) {
				return nullptr;
			}

			return mSubviews.front();
		}

		ViewRef View::getLastSubview()
		{
			if( mSubviews.empty() ) {
				return nullptr;
			}

			return mSubviews.back();
		}

		//
		//  Remove Subview
		//

		ViewRef View::removeSubview( ViewRef view )
		{
			std::deque<ViewRef>::iterator iter = std::find( mSubviews.begin(), mSubviews.end(), view );

			if( iter != mSubviews.end() ) {
				( *iter )->removeSuperview();
				( *iter )->removeScene();

				mSubviews.erase( iter );
				subviewsChanged();

				return view;
			}
			else {
				return nullptr;
			}
		}

		ViewRef View::removeSubviewAtIndex( int index )
		{
			if( index <= 0 || index >= mSubviews.size() ) {
				return nullptr;
			}
			else {
				ViewRef subview = mSubviews[index];

				mSubviews.erase( mSubviews.begin() + index );

				subview->removeSuperview();
				subview->removeScene();

				subviewsChanged();

				return subview;
			}
		}

		void View::removeAllSubviews()
		{
			for( ViewRef& view : mSubviews ) {
				view->removeSuperview();
				view->removeScene();
			}

			subviewsChanged();

			mSubviews.clear();
		}

		//
		//  Move Subviews
		//

		View& View::moveSubviewToFront( ViewRef view )
		{
			auto viewIter = std::find( mSubviews.begin(), mSubviews.end(), view );

			if( viewIter != mSubviews.end() ) {
				mSubviews.erase( viewIter );
				mSubviews.push_back( view );
			}

			return *this;
		}

		View& View::moveSubviewForward( ViewRef view )
		{
			auto viewIter = std::find( mSubviews.begin(), mSubviews.end(), view );

			if( viewIter != mSubviews.end() && *viewIter != mSubviews.back() ) {
				std::iter_swap( viewIter, ++viewIter );
			}

			return *this;
		}

		View& View::moveSubviewToBack( ViewRef view )
		{
			auto viewIter = std::find( mSubviews.begin(), mSubviews.end(), view );

			if( viewIter != mSubviews.end() ) {
				mSubviews.erase( viewIter );
				mSubviews.push_front( view );
			}

			return *this;
		}

		View& View::moveSubviewBackward( ViewRef view )
		{
			auto viewIter = std::find( mSubviews.begin(), mSubviews.end(), view );

			if( viewIter != mSubviews.end() && *viewIter != mSubviews.front() ) {
				std::iter_swap( viewIter, --viewIter );
			}

			return *this;
		}

		void View::setSuperviewAndScene( ViewRef view )
		{
			if( view == nullptr ) {
				return;
			}

			//	See if the View is already a subview of another View.
			if( view->getSuperview() != nullptr ) {
				view->removeFromSuperview();
			}

			//	Assign ourselves as the superview
			view->setSuperview( shared_from_this() );
			view->setScene( mScene.lock() );
		}


		//------------------------------------
		//  Dimensions
		//------------------------------------
		void View::setSize( ci::vec2 size )
		{
			mUseElasticBounds = false;

			if( size != getSize() ) {
				mBounds = ci::Rectf( 0, 0, size.x, size.y );
				calculateOffset();
				mNeedsLayout = true;
			}
		}

		ci::Rectf View::getBounds()
		{
			if( mUseElasticBounds ) {
				// Reset Bounds
				ci::Rectf bounds = ci::Rectf( 0, 0, 0, 0 );

				// Get bounds from subviews
				for( ViewRef& subview : mSubviews ) {
					if( subview->mVisible && !subview->getSuperviewShouldIgnoreInBounds() ) {
						bounds.include( subview->getFrame() );
					}
				}

				return bounds;
			}
			else {
				return mBounds;
			}
		}

		void View::drawBounds()
		{
			ci::gl::ScopedColor color( ( ci::Color( mBoundsColor ) ) );

			//	Draw bounding box
			ci::gl::drawStrokedRect( getBounds() );

			//	Draw origin
			ci::gl::pushModelView();
			ci::gl::translate( -mOffset );
			ci::gl::scale( ci::vec2( 1.f, 1.f ) / mScale );
			ci::gl::drawSolidRect( ci::Rectf( -ORIGIN_SIZE / 2.0f, -ORIGIN_SIZE / 2.0f, ( float )ORIGIN_SIZE, ( float )ORIGIN_SIZE ) );
			ci::gl::popModelView();
		}

		ci::Rectf View::getFrame()
		{
			//        if(bFrameDirty) {
			ci::Rectf r = getBounds();

			ci::mat3 m;
			m = glm::translate( m, mPosition );
			m = glm::rotate( m, getRotation() );
			m = glm::scale( m, mScale );
			m = glm::translate( m, mOffset );

			mFrame = r.transformed( m );
			mFrameDirty = false;
			//        }
			return mFrame;
		}


		//------------------------------------
		//  Events
		//------------------------------------

		//
		//  General
		//

		bool View::isEligibleForInteractionEvents()
		{
			if( !hasScene() ||
			        !isInteractionEnabled() ||
			        !isVisible() ||
			        mScale.x == 0.0f ||
			        mScale.y == 0.0f ) {
				return false;
			}

			return true;
		}
	}
} //  namespace po::scene
