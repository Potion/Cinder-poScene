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

namespace po {
    //Forward declare Scene + NodeContainer
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    //Forward declare NodeRef typedef
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Node
    {
        friend class Scene;
        friend class NodeContainer;
    public:
        
        static NodeRef create();
        
        ~Node();
        
        //------------------
        //UPDATE/DRAW
        
        virtual void update();
        virtual void draw();
        
        //------------------
        //SCENE GRAPH
        
        //Scene
        SceneRef getScene();
        
        //Parent
        NodeContainerRef getParent() const;
        bool hasParent();
        
        //Dimensions
        float getWidth();
        float getHeight();
        
        //Bounds & Frame
        void setDrawBoundsEnabled(bool enabled);
        virtual ci::Rectf getBounds();
        
        void setDrawFrameEnabled(bool enabled);
        ci::Rectf getFrame();
        
        //------------------
        //ATTRIBUTES
        std::string name;
        ci::Vec2f position;
        ci::Vec2f scale;
        ci::Vec3f rotation;
        
        float alpha;
        ci::Color fillColor;
        ci::Color strokeColor;
        
//        int getDrawOrder();
    protected:
        Node();
        void setParent(NodeContainerRef node);
        
        //Tranformation
        void setTransformation();
        
    private:
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        //Scene this node belongs to
        SceneRef scene;
        
        //Bounds and frame
        void drawBounds();
        bool bDrawBounds;
        
        void drawFrame();
        bool bDrawFrame;
        
        //Parent
        void removeParent();
        std::weak_ptr<NodeContainer> parent;
        
        //Visibility
        bool visible;
        
        //Unique identifier
        uint uid;
    };
}