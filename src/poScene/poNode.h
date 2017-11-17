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

#include "cinder/Cinder.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/CinderMath.h"
#include "cinder/Timeline.h"
#include "cinder/Exception.h"
#include "cinder/Signals.h"

#include "poMatrixSet.h"
#include "poEvents.h"

namespace po { namespace scene {
    
    // Nodes are the base items in a scene. They are architected to have standardized
    // methods for attributes (position, scale, rotation, etc.), Cinder animation variables that automatically
    // apply to attributes when running, bounds, hit-testing, interaction event handling, updating and drawing,
    // rendering to textures, rendering into an FBO, Masking, point-transformation and a number of other
    // features.
    //
    // All items that are part of a scene should inherit from Node in some way.
    // The Node base class is intended to be inherited-only, hence the private constructor.
    
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
    
    //	Forward declare Scene + NodeContainer
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    //	Create NodeRef typedef
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Shape;
    typedef std::shared_ptr<Shape> ShapeRef;
    
    class EventCenter;
    typedef std::shared_ptr<EventCenter> EventCenterRef;
    
    //	Signals
    typedef ci::signals::Signal<void(MouseEvent&)> MouseEventSignal;
    typedef ci::signals::Signal<void(TouchEvent&)> TouchEventSignal;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
        friend class Scene;
        friend class NodeContainer;
        friend class EventCenter;

    public:
        ~Node();
		
		//! Setup function, used to initialize node
        /**	Use this to do non-initializing construction of your object, add children, add events, etc.
            Since we're using shared_ptr's the constructor is a bit worthless, we can't call shared_from_this() or get a shared pointer to "this". **/
        virtual void setup() {};
		
        //! Called automatically once per scene update for every node in the scene
        /** Used for any calculations, state changes before drawing **/
        virtual void update() {};
        //! Called automatically once per scene draw, do any OpenGL here
        /** All matrix transformations take place before this call, so you are automatically in local space
            when this function begins **/
		virtual void draw() {};
		
		
        //------------------------------------
        //	Scene graph
		//------------------------------------
		
        //	Scene & Parent
        //  Nodes need a scene and a parent to draw.
        //  Any node that has a scene should have a parent,
        //  but if the node has a parent (any level up) that does not have a scene
        //  it will not have a scene.
        
        //! Get the scene this node currently belongs to (if any)
        SceneRef getScene();
        //! Check if this node currently belongs to a scene
        bool hasScene();
        //! Get the parent of this node (if any)
        NodeContainerRef getParent() const;
        //! Check if this node currently has a parent
        bool hasParent();
		
		//  Differentiate between non-rendering NodeContainers and all other nodes
		//   this gets overridden to return false in NodeContainer
		virtual bool isRenderable (void) const { return true; }
        
        //	Dimensions
        //  Dimensions are determined by the getBounds() function
        //  Override this function for custom bounds, i.e. invisible hit areas
        //  or a fixed size
        
        //! Get the width + height
        ci::vec2 getSize() { return getBounds().getSize(); }
        //! Get the absolute (unscaled) width of the node
        float getWidth() { return getBounds().getWidth(); };
        //! Get the absolute (unscaled) height of the node
        float getHeight() { return getBounds().getHeight(); };
        //! Get the size with scaling applied
        ci::vec2 getScaledSize() { return getSize() * getScale(); }
        //! Get the width with scaling applied
        float getScaledWidth() { return getWidth() * getScale().x; };
        //! Get the height with scaling applied
        float getScaledHeight() { return getHeight() * getScale().y; };
        
        //	Bounds & Frame
        //  The bounds of the node are relative to local space around the top left origin.
        //  The frame is relative to the position of the node
        
        //! Draw a frame around the bounds, useful for debugging
        Node &setDrawBounds(bool enabled) { mDrawBounds = enabled; return *this; };
        //! Return the bounds
        virtual ci::Rectf getBounds();
        //! Set the color that the bounds should be drawn in
        Node &setBoundsColor(ci::Color color) { mBoundsColor = color; return *this; };
        //! Get the current bounds color
        ci::Color getBoundsColor() { return mBoundsColor; };
        //! Get the frame
        ci::Rectf getFrame();
        //! Ignore in the bounds of parent, sometimes useful
        Node &setParentShouldIgnoreInBounds(bool enable) { mParentShouldIgnoreInBounds = enable; return *this;}
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
        
        //! Transform a point from another node's local space into this node's local space
        ci::vec2 nodeToLocal(const ci::vec2 &point, NodeRef node);
        //! Transform a point from this node's local space into another node's local space
        ci::vec2 localToNode(const ci::vec2 &point, NodeRef node);
        //! Transform a point from window space to this node's local space
        ci::vec2 windowToLocal(const ci::vec2 &point);
        //! Transform a point from this node's local space to window space
        ci::vec2 localToWindow(const ci::vec2 &point);
        
        // Scene Transforms
        // These are convenience functions that could also be accomplished
        // using getScene()->getRootNode()->nodeToLocal(ci::vec2(0,0), shared_from_this), windowToLocal(ci::vec2(0,0), etc.
        
        //! Transform a point from the scene's root node's local space to this node's local space. Return (0,0) if this node does not have a scene
        ci::vec2 sceneToLocal(const ci::vec2 &point);
        //! Transform a point from this node's local space to this node's scene's local space.
        ci::vec2 localToScene(const ci::vec2 &point);
        //! Transform a point from this node's scene's local space to window space
        ci::vec2 sceneToWindow(const ci::vec2 &point);
        //! Transform a point from window space to this node's scene's local space
        ci::vec2 windowToScene(const ci::vec2 &point);
        
        //	Visibility
        //  Nodes that are not visible are not drawn and ignored for events. To have a node that remains in the scene (i.e. for a hit area)
        //  set the alpha to 0.0 instead
        
        //! Set visibility on/off
        void setVisible(bool enabled) { mVisible = enabled; };
        //! Find out if this node is visible.
        /** This checks not only the node's visibility but also it's parents up the draw tree to make sure it is visible**/
        bool isVisible();
		
		
        //------------------------------------
        //	Attributes
        //
        // Attributes of the node can be manipulated and animated by using these methods.
        // They are stored within the node, allowing you to set them once and forget them
        //
        //------------------------------------
		
        // Names are convenient ways to label nodes
        // They are not unique and it is up to the user to get right
        
        //! Set the name of the node
        Node &setName(std::string name) { mName = name; return *this; };

        //! Get the name of the node
        std::string getName() const { return mName; }
        
        // Position
        // The position that the origin is at within the parent node
        //! Set the position of the node with a ci::vec2
        Node &setPosition(ci::vec2 position) { return setPosition(position.x, position.y); };
        //! Set the position of the node, convenience method
        Node &setPosition(float x, float y);
        //! Get the position of the node
        ci::vec2 getPosition() { return mPosition; };
        
        // Scale
        // Scales around the origin of the node
        
		//! Set the scale to the same in x & y
		Node &setScale(float scale) { return setScale(scale, scale); };
        //! Set the scale with a ci::vec2
        Node &setScale(ci::vec2 scale) { return setScale(scale.x, scale.y); };
        //! Set the scale, convenience method
        Node &setScale(float x, float y);
        //! Get the scale
        ci::vec2 getScale() { return mScale; };
        
        // Rotation
        // Rotates around the origin of the node
        // Expressed in radians
        
        //! Set the rotation (in radians)
        Node &setRotation(float rotation);
        //! Get the rotation (in radians)
        float getRotation() { return mRotation; };
        
        // Alpha
        // Expressed in range (0.0, 1.0), clamped
        
        //! Set the alpha (0.0, 1.0)
        Node &setAlpha(float alpha);
        //! Get the alpha
        float getAlpha() { return mAlpha; };
        
        // Applied Alpha
        // This represents the alpha of the node * all of the alpha's above it
        // in the scene graph. This is the actual expected alpha that will be displayed
        // on screen.
        // Use this method when drawing custom nodes
        
        //! Get the applied alpha
        float getAppliedAlpha() { return mAppliedAlpha; }
        
        // Offset
        // The offset of drawing, relative to the origin.
        // This can be set either by using one of the built in alignments,
        // or manually by using one of the below methods
        
        //! Set the offset using a ci::vec2
        Node &setOffset(ci::vec2 offset) { return setOffset(offset.x, offset.y); };
        //! Set the offset (convenience method)
        Node &setOffset(float x, float y);
        //! Get the offset
        ci::vec2 getOffset() { return mOffset; };
        
        //	Alignment
        //  Alignments set automatic offset based on the bounds of the node
        //  based on the origin. Examples include CENTER_CENTER, TOP_CENTER, etc.
        
        //! Set the alignment
        Node &setAlignment(Alignment alignment);
        //! Get the alignment
        Alignment getAlignment() { return mAlignment; };
        
        // Matrix Order
        // Support for different orders of Rotate, Translate and Scale. Default is TRS
        
        //! Set the matrix order
        Node &setMatrixOrder(MatrixOrder order) { mMatrixOrder = order; return *this; }
        //! Get the matrix order
        MatrixOrder getMatrixOrder() { return mMatrixOrder; }
        
        // Whole pixel snapping
        // Snap coordinates (position + offset) to whole pixels
        // This does not affect parent node
        
        //! Set the matrix order
        Node &setPixelSnapping(bool pixelSnapping) { mPixelSnapping = pixelSnapping; return *this; };
        bool getPixelSnapping() { return mPixelSnapping; }
        
        // Fill
        // This is the color used when drawing the node,
        // in general when creating a Node class you should use this color
        // but you can ignore it if doing something custom
        
        //! Set the fill color
		Node &setFillColor(ci::ColorA color);
        Node &setFillColor(ci::Color color);
        //! Get the fill color
		Node &setFillColor(float r, float g, float b, float a) { mFillColor = ci::Color(r, g, b); setAlpha(a); return *this; }
        Node &setFillColor(float r, float g, float b) { mFillColor = ci::Color(r, g, b); return *this; }
        //! Enable fill
        Node &fillEnabled(bool enabled) { setFillEnabled(enabled); return *this; }
        //! Disable fill
        void setFillEnabled(bool enabled) { mFillEnabled = enabled; };
        //! Get fill enabled
        bool getFillEnabled() { return mFillEnabled; }
        //! Get the fill color
        ci::Color getFillColor() { return mFillColor; }
        
        // Stroke
        // This is the stroke color used by the node.
        // Works similar to fill color, Stroke needs to be supported by inheriting node to have any effect.
        
        //! Set the stroke color with a ci::Color
        Node &setStrokeColor(ci::Color color) { mStrokeColor = color; return *this; }
        //! Set the stroke color (convenience method)
        Node &setStrokeColor(float r, float g, float b) { mStrokeColor = ci::Color(r, g, b); return *this;}
        //! Enable or disable the stroke
        Node &setStrokeEnabled(bool enabled) { mStrokeEnabled = enabled; return *this; };
        //! Get stroke enabled
        bool getStrokeEnabled() { return mStrokeEnabled; }
        //! Get the stroke color
        ci::Color getStrokeColor() { return mStrokeColor; }
        
        // Caching/FBO
        // Nodes can be cached to an FBO. This is generally useful when applying a shader to a whole node,
        // for example this is how we are doing masking.
        // Note that the size of the FBO is fixed, the update/draw loop remains the same, events will still work
        // To statically cache a Node and it's entire draw tree, use the createTexture() function
        // This is useful when the layout is complex but does not update regularly as an optimization, or to map the scene
        // as a texture.
        
  
        //! Create an FBO, draw this node (and it's entire hierarchy if applicable) into it, and return the texture
        ci::gl::TextureRef createTexture();
        
        // Masking
        // Nodes can be masked to another node
        // The masking node can then be positioned, animated, etc.
        // FBO caching is required for masking, so it is enabled automatically
        
        //! Set the mask
        void setMask(ShapeRef mask);
        //! Remove the mask
        NodeRef removeMask();
        //! Find if we have a mask or not
        bool hasMask() { if (!mMask) return false; return true; };
        //! Get the mask object
        NodeRef getMask() { return mMask; };
        
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
        // Nodes currently support mouse and touch events.
        // These both support and extend the basic Cinder events
        // to allow for specific hit-testing use cases,
        // i.e. "Mouse Down Inside"
        //
        // Requesting a specific event from an object will create a signal (if no one has requested one yet)
        // and then return it for you to make a connection to any function.
        //
        //------------------------------------
        
        //! Get a Mouse Event Signal
        MouseEventSignal &getSignal(MouseEvent::Type type) { return mMouseEventSignals[type]; }
        //! Get a Touch Event Signal
        TouchEventSignal &getSignal(TouchEvent::Type type) { return mTouchEventSignals[type]; }

    protected:
        // Constructor
        Node(std::string name = "");
        
        //! Set the parent for this node
        void setParent(NodeContainerRef node);
        //! Remove the parent for this node
        void removeParent();
        //! Set the scene for this node
        virtual void setScene(SceneRef scene);
        //! Remove the scene for this node
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
        po::scene::NodeRef mMask;
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

		//	Name (optional, helps identify nodes when debugging)
		std::string mName;
		//friend std::ostream& operator<<(std::ostream &os, NodeRef &a);
        
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
        // These traverse child nodes and run
        // Until they hit an endpoint (Node with no children)
        
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
        virtual void calculateMatrices() {};
        
        //	Transformation Matrix
        MatrixSet mMatrix;
        
        //	Scene this node belongs to
        std::weak_ptr<Scene> mScene;
        bool mHasScene;
        
        //	Parent
        std::weak_ptr<NodeContainer> mParent;
        bool mHasParent;
        
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
		
        //! Determine if this node is visible, has a scene and parent, etc.
        bool isEligibleForInteractionEvents();
        
        //	Mouse
        std::map<MouseEvent::Type, MouseEventSignal> mMouseEventSignals;
        bool isEligibleForInteractionEvent(const MouseEvent::Type &type);
        void emitEvent(MouseEvent &event);
        
        //	Touch
        std::map<TouchEvent::Type, TouchEventSignal> mTouchEventSignals;
        bool isEligibleForInteractionEvent(const TouchEvent::Type &type);
        void emitEvent(TouchEvent &event);
		
        
        //------------------------------------
        //  Exceptions
        //------------------------------------
        
        class NodeException
        : public ci::Exception
        {
        };
		
    };

	inline std::ostream &operator<<(std::ostream &os, const po::scene::Node &a) {
		os << a.getName();
		return os;
	}

	inline std::ostream &operator<<(std::ostream &os, const po::scene::NodeRef &a) {
		os << *a;
		return os;
	}
	
} } //  namespace po::scene