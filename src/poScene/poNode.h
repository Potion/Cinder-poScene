//
//  poNode.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//
#pragma once

#include "cinder/Cinder.h"
#include "cinder/CinderMath.h"

#include "poMatrixSet.h"
#include "poEventCenter.h"

namespace po {
    //Forward declare Scene + NodeContainer
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    //Create NodeRef typedef
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
        friend class Scene;
        friend class NodeContainer;
        friend class EventCenter;
    public:
        
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
        
        void setDrawFrameEnabled(bool enabled) { bDrawFrame = enabled; };
        ci::Rectf getFrame();
        
        //Interaction
        void setInteractionEnabled(bool enabled) { bInteractionEnabled = enabled; };
        bool isInteractionEnabled() { return bInteractionEnabled; };
        
        //Hit Testing & Transformation
        virtual bool pointInside(ci::Vec2f  point);
        ci::Vec2f sceneToLocal(ci::Vec2f    point);
        ci::Vec2f globalToLocal(ci::Vec2f   point);
        
        
        //Visibility
        void setVisibilityEnabled(bool enabled) { bVisible = enabled; };
        #pragma message "This should probably step up to see if any of it's parents aren't visible"
        bool isVisible() { return bVisible; };
        
        //------------------
        //ATTRIBUTES
        std::string name;
        ci::Vec2f position;
        ci::Vec2f scale;
        ci::Vec3f rotation;
        
        float alpha;
        
        uint getDrawOrder() { return drawOrder; };
        uint getUID() { return uid; };
        
        //------------------
        //GLOBAL EVENTS
        //Override these methods to receive events
        //Global events, these fire for all Nodes
        virtual void mouseDown(po::MouseEvent event)        {};
        virtual void mouseMove(po::MouseEvent event)        {};
        virtual void mouseDrag(po::MouseEvent event)        {};
        virtual void mouseUp(po::MouseEvent event)          {};
        virtual void mouseWheel( po::MouseEvent event)      {};
        
        //These are po::Scene events, you need to subscribe to them
        virtual void mouseDownInside(po::MouseEvent event)  {};
        virtual void mouseMoveInside(po::MouseEvent event)  {};
        virtual void mouseDragInside(po::MouseEvent event)  {};
        virtual void mouseEnter(po::MouseEvent event)       {};
        virtual void mouseLeave(po::MouseEvent event)       {};
        virtual void mouseOver(po::MouseEvent event)        {};
        virtual void mouseUpInside(po::MouseEvent event)    {};

    protected:
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
        void addEvent(po::MouseEvent::Type type, NodeRef source);
        void removeEvent(po::MouseEvent::Type type, NodeRef source);
        void removeAllEvents();
        
        void registerEventCallback(po::MouseEvent::Type type, NodeRef listener);
        
    private:
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
        
        void drawFrame();
        bool bDrawFrame;
        
        //Parent
        std::weak_ptr<NodeContainer> parent;
        
        //Unique identifiers
        uint drawOrder;
        uint uid;
        
        //------------------
        //NODE EVENTS
        struct EventCallback {
            EventCallback() : markedForRemoval(false) {};
            
            bool markedForRemoval;
            std::weak_ptr<Node> listener;
        };
        
        bool callbackAlreadyExistsForListener(NodeRef listener, std::vector<EventCallback> &callbackList);
        
        //Mouse Events
        //Global events (just send route it to this)
        virtual void notifyGlobal(po::MouseEvent event);
        
        //Callback events
        bool hasCallbacks(po::MouseEvent::Type type);
        virtual po::MouseEvent notifyCallbacks(po::MouseEvent event);
        
        #pragma message "Is vector the best thing to use here?"
        std::map<po::MouseEvent::Type, std::vector<EventCallback> > mouseEventSubscriptions;
        std::map<po::MouseEvent::Type, std::vector<EventCallback> > mouseEventCallbacks;
    };
}