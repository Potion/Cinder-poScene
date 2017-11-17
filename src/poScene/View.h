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

#pragma once

#include <deque>
#include <typeindex>

#include "cinder/Cinder.h"
#include "cinder/app/App.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "cinder/CinderMath.h"
#include "cinder/Timeline.h"
#include "cinder/Exception.h"

#include "poScene/MatrixSet.h"
#include "poScene/Events.h"
#include "poScene/ViewEvents.h"

namespace po
{
	namespace scene
	{
		// Views are the base items in a scene. They are designed to have standardized
		// methods for attributes (position, scale, rotation, etc.), Cinder animation variables that automatically
		// apply to attributes when running, bounds, hit-testing, interaction event handling, updating and drawing,
		// rendering to textures, rendering into an FBO, Masking, point-transformation and a number of other
		// features.
		//
		// All items that are part of a scene should inherit from View in some way.
		// The View base class is intended to be inherited-only, hence the private constructor.

		enum class Alignment {
			TOP_LEFT,
			TOP_CENTER,
			TOP_RIGHT,
			CENTER_LEFT,
			CENTER_CENTER,
			CENTER_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_CENTER,
			BOTTOM_RIGHT,
			NONE
		};

		inline std::ostream& operator<< ( std::ostream& os, Alignment a )
		{
			switch( a ) {
				case Alignment::TOP_LEFT:
					os << "TOP_LEFT";
					break;

				case Alignment::TOP_CENTER:
					os << "TOP_CENTER";
					break;

				case Alignment::TOP_RIGHT:
					os << "TOP_RIGHT";
					break;

				case Alignment::CENTER_LEFT:
					os << "CENTER_LEFT";
					break;

				case Alignment::CENTER_CENTER:
					os << "CENTER_CENTER";
					break;

				case Alignment::CENTER_RIGHT:
					os << "CENTER_RIGHT";
					break;

				case Alignment::BOTTOM_LEFT:
					os << "BOTTOM_LEFT";
					break;

				case Alignment::BOTTOM_CENTER:
					os << "BOTTOM_CENTER";
					break;

				case Alignment::BOTTOM_RIGHT:
					os << "BOTTOM_RIGHT";
					break;

				case Alignment::NONE:
					os << "NONE";
					break;

				default:
					os << "UNKNOWN";
					break;
			}

			return os;
		}

		enum class MatrixOrder {
			TRS, //	standard order
			RST //	orbit order
		};

		inline std::ostream& operator<< ( std::ostream& os, MatrixOrder m )
		{
			switch( m ) {
				case MatrixOrder::TRS:
					os << "TRS";
					break;

				case MatrixOrder::RST:
					os << "RST";
					break;

				default:
					os << "UNKNOWN";
					break;
			}

			return os;
		}

		//	Forward declare refs
		typedef std::shared_ptr<class Scene> SceneRef;
		typedef std::shared_ptr<class ShapeView> ShapeViewRef;
		typedef std::shared_ptr<class EventCenter> EventCenterRef;

		typedef std::shared_ptr<class View> ViewRef;

		typedef ci::signals::Signal<void( ViewRef )> ViewSignal;

		class View
			: public std::enable_shared_from_this<View>
		{
				friend class Scene;
				friend class EventCenter;

			public:
				//! Create a ViewContainer
				static ViewRef create( std::string name = "" );
				static ViewRef create( ci::vec2 size, std::string name = "" );

				~View();

				//! Setup function, used to initialize View
				/**	Use this to do non-initializing construction of your object, add Subviews, add events, etc.
				    Since we're using shared_ptr's the constructor is a bit worthless, we can't call shared_from_this() or get a shared pointer to "this". **/
				virtual void setup() {};

				//! Called automatically once per scene update for every View in the scene
				/** Used for any calculations, state changes before drawing **/
				virtual void update() {};
				//! Called automatically once per scene draw, do any OpenGL here
				/** All matrix transformations take place before this call, so you are automatically in local space
				    when this function begins **/
				virtual void draw();

				virtual void layoutSubviews();
				virtual void setNeedsLayout( bool needsLayout = true ) { mNeedsLayout = needsLayout; }
				virtual void layoutIfNeeded();

				//------------------------------------
				//	Scene graph
				//------------------------------------

				//	Scene & Superview
				//  Views need a scene and a superview to draw.
				//  Any View that has a scene should have a superview,
				//  but if the View has a superview (any level up) that does not have a scene
				//  it will not have a scene.

				//! Get the scene this View currently belongs to (if any)
				virtual SceneRef getScene();
				//! Check if this View currently belongs to a scene
				virtual bool hasScene();
				//! Get the superview of this View (if any)
				virtual ViewRef getSuperview() const;
				//! Check if this View currently has a superview
				virtual bool hasSuperview();

				//  Differentiate between non-rendering Views and all other Views
				//   this gets overridden to return false in View
				virtual bool isRenderable( void ) const { return true; }

				//	Dimensions
				//  Dimensions are determined by the getBounds() function
				//  Override this function for custom bounds, i.e. invisible hit areas
				//  or a fixed size

				//! Set the width and height, switching to manual bounds
				virtual void setSize( ci::vec2 size );
				//! Get the width + height
				virtual ci::vec2 getSize() { return getBounds().getSize(); }
				//! Get the size with a scale applied
				virtual ci::vec2 getSizeForScale( ci::vec2 scale ) { return getSize() * scale; }
				virtual ci::vec2 getSizeForScale( float scale ) { return getSize() * scale; }
				//! Get the absolute (unscaled) width of the View
				virtual float getWidth() { return getBounds().getWidth(); };
				//! Get the absolute (unscaled) height of the View
				virtual float getHeight() { return getBounds().getHeight(); };
				//! Get the size with scaling applied
				virtual ci::vec2 getScaledSize() { return getSize() * getScale(); }
				//! Get the width with scaling applied
				virtual float getScaledWidth() { return getWidth() * getScale().x; };
				//! Get the height with scaling applied
				virtual float getScaledHeight() { return getHeight() * getScale().y; };

				// Subviews
				static const int INVALID_SUBVIEW_INDEX = -1;

				//! Add subviews
				virtual View& addSubview( ViewRef view, bool localize = false );
				//! Add multiple subviews to this View
				/** This method should be preferred when adding a large amount of subviews at the same time.
				The View container needs to recalculate it's matrices every time we add a subview (to update bounds)
				so using this only causes that to happen once vs n times**/
				virtual View& addSubviews( std::vector<ViewRef> views, bool localize = false );
				//! Add a subview at an index
				virtual View& insertSubviewAt( int index, ViewRef view, bool localize = false );
				//! Add a subview before (below) another View
				virtual View& insertSubviewBefore( ViewRef view, ViewRef before, bool localize = false );
				//! Add a subview after (above) another View
				virtual View& insertSubviewAfter( ViewRef view, ViewRef after, bool localize = false );

				// Get Subviews

				//! Get the total number of subviews for this View
				virtual size_t getNumSubviews() { return mSubviews.size(); };
				//! Get all this View's subviews
				virtual const std::deque<ViewRef>& getSubviews();
				//! Find if the View has any subviews
				virtual bool hasSubviews();
				//! See if we have a subview.
				virtual bool hasSubview( ViewRef view );
				//! Get the index of a subview. Returns INVALID_INDEX if not found.
				virtual int getIndexForSubview( const ViewRef& subview );
				//! Get a subview by an index. Returns null_ptr if not found.
				virtual ViewRef getSubviewForIndex( int index );
				//! Get a subview by it's UID. Returns null_ptr if not found.
				virtual ViewRef getSubviewForUID( uint32_t uid );
				//! Get a subview by name. Returns null_ptr if not found.
				virtual ViewRef getSubviewForName( const std::string& name );
				//! Get the first (bottom) subview
				virtual ViewRef getFirstSubview();
				//! Get the last (top) subview
				virtual ViewRef getLastSubview();

				// Remove Subviews

				//! Remove a subview by View reference
				virtual ViewRef removeSubview( ViewRef View );
				//! Remove a subview at a specific index
				virtual ViewRef removeSubviewAtIndex( int index );
				//! Remove all the subviews from this View.
				virtual void removeAllSubviews();

				// Move subviews

				//! Move a subview to the front (top)
				virtual View& moveSubviewToFront( ViewRef View );
				//! Move a subview forward one index
				virtual View& moveSubviewForward( ViewRef View );
				//! Move a subview to after (in front of) a View
				virtual View& moveSubviewAfterSubview( ViewRef after, ViewRef View ) { return insertSubviewAfter( after, View ); };
				//! Move a subview to before (behind) another View
				virtual View& moveSubviewBeforeSubview( ViewRef before, ViewRef View ) { return insertSubviewBefore( before, View ); };
				//! Move a subview to back (bottom)
				virtual View& moveSubviewToBack( ViewRef View );
				//! Move a subview backward one index
				virtual View& moveSubviewBackward( ViewRef View );

				// Remove from superview
				virtual View& removeFromSuperview();

				//	Bounds & Frame
				//  The bounds of the View are relative to local space around the top left origin.
				//  The frame is relative to the position of the View

				//! Draw a frame around the bounds, useful for debugging
				virtual View& setDrawBounds( bool enabled ) { mDrawBounds = enabled; return *this; };
				//! Return the bounds
				virtual ci::Rectf getBounds();
				//! Enable elastic bounds, which gives bounds containing all subviews
				virtual void setUseElasticBounds( bool useElasticBounds = true ) { mUseElasticBounds = useElasticBounds; };
				//! Set the color that the bounds should be drawn in
				virtual View& setBoundsColor( ci::Color color ) { mBoundsColor = color; return *this; };
				//! Get the current bounds color
				virtual ci::Color getBoundsColor() { return mBoundsColor; };
				//! Get the frame
				virtual ci::Rectf getFrame();
				//! Ignore in the bounds of superview, sometimes useful
				virtual View& setSuperviewShouldIgnoreInBounds( bool enable ) { mSuperviewShouldIgnoreInBounds = enable; return *this;}
				//! Get superviewShouldIgnoreInBounds
				virtual bool getSuperviewShouldIgnoreInBounds() { return mSuperviewShouldIgnoreInBounds; };

				//	Interaction
				//  Flag to enable/disable interaction of objects. If it is disabled
				//  they will not receive events and events will pass through, but they will still draw.
				virtual void setInteractionEnabled( bool enabled ) { mInteractionEnabled = enabled; };
				virtual bool isInteractionEnabled() { return mInteractionEnabled; };

				//	Hit Testing & Transformation
				//! Main function for determining hit testings
				/** Inheriting classes should override this function in order to do hit testing outside of bounds, i.e. alpha test, ray casting, etc.**/
				virtual bool pointInside( const ci::vec2& windowPoint );

				//! Transform a point from another View's local space into this View's local space
				virtual ci::vec2 viewToLocal( const ci::vec2& point, ViewRef View );
				//! Transform a point from this View's local space into another View's local space
				virtual ci::vec2 localToView( const ci::vec2& point, ViewRef View );
				//! Transform a point from window space to this View's local space
				virtual ci::vec2 windowToLocal( const ci::vec2& point );
				//! Transform a point from this View's local space to window space
				virtual ci::vec2 localToWindow( const ci::vec2& point );

				// Scene Transforms
				// These are convenience functions that could also be accomplished
				// using getScene()->getRootView()->ViewToLocal(ci::vec2(0,0), shared_from_this), windowToLocal(ci::vec2(0,0), etc.

				//! Transform a point from the scene's root View's local space to this View's local space. Return (0,0) if this View does not have a scene
				virtual ci::vec2 sceneToLocal( const ci::vec2& point );
				//! Transform a point from this View's local space to this View's scene's local space.
				virtual ci::vec2 localToScene( const ci::vec2& point );
				//! Transform a point from this View's scene's local space to window space
				virtual ci::vec2 sceneToWindow( const ci::vec2& point );
				//! Transform a point from window space to this View's scene's local space
				virtual ci::vec2 windowToScene( const ci::vec2& point );

				//	Visibility
				//  Views that are not visible are not drawn and ignored for events. To have a View that remains in the scene (i.e. for a hit area)
				//  set the alpha to 0.0 instead

				//! Set visibility on/off
				virtual void setVisible( bool enabled ) { mVisible = enabled; };
				//! Find out if this View is visible.
				/** This checks not only the View's visibility but also it's superviews up the draw tree to make sure it is visible**/
				virtual bool isVisible();


				//------------------------------------
				//	Attributes
				//
				// Attributes of the View can be manipulated and animated by using these methods.
				// They are stored within the View, allowing you to set them once and forget them
				//
				//------------------------------------

				// Names are convenient ways to label Views
				// They are not unique and it is up to the user to get right

				//! Set the name of the View
				virtual View& setName( std::string name ) { mName = name; return *this; };

				//! Get the name of the View
				virtual std::string getName() const { return mName; }

				//! Set the tag of the view
				virtual View& setTag( unsigned int tag ) { mTag = tag; return *this; }

				//! Get the tag of the view
				virtual int getTag() const { return mTag; }

				// Position
				// The position that the origin is at within the superview View
				//! Set the position of the View with a ci::vec2
				virtual View& setPosition( ci::vec2 position ) { return setPosition( position.x, position.y ); };
				//! Set the position of the View, convenience method
				virtual View& setPosition( float x, float y );
				//! Get the position of the View
				virtual ci::vec2 getPosition() { return mPosition; };

				// Scale
				// Scales around the origin of the View

				//! Set the scale to the same in x & y
				virtual View& setScale( float scale ) { return setScale( scale, scale ); };
				//! Set the scale with a ci::vec2
				virtual View& setScale( ci::vec2 scale ) { return setScale( scale.x, scale.y ); };
				//! Set the scale, convenience method
				virtual View& setScale( float x, float y );
				//! Get the scale
				virtual ci::vec2 getScale() { return mScale; };

				// Rotation
				// Rotates around the origin of the View
				// Expressed in radians

				//! Set the rotation (in radians)
				virtual View& setRotation( float rotation );
				//! Get the rotation (in radians)
				virtual float getRotation() { return mRotation; };

				// Alpha
				// Expressed in range (0.0, 1.0), clamped

				//! Set the alpha (0.0, 1.0)
				virtual View& setAlpha( float alpha );
				//! Get the alpha
				virtual float getAlpha() { return mAlpha; };

				// Applied Alpha
				// This represents the alpha of the View * all of the alpha's above it
				// in the scene graph. This is the actual expected alpha that will be displayed
				// on screen.
				// Use this method when drawing custom Views

				//! Get the applied alpha
				virtual float getAppliedAlpha() { return mAppliedAlpha; }

				// Offset
				// The offset of drawing, relative to the origin.
				// This can be set either by using one of the built in alignments,
				// or manually by using one of the below methods

				//! Set the offset using a ci::vec2
				virtual View& setOffset( ci::vec2 offset ) { return setOffset( offset.x, offset.y ); };
				//! Set the offset (convenience method)
				virtual View& setOffset( float x, float y );
				//! Get the offset
				virtual ci::vec2 getOffset() { return mOffset; };
				//! Forces the offset to be re-calculated if using alignment
				virtual View& calculateOffset();

				//	Alignment
				//  Alignments set automatic offset based on the bounds of the View
				//  based on the origin. Examples include CENTER_CENTER, TOP_CENTER, etc.

				//! Set the alignment
				virtual View& setAlignment( Alignment alignment, bool preservePositioning = false );
				//! Get the alignment
				virtual Alignment getAlignment() { return mAlignment; };

				// Matrix Order
				// Support for different orders of Rotate, Translate and Scale. Default is TRS

				//! Set the matrix order
				virtual View& setMatrixOrder( MatrixOrder order ) { mMatrixOrder = order; return *this; }
				//! Get the matrix order
				virtual MatrixOrder getMatrixOrder() { return mMatrixOrder; }

				//! Calculate the matrices only, use when you need to update position + bounds before a draw
				//! or adding children
				virtual void calculateMatrices( bool calcluateSuperViews = true );

				// Whole pixel snapping
				// Snap coordinates (position + offset) to whole pixels
				// This does not affect superview View

				virtual View& setPixelSnapping( bool pixelSnapping ) { mPixelSnapping = pixelSnapping; return *this; };
				virtual bool getPixelSnapping() { return mPixelSnapping; }

				// Premultiplied Blending
				virtual bool getIsPremultiplied() { return mIsPremultiplied; }
				virtual void setIsPremultiplied( bool isPremultiplied ) { mIsPremultiplied = isPremultiplied; }

				// Background
				// Color the background,based on the bounds of the view.
				// If the alpha is set to 0.0 the draw call does not execute.
				virtual View& setBackgroundColor( ci::ColorA color ) { mBackgroundColor = color; return *this; };
				virtual View& setBackgroundColor( ci::Color color ) { return setBackgroundColor( ci::ColorA( color, 1.0 ) ); };

				// Fill
				// This is the color used when drawing the View,
				// in general when creating a View class you should use this color
				// but you can ignore it if doing something custom

				//! Set the fill color
				virtual View& setFillColor( ci::ColorA color );
				virtual View& setFillColor( ci::Color color );
				virtual View& setFillColor( float r, float g, float b, float a ) { setFillColor( ci::ColorA( r, g, b, a ) ); return *this; }
				virtual View& setFillColor( float r, float g, float b ) { setFillColor( ci::Color( r, g, b ) ); return *this; }
				//! Enable fill
				virtual View& fillEnabled( bool enabled ) { setFillEnabled( enabled ); return *this; }
				//! Disable fill
				virtual void setFillEnabled( bool enabled ) { mFillEnabled = enabled; };
				//! Get fill enabled
				virtual bool getFillEnabled() { return mFillEnabled; }
				//! Get the fill color
				virtual ci::Color getFillColor() { return mFillColor; }

				// Stroke
				// This is the stroke color used by the View.
				// Works similar to fill color, Stroke needs to be supported by inheriting View to have any effect.

				//! Set the stroke color with a ci::Color
				virtual View& setStrokeColor( ci::Color color ) { mStrokeColor = color; return *this; }
				//! Set the stroke color (convenience method)
				virtual View& setStrokeColor( float r, float g, float b ) { mStrokeColor = ci::Color( r, g, b ); return *this;}
				//! Enable or disable the stroke
				virtual View& setStrokeEnabled( bool enabled ) { mStrokeEnabled = enabled; return *this; };
				//! Get stroke enabled
				virtual bool getStrokeEnabled() { return mStrokeEnabled; }
				//! Get the stroke color
				virtual ci::Color getStrokeColor() { return mStrokeColor; }

				// Caching/FBO
				// Views can be cached to an FBO. This is generally useful when applying a shader to a whole View,
				// for example this is how we are doing masking.
				// Note that the size of the FBO is fixed, the update/draw loop remains the same, events will still work
				// To statically cache a View and it's entire draw tree, use the createTexture() function
				// This is useful when the layout is complex but does not update regularly as an optimization, or to map the scene
				// as a texture.


				//! Create an FBO, draw this View (and it's entire hierarchy if applicable) into it, and return the texture
				virtual ci::gl::TextureRef createTexture();

				// Masking
				// Views can be masked to another View
				// The masking View can then be positioned, animated, etc.
				// FBO caching is required for masking, so it is enabled automatically

				//! Set the mask
				virtual void setMask( ShapeViewRef mask );
				//! Remove the mask
				virtual ViewRef removeMask();
				//! Find if we have a mask or not
				virtual bool hasMask() { if( !mMask ) return false; return true; };
				//! Get the mask object
				virtual ViewRef getMask() { return mMask; };

				// Identifiers (Assigned from Scene)
				// Used by the scene to identifier the objects
				// and find their depth in the scene for hit testing

				//! Get the draw order, updated every frame
				uint32_t getDrawOrder() { return mDrawOrder; };
				//! Get the UID for this object
				uint32_t getUID() { return mUid; };

				// Attribute Animation Getters
				// Use these to tie in to the ci::Timeline functions
				// They are updated every frame and automatically cancelled by manually setting any
				// of the underlying values, i.e. setPosition() cancels the position animation

				//! Get the position animation
				ci::Anim<ci::vec2>& getPositionAnim() { return mPositionAnim; };
				//! Get the scale animation
				ci::Anim<ci::vec2>& getScaleAnim() { return mScaleAnim; };
				//! Get the rotation animation
				ci::Anim<float>& getRotationAnim() { return mRotationAnim; };
				//! Get the offset animation
				ci::Anim<ci::vec2>& getOffsetAnim() { return mOffsetAnim; };
				//! Get the fill color animation
				ci::Anim<ci::Color>& getFillColorAnim() { return mFillColorAnim; };
				//! Get the alpha animation
				ci::Anim<float>& getAlphaAnim() { return mAlphaAnim; };


				//------------------------------------
				// Signals
				//
				// Views currently support mouse and touch events.
				// These both support and extend the basic Cinder events
				// to allow for specific hit-testing use cases,
				// i.e. "Mouse Down Inside"
				//
				// Requesting a specific event from an object will create a signal (if no one has requested one yet)
				// and then return it for you to make a connection to any function.
				//
				//------------------------------------

				//! Get an Event Signal

				template<typename EventT, typename EventTypeT, typename SignalTypeT>
				std::shared_ptr<ViewEventController<EventT, EventTypeT, SignalTypeT> > getCorrectEventController()
				{
					// Try to find an appropriate controller
					for( auto& controller : mEventControllers ) {
						std::shared_ptr<ViewEventController<EventT, EventTypeT, SignalTypeT>> castController = std::dynamic_pointer_cast< ViewEventController<EventT, EventTypeT, SignalTypeT> >( controller );

						if( castController != nullptr ) {
							return castController;
						}
					}

					// Create Controller if it doesn't exist
					mEventControllers.push_back( std::shared_ptr<ViewEventController<EventT, EventTypeT, SignalTypeT> >( new ViewEventController<EventT, EventTypeT, SignalTypeT>() ) );
					return getCorrectEventController<EventT, EventTypeT, SignalTypeT>();
				}

				template<typename EventT, typename EventTypeT, typename SignalTypeT>
				SignalTypeT& getSignalT( EventTypeT type )
				{
					return getCorrectEventController<EventT, EventTypeT, SignalTypeT>()->getSignal( type );
				}

				// Check to see if we have any connections for this event type
				template<typename EventT, typename EventTypeT, typename SignalTypeT>
				bool isEligibleForInteractionEventT( const EventTypeT& type )
				{
					return getCorrectEventController<EventT, EventTypeT, SignalTypeT>()->isEligibleForInteractionEvent( type );
				}

				template<typename EventT, typename EventTypeT, typename SignalTypeT>
				void emitEventT( EventT& event )
				{
					event.setSource( shared_from_this() );
					getCorrectEventController<EventT, EventTypeT, SignalTypeT>()->emitEvent( event );
				}

				// Mouse Events
				MouseEventSignal& getSignal( MouseEvent::Type type ) { return getSignalT<MouseEvent, MouseEvent::Type, MouseEventSignal>( type ); }
				bool isEligibleForInteractionEvent( const MouseEvent::Type& type ) { return isEligibleForInteractionEventT<MouseEvent, MouseEvent::Type, MouseEventSignal>( type ); };
				void emitEvent( MouseEvent& event ) { emitEventT<MouseEvent, MouseEvent::Type, MouseEventSignal>( event ); };

				// Touch Events
				TouchEventSignal& getSignal( TouchEvent::Type type ) { return getSignalT<TouchEvent, TouchEvent::Type, TouchEventSignal>( type ); }
				bool isEligibleForInteractionEvent( const TouchEvent::Type& type ) { return isEligibleForInteractionEventT<TouchEvent, TouchEvent::Type, TouchEventSignal>( type ); };
				void emitEvent( TouchEvent& event ) { emitEventT<TouchEvent, TouchEvent::Type, TouchEventSignal>( event ); };

				std::vector<ViewEventControllerBaseRef> mEventControllers;

				//! Determine if this View is visible, has a scene and superview, etc.
				bool isEligibleForInteractionEvents();

				//! View Events
				ViewSignal& getSignalWillAppear() { return mSignalWillAppear; }
				ViewSignal& getSignalWillLayoutSubviews() { return mSignalWillLayoutSubviews; }
				ViewSignal& getSignalDidLayoutSubviews() { return mSignalDidLayoutSubviews; }

			protected:
				// Constructor
				View( std::string name = "" );

				//! Set the superview for this View
				void setSuperview( ViewRef View );
				//! Remove the superview for this View
				void removeSuperview();
				//! Set the scene for this View
				virtual void setScene( SceneRef scene );
				//! Remove the scene for this View
				virtual void removeScene();

				//	Tranformation
				//! Push the transformation matrix based on our attributes
				void setTransformation();

				//	Visibility
				bool mVisible;

				//	Interaction
				bool mInteractionEnabled;

				//	Bounds/Frame
				ci::Rectf mBounds;
				ci::Rectf mFrame;
				bool mUseElasticBounds, mBoundsDirty, mFrameDirty;

				//	Caching/FBO
				void captureMasked();
				void drawMasked();
				po::scene::ViewRef mMask;
				bool mIsMasked;

				//	Name (optional, helps identify Views when debugging)
				std::string mName;

				// Tag (optional, helps identify views and tie them to enums)
				unsigned int mTag;

				//friend std::ostream& operator<<(std::ostream &os, ViewRef &a);

				// Connections storage
				ci::signals::ConnectionList mConnections;

			private:
				// Private attributes
				ci::vec2 mPosition;
				ci::vec2 mScale;
				float mRotation;
				ci::vec2 mOffset;
				ci::ColorA mBackgroundColor;
				ci::Color mFillColor;
				ci::Color mStrokeColor;
				bool mFillEnabled, mStrokeEnabled;
				float mAlpha, mAppliedAlpha;
				MatrixOrder mMatrixOrder;

				bool mPixelSnapping;

				bool mIsPremultiplied;

				// Subviews
				void subviewsChanged();
				std::deque<ViewRef> mSubviews;

				//  Set a view's superview to this container and the scene to this container's scene
				void setSubviewSuperviewAndScene( ViewRef view );

				// Animation
				//! Initialize our attribute animations
				void initAttrAnimations();
				//! Update our attributes based on animation settings
				void updateAttributeAnimations();

				ci::Anim<ci::vec2> mPositionAnim;
				ci::Anim<ci::vec2> mScaleAnim;
				ci::Anim<float> mRotationAnim;
				ci::Anim<float> mAlphaAnim;
				ci::Anim<ci::vec2> mOffsetAnim;
				ci::Anim<ci::Color> mFillColorAnim;

				// Alignment
				Alignment mAlignment;

				// Update and Draw trees
				// These traverse Subviews and run
				// Until they hit an endpoint (View with no Subviews)

				//! Run the update tree
				virtual void updateTree();
				//! Start the draw tree, push transformations and settings, etc.
				virtual void beginDrawTree();
				//! Go through the draw tree
				virtual void drawTree();
				//! Finish drawTree, ending our transformations and restoring to previous state
				virtual void finishDrawTree();

				//! Run through just the matrix tree to update positions and bounds
				virtual void matrixTree();

				//	Transformation Matrix
				MatrixSet mMatrix;

				//	Scene this View belongs to
				std::weak_ptr<Scene> mScene;
				bool mHasScene;

				//	Superview
				std::weak_ptr<View> mSuperview;
				bool mHasSuperview;

				//  Set the superview to this container and the scene to this container's scene
				void setSuperviewAndScene( ViewRef View );

				// Background Drawing
				void drawBackground();
				static ci::gl::BatchRef mBackgroundBatch;

				//	Bounds and frame
				//! Draw the bounds/frame
				void drawBounds();
				bool mDrawBounds;
				ci::Color mBoundsColor;
				bool mSuperviewShouldIgnoreInBounds;

				//	Unique identifiers
				uint32_t mDrawOrder;
				uint32_t mUid;


				// Layout
				bool mNeedsLayout;
				ViewSignal mSignalWillLayoutSubviews;
				ViewSignal mSignalDidLayoutSubviews;
				ViewSignal mSignalWillAppear;


				//------------------------------------
				//  Exceptions
				//------------------------------------

				class ViewException
					: public ci::Exception
				{
				};

				class ViewInvalidSubviewException
					: public ViewException
				{
				};

		};

		inline std::ostream& operator<<( std::ostream& os, const po::scene::View& a )
		{
			os << a.getName();
			return os;
		}

		inline std::ostream& operator<<( std::ostream& os, const po::scene::ViewRef& a )
		{
			os << *a;
			return os;
		}

	}
} //  namespace po::scene
