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
    public:
        static NodeRef create();
        
        Node();
        ~Node();
        
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
        
        
        //------------------
        //ATTRIBUTES
        std::string name;
        Vec2f position;
        Vec2f scale;
        float alpha;
        Vec2f rotation;
        Vec2f offset;
        bool visible;
        bool drawBounds;
    
    protected:
        
    private:
        //Scene this node belongs to
        SceneRef scene;
        
        //Parent and child nodes
        void removeParent();
        
        std::weak_ptr<Node> parent;
        std::vector<NodeRef> children;
        
        //Unique identifier
        uint uid;
    };
}