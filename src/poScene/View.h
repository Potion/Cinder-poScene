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
#include "cinder/CinderMath.h"
#include "cinder/Timeline.h"
#include "cinder/Exception.h"
#include "cinder/Signals.h"

#include "poScene/MatrixSet.h"
#include "poScene/Events.h"
#include "poScene/ViewEvents.h"

namespace po { namespace scene {
    
    // Views are the base items in a scene. They are architected to have standardized
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
	
	inline std::ostream &operator<< (std::ostream &os, Alignment a)  {
		switch (a) {
			case Alignment::TOP_LEFT: os << "TOP_LEFT"; break;
			case Alignment::TOP_CENTER: os << "TOP_CENTER"; break;
			case Alignment::TOP_RIGHT: os << "TOP_RIGHT"; break;
			case Alignment::CENTER_LEFT: os << "CENTER_LEFT"; break;
			case Alignment::CENTER_CENTER: os << "CENTER_CENTER"; break;
			case Alignment::CENTER_RIGHT: os << "CENTER_RIGHT"; break;
			case Alignment::BOTTOM_LEFT: os << "BOTTOM_LEFT"; break;
			case Alignment::BOTTOM_CENTER: os << "BOTTOM_CENTER"; break;
			case Alignment::BOTTOM_RIGHT: os << "BOTTOM_RIGHT"; break;
			case Alignment::NONE: os << "NONE"; break;
			default: os << "UNKNOWN"; break;
		}
		return os;
	}
    
    enum class MatrixOrder {
        TRS, //	standard order
        RST //	orbit order
    };
	
	inline std::ostream &operator<< (std::ostream &os, MatrixOrder m)  {
		switch (m) {
			case MatrixOrder::TRS: os << "TRS"; break;
			case MatrixOrder::RST: os << "RST"; break;
			default: os << "UNKNOWN"; break;
		}
		return os;
	}
    
    //	Forward declare refs
    typedef std::shared_ptr<class Scene> SceneRef;
    typedef std::shared_ptr<class ShapeView> ShapeViewRef;
    typedef std::shared_ptr<class EventCenter> EventCenterRef;

	typedef std::shared_ptr<class View> ViewRef;
    
    //	Signals
    typedef ci::signals::Signal<void(MouseEvent&)> MouseEventSignal;
    typedef ci::signals::Signal<void(TouchEvent&)> TouchEventSignal;
    
    class View
    : public std::enable_shared_from_this<View>
    {
        friend class Scene;
        friend class EventCenter;

    public:
		//! Create a ViewContainer
		static ViewRef create(std::string name = "");
        ~View();
		
		//! Setup function, used to initialize View
        /**	Use this to do non-initializing construction of your object, add children, add events, etc.
            Since we're using shared_ptr's the constructor is a bit worthless, we can't call shared_from_this() or get a shared pointer to "this". **/
        virtual void setup() {};
		
        //! Called automatically once per scene update for every View in the scene
        /** Used for any calculations, state changes before drawing **/
        virtual void update() {};
        //! Called automatically once per scene draw, do any OpenGL here
        /** All matrix transformations take place before this call, so you are automatically in local space
            when this function begins **/
		virtual void draw();
		
		
        //------------------------------------
        //	Scene graph
		//------------------------------------
		
        //	Scene & Parent
        //  Views need a scene and a parent to draw.
        //  Any View that has a scene should have a parent,
        //  but if the View has a parent (any level up) that does not have a scene
        //  it will not have a scene.
        
        //! Get the scene this View currently belongs to (if any)
        SceneRef getScene();
        //! Check if this View currently belongs to a scene
        bool hasScene();
        //! Get the parent of this View (if any)
        ViewRef getParent() const;
        //! Check if this View currently has a parent
        bool hasParent();
		
		//  Differentiate between non-rendering Views and all other Views
		//   this gets overridden to return false in View
		virtual bool isRenderable (void) const { return true; }
        
        //	Dimensions
        //  Dimensions are determined by the getBounds() function
        //  Override this function for custom bounds, i.e. invisible hit areas
        //  or a fixed size
        
        //! Get the width + height
        ci::vec2 getSize() { return getBounds().getSize(); }
        //! Get the absolute (unscaled) width of the View
        float getWidth() { return getBounds().getWidth(); };
        //! Get the absolute (unscaled) height of the View
        float getHeight() { return getBounds().getHeight(); };
        //! Get the size with scaling applied
        ci::vec2 getScaledSize() { return getSize() * getScale(); }
        //! Get the width with scaling applied
        float getScaledWidth() { return getWidth() * getScale().x; };
        //! Get the height with scaling applied
        float getScaledHeight() { return getHeight() * getScale().y; };

		// Children
		static const int INVALID_INDEX = -1;

		//! Add a View
		View &addChild(ViewRef View);
		//! Add multiple children to this View
		/** This method should be preferred when adding a large amount of children at the same time. 
		The View container needs to recalculate it's matrices every time we add a child (to update bounds)
		so using this only causes that to happen once vs n times**/
		View &addChildren(std::vector<ViewRef> Views);
		//! Add a child at an index
		View &addChildAt(int index, ViewRef View);
		//! Add a child before (below) another View
		View &addChildBefore(ViewRef before, ViewRef View);
		//! Add a child after (above) another View
		View &addChildAfter(ViewRef after, ViewRef View);

		// Get Children

		//! Get the total number of children for this View
		size_t getNumChildren() { return mChildren.size(); };
		//! Get all this View's children
		std::deque<ViewRef> getChildren();
		//! Get a reference to all of this View's children
		std::deque<ViewRef> &getChildrenByReference();
		//! Find if the View has any children
		bool hasChildren();
		//! See if we have a child.
		bool hasChild(ViewRef view);
		//! Get a child at an index. Returns INVALID_INDEX if not found
		int getChildIndex(const ViewRef &child);
		//! Get a child by an index. Returns null_ptr if not found.
		ViewRef getChildByIndex(int index);
		//! Get a child by it's UID. Returns null_ptr if not found.
		ViewRef getChildByUID(uint32_t uid);
		//! Get a child by name. Returns null_ptr if not found.
		ViewRef getChildByName(const std::string &name);
		//! Get the first (bottom) child
		ViewRef getFirstChild();
		//! Get the last (top) child
		ViewRef getLastChild();

		// Remove children

		//! Remove a child by View reference
		ViewRef removeChild(ViewRef View);
		//! Remove a child at a specific index
		ViewRef removeChildAt(int index);
		//! Remove all the children from this View.
		void removeAllChildren();

		// Move Child Views

		//! Move a child to the front (top)
		View &moveChildToFront(ViewRef View);
		//! Move a child forward one index
		View &moveChildForward( ViewRef View);
		//! Move a child to after (in front of) a View
		View &moveChildAfter(ViewRef after, ViewRef View) { return addChildAfter(after, View); };
		//! Move a child to back (bottom)
		View &moveChildToBack(ViewRef View);
		//! Move a child backward one index
		View &moveChildBackward(ViewRef View);
		//! Move a child to before (behind) another View
		View &moveChildBefore(ViewRef before, ViewRef View) { return addChildBefore(before, View); };
        
        //	Bounds & Frame
        //  The bounds of the View are relative to local space around the top left origin.
        //  The frame is relative to the position of the View
        
        //! Draw a frame around the bounds, useful for debugging
        View &setDrawBounds(bool enabled) { mDrawBounds = enabled; return *this; };
        //! Return the bounds
        virtual ci::Rectf getBounds();
        //! Set the color that the bounds should be drawn in
        View &setBoundsColor(ci::Color color) { mBoundsColor = color; return *this; };
        //! Get the current bounds color
        ci::Color getBoundsColor() { return mBoundsColor; };
        //! Get the frame
        ci::Rectf getFrame();
        //! Ignore in the bounds of parent, sometimes useful
        View &setParentShouldIgnoreInBounds(bool enable) { mParentShouldIgnoreInBounds = enable; return *this;}
        //! Get parentShouldIgnoreInBounds
        bool getParentShouldIgnoreInBounds() { return mParentShouldIgnoreInBounds; };
        
        //	Interaction
        //  Flag to enable/disable interaction of objects. If it is disabled
        //  they will not receive events and events will pass through, but they will still draw.
        void setInteractionEnabled(bool enabled) { mInteractionEnabled = enabled; };
        bool isInteractionEnabled() { return mInteractionEnabled; };
        
        //	Hit Testing & Transformation
        //! Main function for determining hit testings
        /** Inheriting classes should override this function in order to do hit testing outside of bounds, i.e. alpha test, ray casting, etc.**/
        virtual bool pointInside(const ci::vec2 &windowPoint);
        
        //! Transform a point from another View's local space into this View's local space
        ci::vec2 ViewToLocal(const ci::vec2 &point, ViewRef View);
        //! Transform a point from this View's local space into another View's local space
        ci::vec2 localToView(const ci::vec2 &point, ViewRef View);
        //! Transform a point from window space to this View's local space
        ci::vec2 windowToLocal(const ci::vec2 &point);
        //! Transform a point from this View's local space to window space
        ci::vec2 localToWindow(const ci::vec2 &point);
        
        // Scene Transforms
        // These are convenience functions that could also be accomplished
        // using getScene()->getRootView()->ViewToLocal(ci::vec2(0,0), shared_from_this), windowToLocal(ci::vec2(0,0), etc.
        
        //! Transform a point from the scene's root View's local space to this View's local space. Return (0,0) if this View does not have a scene
        ci::vec2 sceneToLocal(const ci::vec2 &point);
        //! Transform a point from this View's local space to this View's scene's local space.
        ci::vec2 localToScene(const ci::vec2 &point);
        //! Transform a point from this View's scene's local space to window space
        ci::vec2 sceneToWindow(const ci::vec2 &point);
        //! Transform a point from window space to this View's scene's local space
        ci::vec2 windowToScene(const ci::vec2 &point);
        
        //	Visibility
        //  Views that are not visible are not drawn and ignored for events. To have a View that remains in the scene (i.e. for a hit area)
        //  set the alpha to 0.0 instead
        
        //! Set visibility on/off
        void setVisible(bool enabled) { mVisible = enabled; };
        //! Find out if this View is visible.
        /** This checks not only the View's visibility but also it's parents up the draw tree to make sure it is visible**/
        bool isVisible();
		
		
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
        View &setName(std::string name) { mName = name; return *this; };

        //! Get the name of the View
        std::string getName() const { return mName; }
        
        // Position
        // The position that the origin is at within the parent View
        //! Set the position of the View with a ci::vec2
        View &setPosition(ci::vec2 position) { return setPosition(position.x, position.y); };
        //! Set the position of the View, convenience method
        View &setPosition(float x, float y);
        //! Get the position of the View
        ci::vec2 getPosition() { return mPosition; };
        
        // Scale
        // Scales around the origin of the View
        
		//! Set the scale to the same in x & y
		View &setScale(float scale) { return setScale(scale, scale); };
        //! Set the scale with a ci::vec2
        View &setScale(ci::vec2 scale) { return setScale(scale.x, scale.y); };
        //! Set the scale, convenience method
        View &setScale(float x, float y);
        //! Get the scale
        ci::vec2 getScale() { return mScale; };
        
        // Rotation
        // Rotates around the origin of the View
        // Expressed in radians
        
        //! Set the rotation (in radians)
        View &setRotation(float rotation);
        //! Get the rotation (in radians)
        float getRotation() { return mRotation; };
        
        // Alpha
        // Expressed in range (0.0, 1.0), clamped
        
        //! Set the alpha (0.0, 1.0)
        View &setAlpha(float alpha);
        //! Get the alpha
        float getAlpha() { return mAlpha; };
        
        // Applied Alpha
        // This represents the alpha of the View * all of the alpha's above it
        // in the scene graph. This is the actual expected alpha that will be displayed
        // on screen.
        // Use this method when drawing custom Views
        
        //! Get the applied alpha
        float getAppliedAlpha() { return mAppliedAlpha; }
        
        // Offset
        // The offset of drawing, relative to the origin.
        // This can be set either by using one of the built in alignments,
        // or manually by using one of the below methods
        
        //! Set the offset using a ci::vec2
        View &setOffset(ci::vec2 offset) { return setOffset(offset.x, offset.y); };
        //! Set the offset (convenience method)
        View &setOffset(float x, float y);
        //! Get the offset
        ci::vec2 getOffset() { return mOffset; };
        
        //	Alignment
        //  Alignments set automatic offset based on the bounds of the View
        //  based on the origin. Examples include CENTER_CENTER, TOP_CENTER, etc.
        
        //! Set the alignment
        View &setAlignment(Alignment alignment);
        //! Get the alignment
        Alignment getAlignment() { return mAlignment; };
        
        // Matrix Order
        // Support for different orders of Rotate, Translate and Scale. Default is TRS
        
        //! Set the matrix order
        View &setMatrixOrder(MatrixOrder order) { mMatrixOrder = order; return *this; }
        //! Get the matrix order
        MatrixOrder getMatrixOrder() { return mMatrixOrder; }
        
        // Whole pixel snapping
        // Snap coordinates (position + offset) to whole pixels
        // This does not affect parent View
        
        //! Set the matrix order
        View &setPixelSnapping(bool pixelSnapping) { mPixelSnapping = pixelSnapping; return *this; };
        bool getPixelSnapping() { return mPixelSnapping; }
        
        // Fill
        // This is the color used when drawing the View,
        // in general when creating a View class you should use this color
        // but you can ignore it if doing something custom
        
        //! Set the fill color
		View &setFillColor(ci::ColorA color);
        View &setFillColor(ci::Color color);
        //! Get the fill color
		View &setFillColor(float r, float g, float b, float a) { mFillColor = ci::Color(r, g, b); setAlpha(a); return *this; }
        View &setFillColor(float r, float g, float b) { mFillColor = ci::Color(r, g, b); return *this; }
        //! Enable fill
        View &fillEnabled(bool enabled) { setFillEnabled(enabled); return *this; }
        //! Disable fill
        void setFillEnabled(bool enabled) { mFillEnabled = enabled; };
        //! Get fill enabled
        bool getFillEnabled() { return mFillEnabled; }
        //! Get the fill color
        ci::Color getFillColor() { return mFillColor; }
        
        // Stroke
        // This is the stroke color used by the View.
        // Works similar to fill color, Stroke needs to be supported by inheriting View to have any effect.
        
        //! Set the stroke color with a ci::Color
        View &setStrokeColor(ci::Color color) { mStrokeColor = color; return *this; }
        //! Set the stroke color (convenience method)
        View &setStrokeColor(float r, float g, float b) { mStrokeColor = ci::Color(r, g, b); return *this;}
        //! Enable or disable the stroke
        View &setStrokeEnabled(bool enabled) { mStrokeEnabled = enabled; return *this; };
        //! Get stroke enabled
        bool getStrokeEnabled() { return mStrokeEnabled; }
        //! Get the stroke color
        ci::Color getStrokeColor() { return mStrokeColor; }
        
        // Caching/FBO
        // Views can be cached to an FBO. This is generally useful when applying a shader to a whole View,
        // for example this is how we are doing masking.
        // Note that the size of the FBO is fixed, the update/draw loop remains the same, events will still work
        // To statically cache a View and it's entire draw tree, use the createTexture() function
        // This is useful when the layout is complex but does not update regularly as an optimization, or to map the scene
        // as a texture.
        
  
        //! Create an FBO, draw this View (and it's entire hierarchy if applicable) into it, and return the texture
        ci::gl::TextureRef createTexture();
        
        // Masking
        // Views can be masked to another View
        // The masking View can then be positioned, animated, etc.
        // FBO caching is required for masking, so it is enabled automatically
        
        //! Set the mask
        void setMask(ShapeViewRef mask);
        //! Remove the mask
        ViewRef removeMask();
        //! Find if we have a mask or not
        bool hasMask() { if (!mMask) return false; return true; };
        //! Get the mask object
        ViewRef getMask() { return mMask; };
        
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
        ci::Anim<ci::vec2> &getPositionAnim() { return mPositionAnim; };
        //! Get the scale animation
        ci::Anim<ci::vec2> &getScaleAnim() { return mScaleAnim; };
        //! Get the rotation animation
        ci::Anim<float> &getRotationAnim() { return mRotationAnim; };
        //! Get the offset animation
        ci::Anim<ci::vec2> &getOffsetAnim() { return mOffsetAnim; };
        //! Get the fill color animation
        ci::Anim<ci::Color> &getFillColorAnim() { return mFillColorAnim; };
        //! Get the alpha animation
        ci::Anim<float> &getAlphaAnim() { return mAlphaAnim; };
        
        
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
		std::shared_ptr<ViewEventController<EventT, typename EventTypeT, SignalTypeT> > getCorrectEventController() {
			// Try to find an appropriate controller
			for (auto &controller : eventControllers) {
				try {
					// Cast up the controller to the correct type and return
					return std::dynamic_pointer_cast< ViewEventController<EventT, EventTypeT, SignalTypeT> >(controller);
				}
				catch(...) {
					// Keep trying if we can't find one
					continue;
				}
			}

			// Create Controller if it doesn't exist
			eventControllers.push_back(std::shared_ptr<ViewEventController<EventT, EventTypeT, SignalTypeT> >( new ViewEventController<EventT, EventTypeT, SignalTypeT>()));
			return getCorrectEventController<EventT, EventTypeT, SignalTypeT>();
		}

		template<typename EventT, typename EventTypeT, typename SignalTypeT>
		SignalTypeT &getSignalT(EventTypeT type) {
			return getCorrectEventController<EventT, EventTypeT, SignalTypeT>()->getSignal(type);
		}

		// Check to see if we have any connections for this event type
		template<typename EventT, typename EventTypeT, typename SignalTypeT>
		bool isEligibleForInteractionEventT(const EventTypeT &type) {
			return getCorrectEventController<EventT, EventTypeT, SignalTypeT>()->isEligibleForInteractionEvent(type);
		}

		template<typename EventT, typename EventTypeT, typename SignalTypeT>
		void emitEventT(EventT &event) {
			event.setSource(shared_from_this());
			getCorrectEventController<EventT, EventTypeT, SignalTypeT>()->emitEvent(event);
		}

		// Mouse Events
		MouseEventSignal &getSignal(MouseEvent::Type type) { return getSignalT<MouseEvent, MouseEvent::Type, MouseEventSignal>(type); }
		bool isEligibleForInteractionEvent(const MouseEvent::Type &type) { return isEligibleForInteractionEventT<MouseEvent, MouseEvent::Type, MouseEventSignal>(type); };
		void emitEvent(MouseEvent &event) { return emitEventT<MouseEvent, MouseEvent::Type, MouseEventSignal>(event); };

		// Touch Events
        TouchEventSignal &getSignal(TouchEvent::Type type) { return getSignalT<TouchEvent, TouchEvent::Type, TouchEventSignal>(type); }
		bool isEligibleForInteractionEvent(const TouchEvent::Type &type) { return isEligibleForInteractionEventT<TouchEvent, TouchEvent::Type, TouchEventSignal>(type); };
		void emitEvent(TouchEvent &event) { return emitEventT<TouchEvent, TouchEvent::Type, TouchEventSignal>(event); };

		std::vector<ViewEventControllerBaseRef> eventControllers;

    protected:
        // Constructor
        View(std::string name = "");
        
        //! Set the parent for this View
        void setParent(ViewRef View);
        //! Remove the parent for this View
        void removeParent();
        //! Set the scene for this View
        virtual void setScene(SceneRef scene);
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
        bool mBoundsDirty, mFrameDirty;
        
        //	Caching/FBO
        void captureMasked();
        void drawMasked();
        po::scene::ViewRef mMask;
        bool mIsMasked;
        
        
//        //! Generate an FBO to draw into
//        bool createFbo();
//        //! Render our drawTree into the FBO
//        void captureFbo();
//        //! Clear our FBO and reset it (using Cinder hack with depth buffer to avoid memory leak)
//		void resetFbos();
//        
//        //! Draw our FBO
//        virtual void drawFbo();
//        bool mIsCapturingFbo;
//        bool mCacheToFbo;
//        
//        // Masking
//        //! Draw with mask applied to FBO
//        void drawMasked();
//        ShapeRef mMask;
//        bool mIsMasked;

		//	Name (optional, helps identify Views when debugging)
		std::string mName;
		//friend std::ostream& operator<<(std::ostream &os, ViewRef &a);
        
    private:
        // Private attributes
        ci::vec2 mPosition;
        ci::vec2 mScale;
        float mRotation;
        ci::vec2 mOffset;
        ci::Color mFillColor;
        ci::Color mStrokeColor;
        bool mFillEnabled, mStrokeEnabled;
        float mAlpha, mAppliedAlpha;
        MatrixOrder mMatrixOrder;
        
        bool mPixelSnapping;

		// Children
		std::deque<ViewRef> mChildren;

		//  Set a view's parent to this container and the scene to this container's scene
		void View::setChildParentAndScene(ViewRef view);

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
        
        bool  mUpdatePositionFromAnim
            , mUpdateScaleFromAnim
            , mUpdateRotationFromAnim
            , mUpdateOffsetFromAnim
            , mUpdateAlphaFromAnim
            , mUpdateFillColorFromAnim;
        
        // Alignment
        Alignment mAlignment;
        
        // Update and Draw trees
        // These traverse child Views and run
        // Until they hit an endpoint (View with no children)
        
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

        //! Calculate the matrices only
        virtual void calculateMatrices();
        
        //	Transformation Matrix
        MatrixSet mMatrix;
        
        //	Scene this View belongs to
        std::weak_ptr<Scene> mScene;
        bool mHasScene;
        
        //	Parent
        std::weak_ptr<View> mParent;
        bool mHasParent;

		//  Set the parent to this container and the scene to this container's scene
		void setParentAndScene(ViewRef View);
        
        //	Bounds and frame
        //! Draw the bounds/frame
        void drawBounds();
        bool mDrawBounds;
        ci::Color mBoundsColor;
        bool mParentShouldIgnoreInBounds;
        
        //	Unique identifiers
        uint32_t mDrawOrder;
        uint32_t mUid;
        
        //------------------------------------
        //  Interaction Events
        //------------------------------------
		
        //! Determine if this View is visible, has a scene and parent, etc.
        bool isEligibleForInteractionEvents();
		
        
        //------------------------------------
        //  Exceptions
        //------------------------------------
        
        class ViewException
        : public ci::Exception
        {
        };

		class ViewInvalidChildException
			: public ViewException
		{
		};
		
    };

	inline std::ostream &operator<<(std::ostream &os, const po::scene::View &a) {
		os << a.getName();
		return os;
	}

	inline std::ostream &operator<<(std::ostream &os, const po::scene::ViewRef &a) {
		os << *a;
		return os;
	}
	
} } //  namespace po::scene