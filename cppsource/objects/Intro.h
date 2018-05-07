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
#include "Global.h"

class Intro : public BaseObject {
    
public:
    Intro(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    virtual ~Intro();
    
    virtual void update(float dt);
    
    static Intro *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
        return WYNEW Intro(x, y, objectsGroup, obj);
    }
    
    const char *name;
};
