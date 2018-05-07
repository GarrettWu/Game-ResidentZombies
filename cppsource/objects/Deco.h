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

class Deco : public BaseObject {
public:
    virtual void destroyMe() {
        
    }
};

class DecoLight : public Deco {
    
public:
    DecoLight(int x, int y);
    virtual ~DecoLight();
    
    virtual void update(float dt);
    
    static DecoLight *make(float x, float y) {
        return new DecoLight(x, y);
    }
    
    void destroyMe();
    
    wySpriteEx *lightSprite;
    bool isInit;
    float originX, originY;
};

class DecoTorch : public Deco {
    
public:
    wyParticleSystem *fire_emitter;
    
    DecoTorch(int x, int y);
    virtual ~DecoTorch();
    
    virtual void update(float dt);
    
    static DecoTorch *make(float x, float y) {
        return new DecoTorch(x, y);
    }
    
    void destroyMe();
    bool isInit;
    float originX, originY;
};

class DecoTree : public Deco {
    
public:
    wySpriteEx *treeSprite, *treeSprite1;
    
    DecoTree(int x, int y);
    virtual ~DecoTree();
    
    virtual void update(float dt);
    
    static DecoTree *make(float x, float y) {
        return new DecoTree(x, y);
    }
    
    void destroyMe();
};

