//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Deco.h"

DecoLight::DecoLight(int x, int y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    obj_type = TYPE_MAP_DECO;

    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_staticBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(1, 1);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_MAP_DECO];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_MAP_DECO];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_MAP_DECO];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetActive(false);
    body->SetAwake(false);
    body->SetGravityScale(0);
    
    originX = x;
    originY = y;
    isInit = false;
}

DecoLight::~DecoLight() {
    
}

void DecoLight::destroyMe() {
//    LOGE("destory me...");
    lightSprite->setVisible(false);
    lightSprite->setAlpha(255);
    lightSprite->stopAllActions();
    lightSprite->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });

    sGlobal->pushToCache(sGlobal->shadowSpriteCache, lightSprite);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();

    world->DestroyBody(body);
    
    delete this;
}

void DecoLight::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (distance < wyDevice::winWidth && !isInit) {
        isInit = true;

        lightSprite = (wySpriteEx *)sGlobal->popFromCache(sGlobal->shadowSpriteCache);
        lightSprite->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "light.png"));
        lightSprite->setScaleX(sGlobal->scaleX*1.0f);
        lightSprite->setScaleY(sGlobal->scaleY*1.21f);
        lightSprite->setAnchor(0.5, 1.0);
        lightSprite->setAlpha(60);
        lightSprite->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(lightSprite, 999);
        lightSprite->setPosition(originX + DP(24)*sGlobal->scaleY, originY - DP(16)*sGlobal->scaleY);
        lightSprite->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        lightSprite->setFlipX(false);

        lightSprite->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(1.0f, 60, 20), wyFadeTo::make(1.0f, 20, 60), NULL)));
    }
}

DecoTorch::DecoTorch(int x, int y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
//    container = wyNode::make();
//    sGlobal->mainGameLayer->addChildLocked(container);

//    fire_emitter = wyParticleLoader::load("particle_torch_fire"));
//    fire_emitter->setScaleX(sGlobal->scaleX*0.5f);
//    fire_emitter->setScaleY(sGlobal->scaleY*0.8f);
//    fire_emitter->setPositionType(GROUPED);
//    container->addChildLocked(fire_emitter, 1);
    
    obj_type = TYPE_MAP_DECO;
//    container->setVisible(false);
//    container->setPosition(x + DP(12)*sGlobal->scaleX, y - DP(36)*sGlobal->scaleY);
    x += DP(12)*sGlobal->scaleX;
    y += -DP(36)*sGlobal->scaleY;
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_staticBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(1, 1);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_MAP_DECO];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_MAP_DECO];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_MAP_DECO];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetActive(false);
    body->SetAwake(false);
    body->SetGravityScale(0);
    
    isInit = false;
    originX = x;
    originY = y;
    
    if (sGlobal->mainGameLayer->isEndless) {
        sGlobal->mainGameLayer->groundBody.push_back(body);
    }
}

DecoTorch::~DecoTorch() {
    
}

void DecoTorch::destroyMe() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    fire_emitter->setVisible(false);
    fire_emitter->stopSystem();
    fire_emitter->resetSystem();

    sGlobal->pushToCache(sGlobal->fire_emitterArray, fire_emitter);
//    LOGE("fire_emitterArray:%d", sGlobal->fire_emitterArray->num);

    world->DestroyBody(body);
    
    delete this;
}

void DecoTorch::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (distance < wyDevice::winWidth && !isInit) {
        isInit = true;

        fire_emitter = (wyParticleSystem *) sGlobal->popFromCache(sGlobal->fire_emitterArray);
        fire_emitter->setVisible(true);
        fire_emitter->setPositionType(GROUPED);
        fire_emitter->setDirectionAngleVariance(90, 0);
        fire_emitter->setStartSizeVariance(40, 0);
        fire_emitter->setLifeVariance(1.0f, 0);
        fire_emitter->setSpeedVariance(80, 0);
        fire_emitter->setScaleX(sGlobal->scaleX*0.75f);
        fire_emitter->setScaleY(sGlobal->scaleY*0.75f);
        sGlobal->mainGameLayer->addChildLocked(fire_emitter, -1);
        fire_emitter->setPosition(originX, originY);
    }
}

DecoTree::DecoTree(int x, int y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    container = wyNode::make();
    sGlobal->mainGameLayer->addChildLocked(container, 101);
    
    treeSprite1 = wySpriteEx::make(MyTextureManager::makePNG("s03.png"), wyr(296, 382, 282, 323));
    treeSprite1->setScaleX(sGlobal->scaleX*0.8f);
    treeSprite1->setScaleY(sGlobal->scaleY*0.8f);
    treeSprite1->setAnchor(0.5, 0);
    container->addChildLocked(treeSprite1, 1);
    
    treeSprite = wySpriteEx::make(MyTextureManager::makePNG("s03.png"), wyr(210, 600, 87, 105));
    treeSprite->setScaleX(sGlobal->scaleX*0.8f);
    treeSprite->setScaleY(sGlobal->scaleY*0.8f);
    treeSprite->setAnchor(0.5, 0);
    treeSprite->setPosition(DP(64)*sGlobal->scaleX, DP(40)*sGlobal->scaleY);
    container->addChildLocked(treeSprite, 1);
    
    obj_type = TYPE_MAP_DECO;
    container->setVisible(false);
    container->setPosition(x + DP(12)*sGlobal->scaleX, y - DP(36)*sGlobal->scaleY);
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_staticBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(1, 1);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_MAP_DECO];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_MAP_DECO];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_MAP_DECO];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetActive(false);
    body->SetAwake(false);
    body->SetGravityScale(0);
}

DecoTree::~DecoTree() {
    
}

void DecoTree::destroyMe() {
    
}

void DecoTree::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (distance < wyDevice::winWidth) {
        if (!container->isVisible()) {
            container->setVisible(true);
            int angel = 10;
            treeSprite->runAction(wyRepeatForever::make(wySequence::make(wySkewBy::make(randRange(2, 5)/10.0f, angel, 0), wySkewBy::make(randRange(2, 5)/10.0f, -angel, 0), wySkewBy::make(randRange(2, 5)/10.0f, -angel, 0), wySkewBy::make(randRange(2, 5)/10.0f, angel, 0), NULL)));
        }
    }
}
