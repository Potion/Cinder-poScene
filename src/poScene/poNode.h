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
		
		//
        //	Use this to do non-initializing construction of your object, add children, add events, etc.
        //	Since we're using shared_ptr's the constructor is a bit worthless, we can't call shared_from_this() or get a shared pointer to "this"
		//
        virtual void setup() {};
		
        //	These are called automatically when your node is in the scene
        virtual void update() {};
        virtual void draw() = 0;
		
		
        //------------------------------------
        //	Scene graph
			#pragma mark - Scene graph
		//------------------------------------
		
        //	Scene & Parent
        SceneRef getScene();
        bool hasScene();
        NodeContainerRef getParent() const;
        bool hasParent();
        
        //	Dimensions
        ci::Vec2f getSize() { return getBounds().getSize(); }
        float getWidth() { return getBounds().getWidth(); };
        float getHeight() { return getBounds().getHeight(); };
        ci::Vec2f getScaledSize() { return getSize() * getScale(); }
        float getScaledWidth() { return getWidth() * getScale().x; };
        float getScaledHeight() { return getHeight() * getScale().y; };
        
        //	Bounds & Frame
        Node &drawBounds(bool enabled) { setDrawBounds(enabled); return *this; };
        void setDrawBounds(bool enabled) { mDrawBounds = enabled; };
        virtual ci::Rectf getBounds();
        Node &boundsColor(ci::Color color) { setBoundsColor(color); return *this; };
        void setBoundsColor(ci::Color color) { mBoundsColor = color; };
        ci::Color getBoundsColor() { return mBoundsColor; };
        ci::Rectf getFrame();
        
        //	Interaction
        void setInteractionEnabled(bool enabled) { mInteractionEnabled = enabled; };
        bool isInteractionEnabled() { return mInteractionEnabled; };
        
        //	Hit Testing & Transformation
        virtual bool pointInside(const ci::Vec2f &point, bool localize = true);
        
        ci::Vec2f nodeToLocal(const ci::Vec2f &point, NodeRef node);
        ci::Vec2f localToNode(const ci::Vec2f &point, NodeRef node);
        ci::Vec2f sceneToLocal(const ci::Vec2f &point);
        ci::Vec2f localToScene(const ci::Vec2f &point);
        ci::Vec2f sceneToWindow(const ci::Vec2f &point);
        ci::Vec2f windowToScene(const ci::Vec2f &point);
        
        ci::Vec2f windowToLocal(const ci::Vec2f &point);
        ci::Vec2f localToWindow(const ci::Vec2f &point);
        
        //	Visibility
        void setVisible(bool enabled) { mVisible = enabled; };
        bool isVisible();
		
		
        //------------------------------------
        //	Attributes
			#pragma mark - Attributes
		//------------------------------------
		
        Node &name(std::string name) { setName(name); return *this; }
        void setName(std::string name) { mName = name; };
        
        std::string getName() { return mName; }
        
        //	Position
        virtual Node &position(float x, float y) { setPosition(x, y); return *this; }
        virtual Node &position(ci::Vec2f position) { return this->position(position.x, position.y); }
        void setPosition(ci::Vec2f position) { setPosition(position.x, position.y); };
        void setPosition(float x, float y);
        ci::Vec2f getPosition() { return mPosition; };
        
        //	Scale
        Node &scale(float x, float y) { setScale(x, y); return *this; }
        Node &scale(ci::Vec2f scale) { return this->scale(scale.x, scale.y); }
        void setScale(ci::Vec2f scale) { setScale(scale.x, scale.y); };
        void setScale(float x, float y);
        ci::Vec2f getScale() { return mScale; };
        
        //	Rotation
        Node &rotation(float rotation) { setRotation(rotation); return *this; }
        void setRotation(float rotation);
        float getRotation() { return mRotation; };
        
        //	Alpha
        Node &alpha(float alpha) { setAlpha(alpha); return *this; }
        void setAlpha(float alpha);
        float getAlpha() { return mAlpha; };
        
        //	Applied Alpha
        float getAppliedAlpha() { return mAppliedAlpha; }
        
        //Offset
        Node &offset(float x, float y) { setOffset(x, y); return *this; }
        Node &offset(ci::Vec2f offset) { return this->offset(offset.x, offset.y); }
        void setOffset(ci::Vec2f offset) { setOffset(offset.x, offset.y); };
        void setOffset(float x, float y);
        ci::Vec2f getOffset() { return mOffset; };
        
        //	Alignment
        Node &alignment(Alignment alignment) { setAlignment(alignment); return *this; }
        void setAlignment(Alignment alignment);
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
        Node(std::string name ="");
        
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