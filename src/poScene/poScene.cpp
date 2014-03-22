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
         return std::make_shared<Scene>();
    }
    
    Scene::Scene()
    : rootNode(po::NodeContainer::create())
    {
    }
    
    
    Scene::~Scene()
    {
    }
    
    void Scene::update()
    {
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
    
    uint Scene::getNextDrawOrder()
    {
        return drawOrderCounter++;
    }
    
}
