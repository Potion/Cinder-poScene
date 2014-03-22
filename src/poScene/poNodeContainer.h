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
    //Forward declare NodeContainerRef
    class NodeContainer;
    typedef std::shared_ptr<NodeContainer> NodeContainerRef;
    
    class NodeContainer
    : public Node
    , public std::enable_shared_from_this<NodeContainer>
    {
        friend class Scene;
    public:
        static NodeContainerRef create();
        
        //Children
        int getNumChildren();
        void addChild(NodeRef node);
        bool removeChild(NodeRef node);
        
        //Bounds
        virtual ci::Rectf getBounds();
    protected:
        NodeContainer();
        
    private:
        //Update and Draw trees, traverse child nodes
        virtual void updateTree();
        virtual void drawTree();
        
        std::vector<NodeRef> children;
    };
};