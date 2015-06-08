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

#include "boost/signals2.hpp"
#include "cinder/Cinder.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/CinderMath.h"
#include "cinder/Timeline.h"
#include "cinder/Exception.h"
#include "poMatrixSet.h"
#include "poEvents.h"

namespace po { namespace scene {
    
    enum class Alignment {
        NONE,
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER_CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    };
    
    enum class MatrixOrder {
        TRS, //	standard order
        RST //	orbit order
    };
    
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
    typedef boost::signals2::signal<void(MouseEvent&)> MouseEventSignal;
    typedef boost::signals2::signal<void(TouchEvent&)> TouchEventSignal;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
        friend class Scene;
        friend class NodeContainer;
        friend class EventCenter;
        friend class ci::gl::Texture;
        
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
        virtual void draw() = 0;
		
		
        //------------------------------------
        //	Scene graph
			#pragma mark - Scene graph
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
        
        //	Dimensions
        //  Dimensions are determined by the getBounds() function
        //  Override this function for custom bounds, i.e. invisible hit areas
        //  or a fixed size
        
        //! Get the width + height
        ci::Vec2f getSize() { return getBounds().getSize(); }
        //! Get the absolute (unscaled) width of the node
        float getWidth() { return getBounds().getWidth(); };
        //! Get the absolute (unscaled) height of the node
        float getHeight() { return getBounds().getHeight(); };
        //! Get the size with scaling applied
        ci::Vec2f getScaledSize() { return getSize() * getScale(); }
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
        Node &setBoundsColor(ci::Color color) { setBoundsColor(color); return *this; };
        //! Get the current bounds color
        ci::Color getBoundsColor() { return mBoundsColor; };
        //! Get the frame
        ci::Rectf getFrame();
        
        //	Interaction
        //  Flag to enable/disable interaction of objects. If it is disabled
        //  they will not receive events and events will pass through, but they will still draw.
        void setInteractionEnabled(bool enabled) { mInteractionEnabled = enabled; };
        bool isInteractionEnabled() { return mInteractionEnabled; };
        
        //	Hit Testing & Transformation
        //! Main function for determining hit testings
        /** Inheriting classes should override this function in order to do hit testing outside of bounds, i.e. alpha test, ray casting, etc.**/
        virtual bool pointInside(const ci::Vec2f &point, bool localize = true);
        
        //! Transform a point from another node's local space into this node's local space
        ci::Vec2f nodeToLocal(const ci::Vec2f &point, NodeRef node);
        //! Transform a point from this node's local space into another node's local space
        ci::Vec2f localToNode(const ci::Vec2f &point, NodeRef node);
        //! Transform a point from window space to this node's local space
        ci::Vec2f windowToLocal(const ci::Vec2f &point);
        //! Transform a point from this node's local space to window space
        ci::Vec2f localToWindow(const ci::Vec2f &point);
        
        // Scene Transforms
        // These are convenience functions that could also be accomplished
        // using getScene()->getRootNode()->nodeToLocal(ci::Vec2f(0,0), shared_from_this), windowToLocal(ci::Vec2f(0,0), etc.
        
        //! Transform a point from the scene's root node's local space to this node's local space. Return (0,0) if this node does not have a scene
        ci::Vec2f sceneToLocal(const ci::Vec2f &point);
        //! Transform a point from this node's local space to this node's scene's local space.
        ci::Vec2f localToScene(const ci::Vec2f &point);
        //! Transform a point from this node's scene's local space to window space
        ci::Vec2f sceneToWindow(const ci::Vec2f &point);
        //! Transform a point from window space to this node's scene's local space
        ci::Vec2f windowToScene(const ci::Vec2f &point);
        
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
			#pragma mark - Attributes
		//------------------------------------
		
        // Names are convenient ways to label nodes
        // They are not unique and it is up to the user to get right
        
        //! Set the name of the node
        Node &setName(std::string name) { mName = name; return *this; };
        //! Get the name of the node
        std::string getName() { return mName; }
        
        // Position
        // The position that the origin is at within the parent node
        //! Set the position of the node with a ci::Vec2f
        Node &setPosition(ci::Vec2f position) { return setPosition(position.x, position.y); };
        //! Set the position of the node, convenience method
        Node &setPosition(float x, float y);
        //! Get the position of the node
        ci::Vec2f getPosition() { return mPosition; };
        
        // Scale
        // Scales around the origin of the node
        
        //! Set the scale with a ci::Vec2f
        Node &setScale(ci::Vec2f scale) { setScale(scale.x, scale.y); };
        //! Set the scale, convenience method
        Node &setScale(float x, float y);
        //! Get the scale
        ci::Vec2f getScale() { return mScale; };
        
        // Rotation
        // Rotates around the origin of the node
        // Expressed in degrees
        
        //! Set the rotation (in degrees)
        Node &setRotation(float rotation);
        //! Get the rotation (in degrees)
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
        
        //! Set the offset using a ci::Vec2f
        Node &setOffset(ci::Vec2f offset) { return setOffset(offset.x, offset.y); };
        //! Set the offset (convenience method)
        Node &setOffset(float x, float y);
        //! Get the offset
        ci::Vec2f getOffset() { return mOffset; };
        
        //	Alignment
        //  Alignments set automatic offset based on the bounds of the node
        //  based on the origin. Examples include CENTER_CENTER, TOP_CENTER, etc.
        
        //! 
        Node &setAlignment(Alignment alignment);
        Alignment getAlignment() { return mAlignment; };
        
        //	Matrix Order
        Node &matrixOrder(MatrixOrder order) { setMatrixOrder(order); return *this; };
        void setMatrixOrder(MatrixOrder order) { mMatrixOrder = order; }
        MatrixOrder getMatrixOrder() { return mMatrixOrder; }
        
        //	Fill
        virtual Node &fillColor(ci::Color color) { setFillColor(color); return *this; }
        virtual Node &fillColor(float r, float g, float b) { setFillColor(r, g, b); return *this; }
        void setFillColor(ci::Color color) { mFillColor = color; }
        void setFillColor(float r, float g, float b) { mFillColor.set(r, g, b); }
        Node &fillEnabled(bool enabled) { setFillEnabled(enabled); return *this; }
        void setFillEnabled(bool enabled) { mFillEnabled = enabled; };
        bool getFillEnabled() { return mFillEnabled; }
        ci::Color getFillColor() { return mFillColor; }
        
        //	Stroke
        Node &strokeColor(ci::Color color) { setStrokeColor(color); return *this; }
        Node &strokeColor(float r, float g, float b) { setStrokeColor(r, g, b); return *this; }
        void setStrokeColor(ci::Color color) { mStrokeColor = color; }
        void setStrokeColor(float r, float g, float b) { mStrokeColor.set(r, g, b); }
        Node &strokeEnabled(bool enabled) { setStrokeEnabled(enabled); return *this; }
        void setStrokeEnabled(bool enabled) { mStrokeEnabled = enabled; };
        bool getStrokeEnabled() { return mStrokeEnabled; }
        ci::Color getStrokeColor() { return mStrokeColor; }
        
        //	Caching and FBO
        Node &cacheToFboEnabled(bool cache, int width = 0, int height = 0) { setCacheToFboEnabled(cache, width, height); return *this; };
        void setCacheToFboEnabled(bool enabled, int width, int height);
        bool getCachToFboEnabled() { return mCacheToFbo; };
        ci::gl::TextureRef createTexture();
        
        //	Masking
        void setMask(ShapeRef mask);
        ShapeRef removeMask(bool andStopCaching = true);
        bool hasMask() { if (!mMask) return false; return true; };
        ShapeRef getMask() { return mMask; };
        
        //	Identifiers (Assigned from Scene)
        uint32_t getDrawOrder() { return mDrawOrder; };
        uint32_t getUID() { return mUid; };
		
        //	Attribute Animation Getters
        ci::Anim<ci::Vec2f> &getPositionAnim() { return mPositionAnim; };
        ci::Anim<ci::Vec2f> &getScaleAnim() { return mScaleAnim; };
        ci::Anim<float> &getRotationAnim() { return mRotationAnim; };
        ci::Anim<float> &getAlphaAnim() { return mAlphaAnim; };
        ci::Anim<ci::Vec2f> &getOffsetAnim() { return mOffsetAnim; };
        ci::Anim<ci::Color> &getFillColorAnim() { return mFillColorAnim; };
        
        
        //------------------------------------
        //	Signals
			#pragma mark - Signals
		//------------------------------------
        
        MouseEventSignal &getSignal(MouseEvent::Type type) { return mMouseEventSignals[type]; }
        TouchEventSignal &getSignal(TouchEvent::Type type) { return mTouchEventSignals[type]; }

    protected:
        Node(std::string name = "");
        
        void setParent(NodeContainerRef node);
        void removeParent();
        virtual void setScene(SceneRef scene);
        virtual void removeScene();
        
        //	Tranformation
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
        bool createFbo(int width, int height);
        void captureFbo();
		void resetFbo();
        virtual void drawFbo();
        bool mIsCapturingFbo;
        bool mCacheToFbo;
        std::shared_ptr<ci::gl::Fbo> mFbo;
        
        //	Masking
        void drawMasked();
        ShapeRef mMask;
        bool mIsMasked;
        
    private:
        //	Private attributes
        ci::Vec2f mPosition;
        ci::Vec2f mScale;
        float     mRotation;
        ci::Vec2f mOffset;
        ci::Color mFillColor;
        ci::Color mStrokeColor;
        bool mFillEnabled, mStrokeEnabled;
        float mAlpha, mAppliedAlpha;
        MatrixOrder mMatrixOrder;
        
        //	Animation
        void initAttrAnimations();
        void updateAttributeAnimations();
        
        ci::Anim<ci::Vec2f> mPositionAnim;
        ci::Anim<ci::Vec2f> mScaleAnim;
        ci::Anim<float> mRotationAnim;
        ci::Anim<float> mAlphaAnim;
        ci::Anim<ci::Vec2f> mOffsetAnim;
        ci::Anim<ci::Color> mFillColorAnim;
        
        bool  mUpdatePositionFromAnim
            , mUpdateScaleFromAnim
            , mUpdateRotationFromAnim
            , mUpdateOffsetFromAnim
            , mUpdateAlphaFromAnim
            , mUpdateFillColorFromAnim;
        
        //	Alignment
        Alignment mAlignment;
        
        //	Update and Draw trees, traverse child nodes
        virtual void updateTree();
        
        virtual void beginDrawTree();
        virtual void drawTree();
        virtual void finishDrawTree();
        
        virtual void matrixTree();
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
        void drawBounds();
        bool mDrawBounds;
        ci::Color mBoundsColor;
        
        //	Unique identifiers
        uint32_t mDrawOrder;
        uint32_t mUid;
        
        //	Name (optional, helps identify nodes when debugging)
        std::string mName;
        
        //------------------------------------
        //  Interaction Events
			#pragma mark - Interaction Events
        //------------------------------------
		
        void disconnectAllSignals();
        bool isEligibleForInteractionEvents();
        
        //	Mouse
        std::map<MouseEvent::Type, MouseEventSignal> mMouseEventSignals;
        bool isEligibleForInteractionEvent(const MouseEvent::Type &type);
        void emitEvent(MouseEvent &event, const MouseEvent::Type &type);
        
        //	Touch
        std::map<TouchEvent::Type, TouchEventSignal> mTouchEventSignals;
        bool isEligibleForInteractionEvent(const TouchEvent::Type &type);
        void emitEvent(TouchEvent &event, const TouchEvent::Type &type);
		
        
        //------------------------------------
        //  Exceptions
			#pragma mark - Exceptions
        //------------------------------------
        
        class NodeException
        : public ci::Exception
        {
        };
		
    };
	
} } //  namespace po::scene