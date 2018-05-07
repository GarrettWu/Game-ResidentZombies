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

static const int END_NORMAL = 0;
static const int END_ILLUSION = 1;
static const int END_TUOLUO = 2;
static const int END_BOSS1 = 3;

class EndPoint : public BaseObject {
    
public:
    wySPXSprite *women;
    EndPoint(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, bool needShow);
    virtual ~EndPoint();
    
    virtual void update(float dt);
    
    static EndPoint *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
        return WYNEW EndPoint(x, y, objectsGroup, obj, true);
    }
    
    bool needShow;
    static EndPoint *make(float x, float y, bool needShow) {
        return WYNEW EndPoint(x, y, NULL, NULL, needShow);
    }
    
    int type;
    
    wySPXSprite* guang;
    
    void showDialogBox(int idx);
    void doEnd();
    float boss1Timer;
    bool isBoss1Talking, isBoss1Ending;
};


class CheckPoint : public BaseObject {
public:
    wySpriteEx *checkpointTxt;
    
    CheckPoint(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, bool needFire){
        this->needFire = needFire;
        offsetX = 0;
        offsetY = 0;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        container = wyNode::make();
        container->setVisible(false);
        container->setPosition(x - DP(0)*sGlobal->scaleY, y + DP(0)*sGlobal->scaleY);
        sGlobal->mainGameLayer->addChildLocked(container);
        
        wySPXSprite *checkPointSprite = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_DANYAOKU);//
        checkPointSprite->setForceTickMode(true);
        checkPointSprite->setUnitInterval(0.1f);
        checkPointSprite->setLoopCount(-1);
//        if (needFire) {
//            checkPointSprite->playAnimation(0);
//        }
        checkPointSprite->setScaleX(sGlobal->scaleX*0.8f);
        checkPointSprite->setScaleY(sGlobal->scaleY*0.8f);
        container->addChildLocked(checkPointSprite);
        spxSprite = checkPointSprite;
        
        obj_type = TYPE_CHECKPOINT;

        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(0));
        bd.userData = this;
        bd.type = b2_staticBody;
        
        b2PolygonShape staticBox;
        float wd = m_box2d->pixel2Meter(DP(100)*sGlobal->scaleX);
        staticBox.SetAsBox(wd, m_box2d->pixel2Meter(wyDevice::winHeight*10));	
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;	
        fixtureDef.density = 0.0f;   
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = true;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_CHECKPOINT];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_CHECKPOINT];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_CHECKPOINT];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetTransform(bd.position, 0);
        
        checkpointTxt = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "checkpoint_txt.png");
        checkpointTxt->setScaleX(sGlobal->scaleX*0.8f);
        checkpointTxt->setScaleY(sGlobal->scaleY*0.8f);
        checkpointTxt->setVisible(false);
        checkpointTxt->setPosition(x - DP(0)*sGlobal->scaleY, y + DP(60)*sGlobal->scaleY);
        sGlobal->mainGameLayer->addChildLocked(checkpointTxt, 101);
    }
    
    ~CheckPoint() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        b2World* world = m_box2d->getWorld();
        
//        float distance = m_box2d->meter2Pixel(body->GetPosition().x)+offsetX-m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x);
//        
//        if (distance < wyDevice::winWidth) {
            if (!container->isVisible()) {
                container->setVisible(true);
            }
//            spxSprite->tick(dt);
//        }
    }
    
    void fire() {
        sGlobal->mainGameLayer->addToDisable(body);
//        spxSprite->setLoopCount(-1);
//        spxSprite->playAnimation(0);
        if (!needFire) {
            checkpointTxt->setVisible(true);
            checkpointTxt->runAction(wyMoveBy::make(1.0f, 0, DP(100)*sGlobal->scaleY));
            checkpointTxt->runAction(wySequence::make(wyDelayTime::make(0.5f), wyFadeOut::make(0.5f), NULL));
        }
        
        sGlobal->playEffect(se_upgrade);
    }
    
    static CheckPoint *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
        return WYNEW CheckPoint(x, y, objectsGroup, obj, true);
    }
    
    bool needFire;
    static CheckPoint *make(float x, float y, bool needFire) {
        return WYNEW CheckPoint(x, y, NULL, NULL, needFire);
    }
};

//class Cannon : public BaseObject {
//public:
//    Cannon(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj){
//        offsetX = 0;
//        offsetY = 0;
//        
//        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        b2World* world = m_box2d->getWorld();
//        
//        wySPXSprite *checkPointSprite = wySPXSprite::make("huoba"), MyTextureManager::makePNG("huoba")), 0);
//        checkPointSprite->setForceTickMode(true);
//        checkPointSprite->setUnitInterval(0.1f);
//        checkPointSprite->setLoopCount(-1);
//        
//        obj_type = TYPE_CANNON;
//        sprite = checkPointSprite;
//        sprite->setScaleX(sGlobal->scaleX);
//        sprite->setScaleY(sGlobal->scaleY);
//        sGlobal->mainGameLayer->addChildLocked(sprite);
//        sprite->setPosition(x, y + DP(30)*sGlobal->scaleY);
//        sprite->setVisible(false);
//        
//        b2BodyDef bd;
//        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(0));
//        bd.userData = this;
//        bd.type = b2_staticBody;
//        
//        b2PolygonShape staticBox;
//        float wd = m_box2d->pixel2Meter(DP(10));
//        staticBox.SetAsBox(wd, m_box2d->pixel2Meter(wyDevice::winHeight*10));	
//        b2FixtureDef fixtureDef;
//        fixtureDef.shape = &staticBox;	
//        fixtureDef.density = 0.0f;   
//        fixtureDef.friction = 0.0f;
//        fixtureDef.restitution = 0.0f;
//        fixtureDef.isSensor = true;
//        body = world->CreateBody(&bd);
//        body->CreateFixture(&fixtureDef);
//        body->SetTransform(bd.position, 0);
//    }
//    
//    ~Cannon() {
//        
//    }
//    
//    void update(float dt) {
//        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        b2World* world = m_box2d->getWorld();
//
//        if (!sprite->isVisible()) {
//            sprite->setVisible(true);
//        }
//        ((wySPXSprite *)sprite)->tick(dt);
//    }
//    
//    void fire() {
//        sGlobal->mainGameLayer->addToDisable(body);
//        b2Vec2 f = sGlobal->mainGameLayer->hero->body->GetWorldVector(b2Vec2(10.0f, 90.0f));
//        b2Vec2 p = sGlobal->mainGameLayer->hero->body->GetWorldPoint(b2Vec2(0.0f, 0.0f));
//        b2Vec2 vel = sGlobal->mainGameLayer->hero->body->GetLinearVelocity();
//        sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(vel.x, 0));
//        sGlobal->mainGameLayer->hero->body->ApplyLinearImpulse(f, p);
//        sGlobal->mainGameLayer->hero->needSetSpeed = true;
//    }
//    
//    static Cannon *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
//        return WYNEW Cannon(x, y, objectsGroup, obj);
//    }
//};

