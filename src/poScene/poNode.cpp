//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "poNode.h"

namespace po {
    NodeRef Node::create()
    {
        return std::make_shared<Node>();
    }
    
    Node::~Node() {}
    
    NodeRef Node::getParent() {
        return parent;
    }
    
    void Node::setParent(NodeRef node) {
        parent = node;
    }
    
    NodeRef Node::addChild(NodeRef node)
    {
        //See if the node is already a child of another node.
        if(node->getParent()) {
            node->getParent()->removeChild(node);
        }
        
        //Assign ourselves as the parent
        node->setParent(shared_from_this());
        
        //Track Node
        children.push_back(node);
        
        return node;
    }
    
    bool Node::removeChild(NodeRef node)
    {
        std::vector<NodeRef>::iterator iter = std::find(children.begin(), children.end(), node);
        
        if(iter != children.end()) {
            node->getParent().reset();
            children.erase(iter);
            return true;
        }
        
        return false;
    }
}
