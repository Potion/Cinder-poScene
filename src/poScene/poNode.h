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
        enum Alignment {
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
        
        static NodeRef create();
        ~Node();
        
        //------------------
        //SETUP
        //Use this to do non-initializing construction of your object, add events, etc.
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
        float getWidth();
        float getHeight();
        
        //Bounds & Frame
        void setDrawBoundsEnabled(bool enabled) { bDrawBounds = enabled; };
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
        std::string name;
        
        //Position
        void setPosition(float x, float y);
        void setPosition(ci::Vec2f position) { setPosition(position.x, position.y); };
        ci::Vec2f getPosition() { return position; };
        ci::Anim<ci::Vec2f> positionAnim;
        
        //Scale
        void setScale(float x, float y);
        void setScale(ci::Vec2f scale) { setPosition(scale.x, scale.y); };
        ci::Vec2f getScale() { return scale; };
        ci::Anim<ci::Vec2f> scaleAnim;
        
        //Rotation
        void setRotation(float rotation);
        float getRotation() { return rotation; };
        ci::Anim<float> rotationAnim;
        
        //Alpha
        void setAlpha(float alpha);
        float getAlpha() { return alpha; };
        ci::Anim<float> alphaAnim;
        
        //Offset
        void setOffset(float x, float y);
        void setOffset(ci::Vec2f offset) { setOffset(offset.x, offset.y); };
        ci::Vec2f getOffset() { return offset; };
        ci::Anim<ci::Vec2f> offsetAnim;
        
        //Alignment
        void setAlignment(Alignment alignment);
        Alignment getAlignment() { return alignment; };
        
        
        
        uint getDrawOrder() { return drawOrder; };
        uint getUID() { return uid; };
        
        //------------------
        //SIGNALS
        #pragma mark - Signals -
        
        //Mouse Down Inside
        template<typename T, typename Y>
        connection connectMouseDownInside( T fn, Y *inst ) { return signalMouseDownInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseDownInside(Node* listener);
        void disconnectMouseDownInside(Node* listener);
        void emitMouseDownInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseDownInside() { return signalMouseDownInside; };
        
        //Mouse Move Inside
        #pragma mark -
        template<typename T, typename Y>
        connection connectMouseMoveInside( T fn, Y *inst ) { return signalMouseMoveInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseMoveInside(Node* listener);
        void disconnectMouseMoveInside(Node* listener);
        void emitMouseMoveInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseMoveInside() { return signalMouseMoveInside; };
        
        //Mouse Drag Inside
        #pragma mark -
        template<typename T, typename Y>
        connection connectMouseDragInside( T fn, Y *inst ) { return signalMouseDragInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseDragInside(Node* listener);
        void disconnectMouseDragInside(Node* listener);
        void emitMouseDragInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseDragInside() { return signalMouseDragInside; };
        
        //Mouse Up Inside
        #pragma mark -
        template<typename T, typename Y>
        connection connectMouseUpInside( T fn, Y *inst ) { return signalMouseUpInside.connect(std::bind( fn, inst, std::_1 )); };
        void connectMouseUpInside(Node* listener);
        void disconnectMouseUpInside(Node* listener);
        void emitMouseUpInside(po::MouseEvent &event);
        MouseEventSignal& getSignalMouseUpInside() { return signalMouseUpInside; };
    
        //------------------
        //SIGNALS
        #pragma mark Events
        
        //Override these methods to receive events
        //Global events, these fire for all Nodes
        virtual void mouseDown(po::MouseEvent &event)        {};
        virtual void mouseMove(po::MouseEvent &event)        {};
        virtual void mouseDrag(po::MouseEvent &event)        {};
        virtual void mouseUp(po::MouseEvent &event)          {};
        virtual void mouseWheel( po::MouseEvent &event)      {};
        
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
        Node();
        
        void setParent(NodeContainerRef node);
        void removeParent();
        virtual void setScene(SceneRef scene);
        virtual void removeScene();
        
        //Tranformation
        void setTransformation();
        
        //Visibility
        bool bVisible;
        
        //Interaction
        bool bInteractionEnabled;
        
        //Event registration
        void subscribeToEvent(po::MouseEvent::Type type, NodeRef source = nullptr);
        void unsubscribeFromEvent(po::MouseEvent::Type type, NodeRef source = nullptr);
        void removeAllEvents();
        
        void registerEventCallback(po::MouseEvent::Type type, NodeRef listener);
        
    private:
        //Private attributes
        ci::Vec2f position;
        ci::Vec2f scale;
        float     rotation;
        ci::Vec2f offset;
        
        float alpha;
        
        //Anim update bools
        void initAttrAnimations();
        void updateAttributeAnimations();
        
        bool bUpdatePositionFromAnim;
        bool bUpdateScaleFromAnim;
        bool bUpdateRotationFromAnim;
        bool bUpdateOffsetFromAnim;
        bool bUpdateAlphaFromAnim;
        
        //Alignment
        Alignment alignment;
        
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        //Transformation Matrix
        po::MatrixSet matrix;
        
        //Scene this node belongs to
        std::weak_ptr<Scene> scene;
        
        //Bounds and frame
        void drawBounds();
        bool bDrawBounds;
        
        //Parent
        std::weak_ptr<NodeContainer> parent;
        
        //Unique identifiers
        uint drawOrder;
        uint uid;
        
        
        #pragma mark - Events -
        //EVENTS
        
        //Mouse Events
        //Global events (just send route it to this)
        virtual void notifyGlobal(po::MouseEvent &event);
        
        //Signals
        MouseEventSignal signalMouseDownInside, signalMouseMoveInside, signalMouseDragInside, signalMouseUpInside;
        
        void trackConnection(MouseEvent::Type type, Node *listener, scoped_connection *connection);
        void disconnect(MouseEvent::Type type, Node *listener);
        std::map<po::MouseEvent::Type, std::map<po::Node*, std::unique_ptr<scoped_connection> > > mouseConnections;
        
        bool hasConnection(po::MouseEvent::Type type);
        virtual void emitEvent(po::MouseEvent &event);
        
    };
}