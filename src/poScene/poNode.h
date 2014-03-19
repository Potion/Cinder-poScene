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

#include "poScene.h"

using namespace ci;

namespace po {
    //Forward declare Scene
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
    friend class Scene;
    public:
        
        static NodeRef create();
        
        Node();
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
        void setParent(NodeRef node);
        NodeRef getParent() const;
        bool hasParent();
        
        //Children
        int getNumChildren();
        NodeRef addChild(NodeRef node);
        bool removeChild(NodeRef node);
        
        //Bounds & Frame
        ci::Rectf getBounds();
        ci::Rectf getFrame();
        
        //------------------
        //ATTRIBUTES
        std::string name;
        Vec2f position;
        Vec3f scale;
        Vec3f rotation;
        Vec2f offset;
        
        float alpha;
        ci::Color fillColor;
        ci::Color strokeColor;
        
        bool visible;
        
        //------------------
        //BOUNDS & FRAME
//        ci::Rectf getBounds();
//        void setDrawBounds(bool bDrawBounds);
//        bool getDrawBounds();
//        
//        ci::Rectf getFrame();
//        void setDrawFrame();
//        bool getDrawFrame();
//        
//        int getDrawOrder();
        
    private:
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        //Scene this node belongs to
        SceneRef scene;
        
        //Translation matrix
        void setMatrix(const ci::Matrix44f &parentMatrix);
        ci::Matrix44f matrix;
        
        //Bounds and frame
        void calculateBounds();
        ci::Rectf bounds, frame;
        bool bDrawBounds, bDrawFrame;
        
        //Parent and child nodes
        void removeParent();
        
        std::weak_ptr<Node> parent;
        std::vector<NodeRef> children;
        
        //Unique identifier
        uint uid;
    };
}