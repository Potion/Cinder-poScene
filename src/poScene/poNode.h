//
//  poNode.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//
#pragma once

#include "boost/signals2.hpp"

#include "cinder/Cinder.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/CinderMath.h"
#include "cinder/Timeline.h"

#include "poMatrixSet.h"
#include "poEventCenter.h"

namespace po {
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
        // standard order
        TRS,
        // orbit order
        RST
    };
    
    //Forward declare Scene + NodeContainer
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    //Create NodeRef typedef
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Shape;
    typedef std::shared_ptr<Shape> ShapeRef;
    
    //Signals
    typedef boost::signals2::signal<void(po::MouseEvent&)> MouseEventSignal;
    typedef boost::signals2::signal<void(po::TouchEvent&)> TouchEventSignal;
    typedef boost::signals2::signal<void(po::KeyEvent&)> KeyEventSignal;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
        friend class Scene;
        friend class NodeContainer;
        friend class EventCenter;
        
        friend class ci::gl::Texture;
        
    public:
        static NodeRef create(std::string name="");
        ~Node();
        
        //------------------
        //SETUP
        //Use this to do non-initializing construction of your object, add children, add events, etc.
        //Since we're using shared_ptr's the constructor is a bit worthless, we can't call shared_from_this() or get a shared pointer to "this"
        virtual void setup()    {};
        
        //------------------
        //UPDATE/DRAW
        //These are called automatically when your node is in the scene
        virtual void update()   {};
        virtual void draw()     {};
        
        //------------------
        //SCENE GRAPH
        
        //Scene & Parent
        SceneRef getScene();
        bool hasScene();
        
        NodeContainerRef getParent() const;
        bool hasParent();
        
        //Dimensions
        float getWidth()    { return getBounds().getWidth(); };
        float getHeight()   { return getBounds().getHeight(); };
        
        float getScaledWidth()  { return getWidth() * getScale().x;     };
        float getScaledHeight() { return getHeight() * getScale().y;    };
        
        //Bounds & Frame
        Node& drawBounds(bool enabled)  { setDrawBounds(enabled); return *this; };
        void setDrawBounds(bool enabled) { mDrawBounds = enabled; };
        virtual ci::Rectf getBounds();
        
        Node& boundsColor(ci::Color color) { setBoundsColor(color); return *this; };
        void setBoundsColor(ci::Color color) { mBoundsColor = color; };
        ci::Color getBoundsColor() { return mBoundsColor; };
        
        ci::Rectf getFrame();
        
        //Interaction
        void setInteractionEnabled(bool enabled) { mInteractionEnabled = enabled; };
        bool isInteractionEnabled() { return mInteractionEnabled; };
        
        //Hit Testing & Transformation
        virtual bool pointInside(const ci::Vec2f    &point, bool localize=true);
        ci::Vec2f sceneToLocal(const ci::Vec2f      &point);
        ci::Vec2f globalToLocal(const ci::Vec2f     &point);
        ci::Vec2f localToGlobal(const ci::Vec2f     &point);
        
        //Visibility
        void setVisible(bool enabled) { mVisible = enabled; };
        bool isVisible();
        
        //------------------
        //ATTRIBUTES
        Node& name(std::string name)    { setName(name); return *this; }
        void setName(std::string name)  { mName = name; };
        
        std::string getName() { return mName; }
        
        //Position
        virtual Node& position(float x, float y)        { setPosition(x,y); return *this; }
        virtual Node& position(ci::Vec2f position)      { return this->position(position.x, position.y); }
        void setPosition(ci::Vec2f position)    { setPosition(position.x, position.y); };
        void setPosition(float x, float y);
        
        ci::Vec2f getPosition()                 { return mPosition; };
        
        //Scale
        Node& scale(float x, float y)   { setScale(x,y); return *this; }
        Node& scale(ci::Vec2f scale)    { return this->scale(scale.x, scale.y); }
        void setScale(ci::Vec2f scale)  { setScale(scale.x, scale.y); };
        void setScale(float x, float y);
        
        ci::Vec2f getScale()                { return mScale; };
        
        //Rotation
        Node& rotation(float rotation)  { setRotation(rotation); return *this; }
        void setRotation(float rotation);
        
        float getRotation()                 { return mRotation; };
        
        //Alpha
        Node& alpha(float alpha) { setAlpha(alpha); return *this; }
        void setAlpha(float alpha);
        
        float getAlpha()                { return mAlpha; };
        
        //Applied Alpha
        float getAppliedAlpha() { return mAppliedAlpha; }
        
        //Offset
        Node& offset(float x, float y)      { setOffset(x,y); return *this; }
        Node& offset(ci::Vec2f offset)      { return this->offset(offset.x, offset.y); }
        void setOffset(ci::Vec2f offset)    { setOffset(offset.x, offset.y); };
        void setOffset(float x, float y);
        
        ci::Vec2f getOffset() { return mOffset; };
        
        //Alignment
        Node& alignment(Alignment alignment) { setAlignment(alignment); return *this; }
        void setAlignment(Alignment alignment);
        
        Alignment getAlignment() { return mAlignment; };
        
        //Matrix Order
        Node& matrixOrder(MatrixOrder order) { setMatrixOrder(order); return *this; };
        void setMatrixOrder(MatrixOrder order) { mMatrixOrder=order; }
        
        MatrixOrder getMatrixOrder() { return mMatrixOrder; }
        
        //Fill
        virtual Node& fillColor(ci::Color color)               { setFillColor(color); return *this; }
        virtual Node& fillColor(float r, float g, float b)     { setFillColor(r,g,b); return *this; }
        void setFillColor(ci::Color color)              { mFillColor = color; }
        void setFillColor(float r, float g, float b)    { mFillColor.set(r,g,b); }
        
        Node& fillEnabled(bool enabled)    { setFillEnabled(enabled); return *this; }
        void setFillEnabled(bool enabled)   { mFillEnabled = enabled; };
        
        bool getFillEnabled() { return mFillEnabled; }
        ci::Color getFillColor() { return mFillColor; }
        
        //Stroke
        Node& strokeColor(ci::Color color)               { setStrokeColor(color); return *this; }
        Node& strokeColor(float r, float g, float b)     { setStrokeColor(r,g,b); return *this; }
        void setStrokeColor(ci::Color color)              { mStrokeColor = color; }
        void setStrokeColor(float r, float g, float b)    { mStrokeColor.set(r,g,b); }
        
        Node& strokeEnabled(bool enabled)      { setStrokeEnabled(enabled); return *this; }
        void setStrokeEnabled(bool enabled)     { mStrokeEnabled = enabled; };
        
        bool getStrokeEnabled()     { return mStrokeEnabled; }
        ci::Color getStrokeColor()  { return mStrokeColor; }
        
        //Caching and FBO
        Node& cacheToFboEnabled(bool cache, int width=0, int height=0) { setCacheToFboEnabled(cache, width, height); return *this; };
        void setCacheToFboEnabled(bool enabled, int width=0, int height=0);
        bool getCachToFboEnabled() { return mCacheToFbo; };
        
        ci::gl::TextureRef createTexture();
        
        //Masking
        void setMask(po::ShapeRef mask);
        po::ShapeRef removeMask(bool andStopCaching = true);
        bool hasMask() { if(!mMask) return false; return true; };
        po::ShapeRef getMask() { return mMask; };
        
        //Identifiers (Assigned from Scene)
        uint32_t getDrawOrder() { return mDrawOrder; };
        uint32_t getUID() { return mUid; };
        
        
        //Attribute Animation Getters
        ci::Anim<ci::Vec2f> &getPositionAnim()  { return mPositionAnim; };
        ci::Anim<ci::Vec2f> &getScaleAnim()     { return mScaleAnim; };
        ci::Anim<float>     &getRotationAnim()  { return mRotationAnim; };
        ci::Anim<float>     &getAlphaAnim()     { return mAlphaAnim; };
        ci::Anim<ci::Vec2f> &getOffsetAnim()    { return mOffsetAnim; };
        ci::Anim<ci::Color> &getFillColorAnim() { return mFillColorAnim; };
        
        
        
        //------------------
        //SIGNALS
        #pragma mark - Signals -
        
        //Mouse
        MouseEventSignal& getSignalMouseDownInside()    { return mSignalMouseDownInside; };
        MouseEventSignal& getSignalMouseMoveInside()    { return mSignalMouseMoveInside; };
        MouseEventSignal& getSignalMouseDragInside()    { return mSignalMouseDragInside; };
        MouseEventSignal& getSignalMouseUpInside()      { return mSignalMouseUpInside; };
        
        //Touch
        TouchEventSignal& getSignalTouchesBeganInside()     { return mSignalTouchesBeganInside; };
        TouchEventSignal& getSignalTouchesMovedInside()     { return mSignalTouchesMovedInside; };
        TouchEventSignal& getSignalTouchesEndedInside()     { return mSignalTouchesEndedInside; };
    
        
        //------------------
        //EVENT HANDLERS
        #pragma mark - Events -
        
        //Override these methods to receive Global events
        virtual void mouseDown(po::MouseEvent &event)       {};
        virtual void mouseMove(po::MouseEvent &event)       {};
        virtual void mouseDrag(po::MouseEvent &event)       {};
        virtual void mouseUp(po::MouseEvent &event)         {};
        virtual void mouseWheel( po::MouseEvent &event)     {};
        
        virtual void touchesBegan(po::TouchEvent &event)    {};
        virtual void touchesMoved(po::TouchEvent &event)    {};
        virtual void touchesEnded(po::TouchEvent &event)    {};
        
        virtual void keyDown(po::KeyEvent &event)           {};
        virtual void keyUp(po::KeyEvent &event)             {};

    protected:
        #pragma mark -
        Node(std::string name ="");
        
        void setParent(NodeContainerRef node);
        void removeParent();
        virtual void setScene(SceneRef scene);
        virtual void removeScene();
        
        //Tranformation
        void setTransformation();
        
        //Visibility
        bool mVisible;
        
        //Bounds/Frame
        ci::Rectf mBounds;
        ci::Rectf mFrame;
        bool mBoundsDirty, mFrameDirty;
        
        //Interaction
        bool mInteractionEnabled;
        
        //Caching/FBO
        bool createFbo(int width, int height);
        void captureFbo();
        virtual void drawFbo();
        bool mIsCapturingFbo;
        
        bool mCacheToFbo;
        std::shared_ptr<ci::gl::Fbo> mFbo;
        ci::gl::GlslProg mMaskShader;
        
        //Masking
        void drawMasked();
        po::ShapeRef mMask;
        bool mIsMasked;
        
        //Global Signals
        //Mouse
        MouseEventSignal& getSignalMouseDown()          { return mSignalMouseDown; };
        MouseEventSignal& getSignalMouseMove()          { return mSignalMouseMove; };
        MouseEventSignal& getSignalMouseDrag()          { return mSignalMouseDrag; };
        MouseEventSignal& getSignalMouseUp()            { return mSignalMouseUp; };
        
        //Touch
        TouchEventSignal& getSignalTouchesBegan()       { return mSignalTouchesBegan; };
        TouchEventSignal& getSignalTouchesMoved()       { return mSignalTouchesMoved; };
        TouchEventSignal& getSignalTouchesEnded()       { return mSignalTouchesEnded; };
        
    private:
        //Private attributes
        ci::Vec2f mPosition;
        ci::Vec2f mScale;
        float     mRotation;
        ci::Vec2f mOffset;
        
        ci::Color mFillColor;
        ci::Color mStrokeColor;
        bool mFillEnabled, mStrokeEnabled;
        
        float mAlpha, mAppliedAlpha;
        
        po::MatrixOrder mMatrixOrder;
        
        //Animation
        void initAttrAnimations();
        void updateAttributeAnimations();
        
        ci::Anim<ci::Vec2f> mPositionAnim;
        ci::Anim<ci::Vec2f> mScaleAnim;
        ci::Anim<float>     mRotationAnim;
        ci::Anim<float>     mAlphaAnim;
        ci::Anim<ci::Vec2f> mOffsetAnim;
        ci::Anim<ci::Color> mFillColorAnim;
        
        bool  mUpdatePositionFromAnim
            , mUpdateScaleFromAnim
            , mUpdateRotationFromAnim
            , mUpdateOffsetFromAnim
            , mUpdateAlphaFromAnim
            , mUpdateFillColorFromAnim;
        
        //Alignment
        Alignment mAlignment;
        
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        
        virtual void beginDrawTree();
        virtual void drawTree();
        virtual void finishDrawTree();
        
        virtual void matrixTree();
        
        //Transformation Matrix
        po::MatrixSet mMatrix;
        
        //Scene this node belongs to
        std::weak_ptr<Scene> mScene;
        bool mHasScene;
        
        //Parent
        std::weak_ptr<NodeContainer> mParent;
        bool mHasParent;
        
        //Bounds and frame
        void drawBounds();
        bool mDrawBounds;
        ci::Color mBoundsColor;
        
        //Unique identifiers
        uint32_t mDrawOrder;
        uint32_t mUid;
        
        //Name (optional, helps identify nodes when debugging)
        std::string mName;
        
        //Interaction Events
        //Mouse
        MouseEventSignal mSignalMouseDown, mSignalMouseDownInside,
                            mSignalMouseMove, mSignalMouseMoveInside,
                            mSignalMouseDrag, mSignalMouseDragInside,
                            mSignalMouseUp, mSignalMouseUpInside;
        
        bool hasConnection(const po::MouseEvent::Type &type);
        void emitEvent(po::MouseEvent &event, const po::MouseEvent::Type &type);
        
        //Touch
        TouchEventSignal mSignalTouchesBegan, mSignalTouchesBeganInside,
                            mSignalTouchesMoved, mSignalTouchesMovedInside,
                            mSignalTouchesEnded, mSignalTouchesEndedInside;
        
        bool hasConnection(const po::TouchEvent::Type &type);
        void emitEvent(po::TouchEvent &event, const po::TouchEvent::Type &type);
        
        //Key
        KeyEventSignal mSignalKeyDown, mSignalKeyUp;
        
        bool hasConnection(const po::KeyEvent::Type &type);
        void emitEvent(po::KeyEvent &event, const po::KeyEvent::Type &type);
    };
}