//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "cinder/CinderMath.h"

#include "poNode.h"
#include "poNodeContainer.h"
#include "poScene.h"

namespace po {
    static uint OBJECT_UID  = 0;
    static const int ORIGIN_SIZE   = 2;
    
    NodeRef Node::create()
    {
        return std::shared_ptr<Node>(new Node());
    }
    
    Node::Node()
    :   uid(OBJECT_UID++)
    ,   position(0.f,0.f)
    ,   scale(1.f,1.f)
    ,   rotation(0)
    ,   offset(0.f,0.f)
    ,   alpha(1.f)
    ,   positionAnim(ci::Vec2f(0.f,0.f))
    ,   scaleAnim(ci::Vec2f(1.f,1.f))
    ,   rotationAnim(0)
    ,   offsetAnim(ci::Vec2f(0.f,0.f))
    ,   alphaAnim(1.f)
    ,   bUpdatePositionFromAnim(false)
    ,   bUpdateScaleFromAnim(false)
    ,   bUpdateRotationFromAnim(false)
    ,   bUpdateOffsetFromAnim(false)
    ,   bUpdateAlphaFromAnim(false)
    ,   bDrawBounds(false)
    ,   bBoundsDirty(true)
    ,   bFrameDirty(true)
    ,   bVisible(true)
    ,   bInteractionEnabled(true)
    {
        //Initialize our animations
        initAttrAnimations();
    }
    
    Node::~Node() {
        
    }
    
    
    
    //------------------------------------------------------
    #pragma mark - Update & Draw Trees -
    
    void Node::updateTree()
    {
        //Update our tween tie-in animations
        updateAttributeAnimations();
        
        //Call our update function
        update();
    }
    
    void Node::drawTree()
    {
        //If we're invisible, nothing to do here
        if(!bVisible) return;
        
        //Update our draw order
        drawOrder = scene.lock()->getNextDrawOrder();
        
        //Push our Matrix
        ci::gl::pushMatrices();
        setTransformation();
        
        //Draw this item
        draw();
        
        //Draw bounds if necessary
        if(bDrawBounds)
            drawBounds();
        
        //Pop our Matrix
        ci::gl::popMatrices();
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark - Attributes -
    
    void Node::setPosition(float x, float y)
    {
        positionAnim.stop();
        bUpdatePositionFromAnim = false;
        position.set(x, y);
        positionAnim.ptr()->set(position);
        bFrameDirty = true;
    }
    
    void Node::setScale(float x, float y)
    {
        scaleAnim.stop();
        bUpdateScaleFromAnim = false;
        scale.set(x, y);
        scaleAnim.ptr()->set(scale);
        
        bFrameDirty     = true;
        bBoundsDirty    = true;
    }
    
    void Node::setRotation(float rotation)
    {
        rotationAnim.stop();
        bUpdateRotationFromAnim = false;
        this->rotation = rotation;
        rotationAnim = rotation;
        
        bFrameDirty     = true;
        bBoundsDirty    = true;
    }
    
    void Node::setAlpha(float alpha)
    {
        alphaAnim.stop();
        bUpdateAlphaFromAnim = false;
        this->alpha = ci::math<float>::clamp(alpha, 0.f, 1.f);
        alphaAnim = this->alpha;
    }
    
    void Node::setOffset(float x, float y) {
        offsetAnim.stop();
        bUpdateOffsetFromAnim = false;
        offset.set(x, y);
        offsetAnim.ptr()->set(offset);
        bFrameDirty = true;
        
        //If we are manually setting the offset, we can't have alignment
        setAlignment(Alignment::NONE);
    }
    
    void Node::initAttrAnimations()
    {
        //Initialize the isComplete() method of each tween, a bit annoying
        positionAnim.stop();
        scaleAnim.stop();
        rotationAnim.stop();
        offsetAnim.stop();
        alphaAnim.stop();
    }
    
    void Node::updateAttributeAnimations()
    {
        //See if a tween is in progress, if so we want to use that value
        //Setting an attribute calls stop(), so that will override this
        if(!positionAnim.isComplete())  bUpdatePositionFromAnim = true;
        if(!scaleAnim.isComplete())     bUpdateScaleFromAnim    = true;
        if(!rotationAnim.isComplete())  bUpdateRotationFromAnim = true;
        if(!alphaAnim.isComplete())     bUpdateAlphaFromAnim    = true;
        if(!offsetAnim.isComplete())    bUpdateOffsetFromAnim   = true;
        
        //Update Anims if we care
        if(bUpdatePositionFromAnim)     position    = positionAnim;
        if(bUpdateScaleFromAnim)        scale       = scaleAnim;
        if(bUpdateRotationFromAnim)     rotation    = rotationAnim;
        if(bUpdateAlphaFromAnim)        alpha       = alphaAnim;
        if(bUpdateOffsetFromAnim)       offset      = offsetAnim;
    }
    
    //------------------------------------------------------
    #pragma mark  - Alignment -
    
    void Node::setAlignment(po::Node::Alignment alignment)
    {
        this->alignment = alignment;
        
        if(alignment == Alignment::NONE) return;
        
        ci::Rectf bounds = getBounds();
        
        switch (alignment) {
            case Alignment::TOP_LEFT:
                offset.set(0,0); break;
            case Alignment::TOP_CENTER:
                offset.set(-bounds.getWidth()/2.f,0); break;
            case Alignment::TOP_RIGHT:
                offset.set(-bounds.getWidth(),0); break;
            case Alignment::CENTER_LEFT:
                offset.set(0,-bounds.getHeight()/2.f); break;
            case Alignment::CENTER_CENTER:
                offset.set(-bounds.getWidth()/2.f,-bounds.getHeight()/2.f); break;
            case Alignment::CENTER_RIGHT:
                offset.set(-bounds.getWidth(),-bounds.getHeight()/2.f); break;
            case Alignment::BOTTOM_LEFT:
                offset.set(0,-bounds.getHeight()); break;
            case Alignment::BOTTOM_CENTER:
                offset.set(-bounds.getWidth()/2.f,-bounds.getHeight()); break;
            case Alignment::BOTTOM_RIGHT:
                offset.set(-bounds.getWidth(),-bounds.getHeight()); break;
        }
        
        offset = offset-bounds.getUpperLeft();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Transformation -
    
    void Node::setTransformation()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::translate(position);
        ci::gl::rotate(rotation);
        ci::gl::scale(scale);
        
        ci::gl::translate(offset);
    
        matrix.set(ci::gl::getModelView(), ci::gl::getProjection(), ci::gl::getViewport());
    }
    
    ci::Vec2f Node::sceneToLocal(ci::Vec2f scenePoint)
    {
        return ci::Vec2f();
    }
    
    ci::Vec2f Node::globalToLocal(ci::Vec2f globalPoint)
    {
        return matrix.globalToLocal(globalPoint);
    }
    
    bool Node::pointInside(const ci::Vec2f &point)
    {
        return false;
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark - Parent & Scene -
    
    void Node::setScene(SceneRef sceneRef) {
        scene = sceneRef;
        if(hasScene()) scene.lock()->trackChildNode(shared_from_this());
    }
    
    SceneRef Node::getScene()
    {
        return scene.lock();
    }
    
    bool Node::hasScene()
    {
        return (scene.lock() ? true : false);
    }
    
    void Node::removeScene()
    {
        if(hasScene()) scene.lock()->untrackChildNode(shared_from_this());
        scene.reset();
    }
    
    void Node::setParent(NodeContainerRef containerNode)
    {
        parent = containerNode;
    }
    
    NodeContainerRef Node::getParent() const {
        return parent.lock();
    }
        
    bool Node::hasParent()
    {
        return (parent.lock() ? true : false);
    }
    
    void Node::removeParent() {
        parent.reset();
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark  - Dimensions -
    
    ci::Rectf Node::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        return bounds;
    }
    
    void Node::drawBounds()
    {
        ci::gl::color(255,0,0);
        
        //Draw bounding box
        ci::gl::drawStrokedRect(getBounds());
        
        //Draw origin
        ci::gl::pushMatrices();
        ci::gl::translate(-offset);
        ci::gl::scale(ci::Vec2f(1.f,1.f)/ scale);
        ci::gl::drawSolidRect(ci::Rectf(-ORIGIN_SIZE/2, -ORIGIN_SIZE/2, ORIGIN_SIZE, ORIGIN_SIZE));
        ci::gl::popMatrices();
    }
    
    ci::Rectf Node::getFrame()
    {
        if(bFrameDirty) {
            ci::Rectf r = getBounds();
            
            ci::MatrixAffine2f m;
            m.translate(position);
            m.rotate(ci::toRadians(getRotation()));
            m.scale(scale);
            m.translate(offset);
            
            frame = r.transformCopy(m);
            bFrameDirty = false;
        }
        return frame;
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark  - Events -
    
    #pragma mark General
    void Node::removeAllEvents() {}
    
    
    #pragma mark Mouse Events
    
    //Signals
    void Node::trackConnection(MouseEvent::Type type, Node *listener, scoped_connection *connection) {
        mouseConnections[type][listener] = std::unique_ptr<scoped_connection>(connection);
    };
    
    void Node::disconnect(MouseEvent::Type type, Node *listener) {
        mouseConnections[type][listener]->disconnect();
    }
    
    
    #pragma mark -
    //Mouse Down Inside
    void Node::connectMouseDownInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(signalMouseDownInside.connect(std::bind( &Node::mouseDownInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::DOWN_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseDownInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::DOWN_INSIDE, listener);
    };
    
    void Node::emitMouseDownInside(po::MouseEvent &event)
    {
        signalMouseDownInside(event);
    };
    
    
    #pragma mark -
    //Mouse Move Inside
    void Node::connectMouseMoveInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(signalMouseMoveInside.connect(std::bind( &Node::mouseMoveInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::MOVE_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseMoveInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::MOVE_INSIDE, listener);
    };
    
    void Node::emitMouseMoveInside(po::MouseEvent &event)
    {
        signalMouseMoveInside(event);
    };
    
    
    #pragma mark -
    //Mouse Drag Inside
    void Node::connectMouseDragInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(signalMouseDragInside.connect(std::bind( &Node::mouseDragInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::DRAG_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseDragInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::DRAG_INSIDE, listener);
    };
    
    void Node::emitMouseDragInside(po::MouseEvent &event)
    {
        signalMouseMoveInside(event);
    };
    
    
    #pragma mark -
    //Mouse Up Inside
    void Node::connectMouseUpInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(signalMouseUpInside.connect(std::bind( &Node::mouseUpInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::UP_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseUpInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::UP_INSIDE, listener);
    };
    
    void Node::emitMouseUpInside(po::MouseEvent &event)
    {
        signalMouseMoveInside(event);
    };

    
    #pragma mark -
    //Global Events
    void Node::notifyGlobal(po::MouseEvent &event) {
        switch (event.getType()) {
            case po::MouseEvent::Type::DOWN:
                mouseDown(event);
                break;
                
            case po::MouseEvent::Type::MOVE:
                mouseMove(event);
                break;
                
            case po::MouseEvent::Type::DRAG:
                mouseDrag(event);
                break;
                
            case po::MouseEvent::Type::UP:
                mouseUp(event);
                break;
                
            case po::MouseEvent::Type::WHEEL:
                mouseWheel(event);
                break;
        }
    }
    
    #pragma mark -
    //SceneGraph Specific events
    
    //See if we care about an event
    bool Node::hasConnection(po::MouseEvent::Type type)
    {
        if(!mouseConnections[type].size()) {
           return false;
        }
        else {
            std::map<po::Node*, std::unique_ptr<scoped_connection> >::iterator iter = mouseConnections[type].begin();
            for(; iter!=mouseConnections[type].end(); ++iter) {
                if(iter->second.get()->connected()) return true;
            }
        }
        
        return false;
    }
    
    //For the given event, notify everyone that we have as a subscriber
    void Node::emitEvent(po::MouseEvent &event)
    {
        //Set the source
        event.source = shared_from_this();
        
        //Emit the Event
        switch (event.getType()) {
            case po::MouseEvent::Type::DOWN_INSIDE:
                signalMouseDownInside(event);
                break;
                
            case po::MouseEvent::Type::MOVE_INSIDE:
                signalMouseMoveInside(event);
                break;
                
            case po::MouseEvent::Type::DRAG_INSIDE:
                signalMouseDragInside(event);
                break;
                
            case po::MouseEvent::Type::UP_INSIDE:
                signalMouseUpInside(event);
                break;
        }
    }
}
