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

const static int SK_IDLE = 0;
const static int SK_WALK = 1;
const static int SK_ATK = 2;

class GhostBaby : public Enemy {
    
public:
    GhostBaby(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~GhostBaby();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    
    static GhostBaby *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW GhostBaby(x, y, objectsGroup, obj, myCmd);
    }
    
    void fight();
};
