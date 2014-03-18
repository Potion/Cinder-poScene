//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "poNode.h"

namespace po {
    static uint OBJECT_UID = 0;
    
    NodeRef Node::create()
    {
        return std::make_shared<Node>();
    }
    
    Node::Node()
    :   uid(OBJECT_UID++)
    {
    }
    
    Node::~Node() {}
    
    void Node::update()
    {
        
    }
    
    void Node::draw()
    {
    }
    
    
    SceneRef Node::getScene()
    {
        return scene;
    }
    
    NodeRef Node::getParent() const {
        return parent.lock();
    }
    
    void Node::setParent(NodeRef node)
    {
        scene = node->getScene();
        parent = node;
    }
        
    bool Node::hasParent()
    {
        if(parent.lock()) {
            return true;
        }
        
        return false;
    }
    
    void Node::removeParent() {
        parent.reset();
    }
    
    int Node::getNumChildren()
    {
        return children.size();
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
            //Remove reference to this node in child
            node->removeParent();
            
            //Erase node
            children.erase(iter);
            
            return true;
        }
        
        return false;
    }
}
