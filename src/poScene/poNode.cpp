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
    ,   scale(1.0,1.0,1.0)
    ,   bDrawBounds(false)
    ,   bDrawFrame(false)
    {
    }
    
    Node::~Node() {}
    
    void Node::updateTree()
    {
        update();
        for(NodeRef node : children)
            node->updateTree();
    }
    
    void Node::update() {}
    
    void Node::drawTree()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::pushMatrices();
        ci::gl::translate(position);
        ci::gl::rotate(rotation);
        ci::gl::scale(scale);
        
        draw();
        
        for(NodeRef node : children)
            node->drawTree();
        
        if(bDrawBounds)
            drawBounds();
        
        if(bDrawFrame)
            drawFrame();
        
        ci::gl::popMatrices();
    }
    
    void Node::draw(){
    }
    
    
    //------------------------------------------------------
    #pragma mark - SceneGraph -
    
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
    
    
    //------------------------------------------------------
    #pragma mark  - Dimensions -
    
    ci::Rectf Node::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        
        for(NodeRef child : children)
            bounds.include(child->getFrame());
        
        return bounds;
    }
    
    void Node::setDrawBounds(bool shouldDraw)
    {
        bDrawBounds = shouldDraw;
    }
    
    void Node::drawBounds()
    {
        ci::gl::color(255,0,0);
        ci::gl::drawStrokedRect(getBounds());
    }
    
    ci::Rectf Node::getFrame()
    {
        ci::Rectf frame = getBounds();
        frame.offset(position);
        return frame;
    }
    
    void Node::setDrawFrame(bool shouldDraw)
    {
        bDrawFrame = shouldDraw;
    }
    
    void Node::drawFrame()
    {
        ci::gl::color(255,0,0);
        ci::gl::drawStrokedRect(getBounds());
    }
}
