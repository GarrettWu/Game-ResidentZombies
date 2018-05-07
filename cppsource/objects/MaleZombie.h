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

class MaleZombie : public Enemy {
    
public:
    MaleZombie(float x, float y, int type, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~MaleZombie();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    void beginContact(b2Body *actorB);
    void endContact(b2Body *actorB);
    
    static MaleZombie *make(float x, float y, int type, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW MaleZombie(x, y, type, objectsGroup, obj, myCmd);
    }
    
    void fight();
    
    void onDead();
};
