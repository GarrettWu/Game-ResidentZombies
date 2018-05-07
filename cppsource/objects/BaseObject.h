#pragma once

#include "WiEngine.h"
#include "WiEngine-Box2D.h"

class BaseObject : public wyNode {
    
public:
    BaseObject();
    virtual ~BaseObject();
    
    int obj_type;
    int sub_type;
    
    int hp, maxHp;
    int atk;
    int gold;
    
    wyNode *container;
    wySPXSprite *spxSprite;
    
    b2Body *body;
    
    float spxScaleX;
    
    virtual void update(float dt);
    virtual void handleCollision(b2Body *actorB);
    virtual void endContact(b2Body *actorB);
    virtual void beginContact(b2Body *actorB);
    
    wyRect bodyRect;
    float offsetX, offsetY, ofyInMeter;
    
//    void createFixtureByCurrentFrame(wySPXSprite *spxSprite, bool isSensor);
    void createFixtureByCurrentFrame(bool isSensor);
    void createFixtureByCurrentFrame();
    
    float halfBodyHeightInMeter, halfBodyWidthInMeter;
    
    virtual void onHitHero();
    virtual void destroyMyself() {
        
    }
    
    bool isHeightInited;

	float distance;
    
    float widthInPixels;
    
    bool isSensor;
    
    float startX, endX;
    
    bool isDestroied;
    
    bool isRecreatingFixture;
};