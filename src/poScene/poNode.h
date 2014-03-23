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
    public:
        
        static NodeRef create();
        
        ~Node();
        
        //------------------
        //UPDATE/DRAW
        
        virtual void update();
        virtual void draw();
        
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
        
        //Visibility
        void setVisibilityEnabled(bool enabled) { bVisible = enabled; };
        
        //------------------
        //ATTRIBUTES
        std::string name;
        ci::Vec2f position;
        ci::Vec2f scale;
        ci::Vec3f rotation;
        
        float alpha;
        ci::Color fillColor;
        ci::Color strokeColor;
        
        //------------------
        //GLOBAL EVENTS
        virtual void mouseDown(po::MouseEvent event) { std::cout << "Mouse Down! " << uid << std::endl; };
        virtual void mouseMove(po::MouseEvent event) { std::cout << "Mouse Move! " << uid << std::endl; }
        virtual void mouseDrag(po::MouseEvent event) { std::cout << "Mouse Drag! " << uid << std::endl; };
        virtual void mouseUp(po::MouseEvent event) { std::cout << "Mouse Up! " << uid << std::endl; };
        virtual void mouseWheel( po::MouseEvent event) { std::cout << "Mouse Wheel! " << uid << std::endl; };
        
        //------------------
        //NODE EVENTS
        
//        int getDrawOrder();
    protected:
        Node();
        
        void setParent(NodeContainerRef node);
        void removeParent();
        virtual void setScene(SceneRef scene);
        virtual void removeScene();
        
        //Tranformation
        void setTransformation();
        
    private:
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        //Transformation Matrix
        ci::Matrix44f matrix;
        
        //Scene this node belongs to
        std::weak_ptr<Scene> scene;
        
        //Bounds and frame
        void drawBounds();
        bool bDrawBounds;
        
        void drawFrame();
        bool bDrawFrame;
        
        //Parent
        std::weak_ptr<NodeContainer> parent;
        
        //Visibility
        bool bVisible;
        
        //Interaction
        bool bInteractionEnabled;
        
        //Unique identifier
        uint uid;
    };
}