//
//  poScene.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "poScene.h"
#include "poNodeContainer.h"

namespace po {
    SceneRef Scene::create()
    {
        SceneRef scene(new Scene());
        std::cout << scene.get() << std::endl;
        //#pragma message "Need to call this here, can't do it in the constructor with shared_from_this"
        scene->getRootNode()->setScene(scene);
        return scene;
    }
    
    Scene::Scene()
    : rootNode(po::NodeContainer::create())
    , eventCenter(EventCenter::create())
    {
    }
    
    Scene::~Scene()
    {
    }
    
    void Scene::update()
    {
        eventCenter->processEvents(shared_from_this());
        getRootNode()->updateTree();
    }
    
    void Scene::draw()
    {
        drawOrderCounter = 0;
        getRootNode()->drawTree();
    }
    
    NodeContainerRef Scene::getRootNode()
    {
        return rootNode;
    }
    
    //Child node tracking
    void Scene::trackChildNode(NodeRef node) {
        allChildren.push_back(node);
    }
    
    void Scene::untrackChildNode(NodeRef node) {
        std::vector<NodeRef>::iterator iter = std::find(allChildren.begin(), allChildren.end(), node);
        if(iter != allChildren.end())
            allChildren.erase(iter);
    }
    
    uint Scene::getNextDrawOrder()
    {
        return drawOrderCounter++;
    }
    
}
