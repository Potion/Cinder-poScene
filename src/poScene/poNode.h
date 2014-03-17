//
//  poNode.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#pragma once

namespace po {
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Node
    : public std::enable_shared_from_this<Node>
    {
    public:
        static NodeRef create();
        ~Node();
        
        void setParent(NodeRef node);
        NodeRef getParent();
        
        
        NodeRef addChild(NodeRef node);
        bool removeChild(NodeRef node);
        
    private:
        NodeRef parent;
        std::vector<NodeRef> children;
    };
}