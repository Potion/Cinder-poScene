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
#include "cinder/CinderMath.h"
#include "cinder/Timeline.h"

#include "poMatrixSet.h"
#include "poEventCenter.h"

using namespace boost;
using namespace signals2;

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
    
    //Forward declare Scene + NodeContainer
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    //Create NodeRef typedef
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //Signals
    typedef boost::signals2::signal<void(po::MouseEvent&)> MouseEventSignal;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
        friend class Scene;
        friend class NodeContainer;
        friend class EventCenter;
        
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
        
        //Bounds & Frame
        void setDrawBoundsEnabled(bool enabled) { mDrawBounds = enabled; };
        virtual ci::Rectf getBounds();
        
        ci::Rectf getFrame();
        
        //Interaction
        void setInteractionEnabled(bool enabled) { bInteractionEnabled = enabled; };
        bool isInteractionEnabled() { return bInteractionEnabled; };
        
        //Hit Testing & Transformation
        virtual bool pointInside(const ci::Vec2f  &point);
        ci::Vec2f sceneToLocal(ci::Vec2f    point);
        ci::Vec2f globalToLocal(ci::Vec2f   point);
        
        //Visibility
        void setVisibilityEnabled(bool enabled) { bVisible = enabled; };
        #pragma message "This should probably step up to see if any of it's parents aren't visible"
        bool isVisible() { return bVisible; };
        
        //------------------
        //ATTRIBUTES
        Node& name(std::string name)    { setName(name); return *this; }
        void setName(std::string name)  { mName = name; };
        
        std::string getName() { return mName; }
        
        //Position
        Node& position(float x, float y)        { setPosition(x,y); return *this; }
        Node& position(ci::Vec2f position)      { return this->position(position.x, position.y); }
        void setPosition(ci::Vec2f position)    { setPosition(position.x, position.y); };
        void setPosition(float x, float y);
        
        ci::Vec2f getPosition()                 { return mPosition; };
        ci::Anim<ci::Vec2f>& getPositionAnim()  { return mPositionAnim; };
        
        //Scale
        Node& scale(float x, float y)   { setScale(x,y); return *this; }
        Node& scale(ci::Vec2f scale)    { return this->scale(scale.x, scale.y); }
        void setScale(ci::Vec2f scale)  { setPosition(scale.x, scale.y); };
        void setScale(float x, float y);
        
        ci::Vec2f getScale()                { return mScale; };
        ci::Anim<ci::Vec2f>& getScaleAnim() { return mScaleAnim; };
        
        //Rotation
        Node& rotation(float rotation)  { setRotation(rotation); return *this; }
        void setRotation(float rotation);
        
        float getRotation()                 { return mRotation; };
        ci::Anim<float>& getRotationAnim()  { return mRotationAnim; };
        
        //Alpha
        Node& alpha(float alpha) { setAlpha(alpha); return *this; }
        void setAlpha(float alpha);
        
        float getAlpha()                { return mAlpha; };
        ci::Anim<float>& getAlphaAnim() { return mAlphaAnim; };
        
        //Offset
        Node& offset(float x, float y)      { setOffset(x,y); return *this; }
        Node& offset(ci::Vec2f offset)      { return this->offset(offset.x, offset.y); }
        void setOffset(ci::Vec2f offset)    { setOffset(offset.x, offset.y); };
        void setOffset(float x, float y);
        
        ci::Vec2f getOffset() { return mOffset; };
        ci::Anim<ci::Vec2f>& getOffsetAnim() { return mOffsetAnim; };
        
        //Alignment
        Node& alignment(Alignment alignment) { setAlignment(alignment); return *this; }
        void setAlignment(Alignment alignment);
        
        Alignment getAlignment() { return mAlignment; };
        
        //Identifiers (Assigned from Scene)
        uint getDrawOrder() { return mDrawOrder; };
        uint getUID() { return mUid; };
        
        
        
        //------------------
        //SIGNALS
        #pragma mark - Signals -
        
        //Mouse Down Inside
        template<typename T, typename Y>
        connection connectMouseDownInside( T fn, Y *inst ) { return mSignalMouseDownInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseDownInside(Node* listener);
        void disconnectMouseDownInside(Node* listener);
        void emitMouseDownInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseDownInside() { return mSignalMouseDownInside; };
        
        //Mouse Move Inside
        #pragma mark -
        template<typename T, typename Y>
        connection connectMouseMoveInside( T fn, Y *inst ) { return mSignalMouseMoveInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseMoveInside(Node* listener);
        void disconnectMouseMoveInside(Node* listener);
        void emitMouseMoveInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseMoveInside() { return mSignalMouseMoveInside; };
        
        //Mouse Drag Inside
        #pragma mark -
        template<typename T, typename Y>
        connection connectMouseDragInside( T fn, Y *inst ) { return mSignalMouseDragInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseDragInside(Node* listener);
        void disconnectMouseDragInside(Node* listener);
        void emitMouseDragInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseDragInside() { return mSignalMouseDragInside; };
        
        //Mouse Up Inside
        #pragma mark -
        template<typename T, typename Y>
        connection connectMouseUpInside( T fn, Y *inst ) { return mSignalMouseUpInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseUpInside(Node* listener);
        void disconnectMouseUpInside(Node* listener);
        void emitMouseUpInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseUpInside() { return mSignalMouseUpInside; };
    
        //------------------
        //Events
        #pragma mark - Events -
        
        //Override these methods to receive events
        //Global events, these fire for all Nodes
        virtual void mouseDown(po::MouseEvent &event)        {};
        virtual void mouseMove(po::MouseEvent &event)        {};
        virtual void mouseDrag(po::MouseEvent &event)        {};
        virtual void mouseUp(po::MouseEvent &event)          {};
        virtual void mouseWheel( po::MouseEvent &event)      {};
        
        virtual void keyDown(po::KeyEvent &event)            {};
        virtual void keyUp(po::KeyEvent &event)              {};
        
        //These are po::Scene events, you need to subscribe to them
        #pragma mark -
        virtual void mouseDownInside(po::MouseEvent &event)  {};
        virtual void mouseMoveInside(po::MouseEvent &event)  {};
        virtual void mouseDragInside(po::MouseEvent &event)  {};
        virtual void mouseEnter(po::MouseEvent &event)       {};
        virtual void mouseLeave(po::MouseEvent &event)       {};
        virtual void mouseOver(po::MouseEvent &event)        {};
        virtual void mouseUpInside(po::MouseEvent &event)    {};
        
        

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
        bool bVisible;
        
        //Bounds/Frame
        ci::Rectf mBounds;
        ci::Rectf mFrame;
        bool mBoundsDirty, mFrameDirty;
        
        //Interaction
        bool bInteractionEnabled;
        
    private:
        //Private attributes
        ci::Vec2f mPosition;
        ci::Vec2f mScale;
        float     mRotation;
        ci::Vec2f mOffset;
        
        float mAlpha;
        
        //Animation
        void initAttrAnimations();
        void updateAttributeAnimations();
        
        ci::Anim<ci::Vec2f> mPositionAnim;
        ci::Anim<ci::Vec2f> mScaleAnim;
        ci::Anim<float>     mRotationAnim;
        ci::Anim<float>     mAlphaAnim;
        ci::Anim<ci::Vec2f> mOffsetAnim;
        
        bool mUpdatePositionFromAnim;
        bool mUpdateScaleFromAnim;
        bool mUpdateRotationFromAnim;
        bool mUpdateOffsetFromAnim;
        bool mUpdateAlphaFromAnim;
        
        //Alignment
        Alignment mAlignment;
        
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        //Transformation Matrix
        po::MatrixSet mMatrix;
        
        //Scene this node belongs to
        std::weak_ptr<Scene> mScene;
        
        //Parent
        std::weak_ptr<NodeContainer> mParent;
        
        //Bounds and frame
        void drawBounds();
        bool mDrawBounds;
        
        //Unique identifiers
        uint mDrawOrder;
        uint mUid;
        
        //Name (optional, helps identify nodes when debugging)
        std::string mName;
        
        
        #pragma mark - Events -
        //EVENTS
        
        //Mouse Events
        //Global events (just send route it to this)
        virtual void notifyGlobal(po::MouseEvent &event);
        
        //Signals
        MouseEventSignal mSignalMouseDownInside, mSignalMouseMoveInside, mSignalMouseDragInside, mSignalMouseUpInside;
        
        //Key Events
        virtual void notifyGlobal(po::KeyEvent &event);
        
        void trackConnection(MouseEvent::Type type, Node *listener, scoped_connection *connection);
        void disconnect(MouseEvent::Type type, Node *listener);
        std::map<po::MouseEvent::Type, std::map<po::Node*, std::unique_ptr<scoped_connection> > > mouseConnections;
        
        bool hasConnection(po::MouseEvent::Type type);
        virtual void emitEvent(po::MouseEvent &event);
        
    };
}