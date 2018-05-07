//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "Enemy.h"

class WolfZombie : public Enemy {
    
public:
    WolfZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~WolfZombie();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    
    static WolfZombie *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW WolfZombie(x, y, objectsGroup, obj, myCmd);
    }
    
    void fight();
};
