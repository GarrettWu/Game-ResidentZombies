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

class FemaleZombie : public Enemy {
    
public:
    FemaleZombie(float x, float y, int type, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~FemaleZombie();
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    void endContact(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doFightAnim();
    
    static FemaleZombie *make(float x, float y, int type, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW FemaleZombie(x, y, type, objectsGroup, obj, myCmd);
    }

    void destroyMyself();
    void dead();
};

