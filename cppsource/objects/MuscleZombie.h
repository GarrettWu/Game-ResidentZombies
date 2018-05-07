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

class MuscleZombie : public Enemy {
    
public:
    MuscleZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~MuscleZombie();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    void endContact(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    
    static MuscleZombie *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW MuscleZombie(x, y, objectsGroup, obj, myCmd);
    }

    void destroyMyself();

    void dead();
};
