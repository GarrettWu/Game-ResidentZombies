//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "BaseObject.h"

class Box : public BaseObject {
    
public:
    Box(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    virtual ~Box();
    
    virtual void update(float dt);
    
    BaseObject *child;
    
    static Box *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
        return WYNEW Box(x, y, objectsGroup, obj);
    }
    
    wySPXSprite *deadSprite;
    void dead();
    void getBonus(int type, int x, int y);
};
