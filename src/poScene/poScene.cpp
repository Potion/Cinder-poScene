//
//  poScene.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "poScene.h"

namespace po {
    SceneRef Scene::create()
    {
         return std::make_shared<Scene>();
    }
    
    Scene::Scene()
    : rootNode(po::Node::create())
    {
    }
    
    
    Scene::~Scene()
    {
    }
    
    void Scene::update()
    {
    }
    
    NodeRef Scene::getRootNode()
    {
        return rootNode;
    }
    
}
