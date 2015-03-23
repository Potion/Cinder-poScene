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
    {
        friend class po::Scene;
    public:
        static NodeContainerRef create(std::string name="");
        
        ~NodeContainer();
        
        //  Children
        static const int    INVALID_INDEX = -1;
        
		int getNumChildren() { return mChildren.size(); };
        void addChild(NodeRef node);
        void addChildren(std::vector<NodeRef> nodes);
        void addChildAt(int index, NodeRef node);
        void addChildBefore(NodeRef before, NodeRef node);
        void addChildAfter(NodeRef after, NodeRef node);
        
        
        std::deque<NodeRef> getChildren();
        std::deque<NodeRef>& getChildrenByReference();
        bool hasChildren();
        
        int getChildIndex(const NodeRef& child);
        NodeRef getChildByIndex(int index);
        NodeRef getChildByUID(uint32_t uid);
        NodeRef getChildByName(const std::string &name);
        NodeRef getFirstChild();
        NodeRef getLastChild();
        
        void removeChild(NodeRef node);
        void removeChildAt(int index);
        void removeAllChildren();
        
        void moveChildToFront(NodeRef node);
        void moveChildForward( NodeRef node);
        void moveChildAfter(NodeRef after, NodeRef node) { addChildAfter(after, node); };
        void moveChildToBack(NodeRef node);
        void moveChildBackward(NodeRef node);
        void moveChildBefore(NodeRef before, NodeRef node) { addChildBefore(before, node); };
        
        //  Bounds
        virtual ci::Rectf getBounds();

        //  Interaction
        virtual bool pointInside(const ci::Vec2f &point, bool localize = true);
        
    protected:
        virtual void draw();
        
        NodeContainer();
        NodeContainer(std::string name);
        
        void drawFbo();
        
        void setScene(SceneRef scene);
        void removeScene();
        
    private:
        //  Set the parent to this container and the scene to this container's scene
        void setParentAndScene(NodeRef node);
        
        //  Update and Draw trees, traverse child nodes
        virtual void updateTree();
        void matrixTree();
        
        //  Children
        std::deque<NodeRef> mChildren;
        
        //  Matrices
        void calculateMatrices();
    };
    
    
    
    //
    //  Exceptions
    //
    
    class NodeContainerException
    : public ci::Exception
    {
    };
    
    class NodeContainerInvalidChildException
    : public NodeContainerException
    {
    };
};