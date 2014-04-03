//
//  poNodeContainer.h
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//
#pragma once

#include "poNode.h"

namespace po {
    ///Create NodeContainerRef typedef
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    class NodeContainer
    : public Node
    //, public std::enable_shared_from_this<NodeContainer>
    {
        friend class Scene;
    public:
        static NodeContainerRef create(std::string name="");
        
        //Children
        static const int    INVALID_INDEX = -1;
        
        int getNumChildren();
        void addChild(NodeRef node);
        void addChildAt(int index, NodeRef node);
        void addChildBefore(NodeRef before, NodeRef node);
        void addChildAfter(NodeRef after, NodeRef node);
        
        
        std::vector<NodeRef> getChildren();
        std::vector<NodeRef>& getChildrenByReference();
        
        int getChildIndex(const NodeRef& child);
        NodeRef getChildByIndex(int index);
        NodeRef getChildByUID(uint uid);
        NodeRef getChildByName(const std::string &name);
        NodeRef getFirstChild();
        NodeRef getLastChild();
        
        bool removeChild(NodeRef node);
        bool removeChildAt(int index);
        void removeAllChildren();
        
        void moveChildToFront(NodeRef& node);
        void moveChildForward(NodeRef& node);
        void moveChildToBack(NodeRef& node);
        void moveChildBackward(NodeRef& node);
        
        //Bounds
        virtual ci::Rectf getBounds();

        //Interaction
        virtual bool pointInside(const ci::Vec2f &point);
        
    protected:
        NodeContainer(std::string name="");
        void setScene(SceneRef scene);
        void removeScene();
        
    private:
        //Set the parent to this container and the scene to this container's scene
        void setParentAndScene(NodeRef node);
        
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        std::vector<NodeRef> mChildren;
    };
};