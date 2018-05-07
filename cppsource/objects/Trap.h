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

class TrapBase : public BaseObject {
    
public:
    virtual void destroyMyself() {
        
    }
};

class Trap : public TrapBase {
    
public:
    Trap(float x, float y);
    virtual ~Trap();
    
    virtual void update(float dt);
    
    static Trap *make(float x, float y) {
        return WYNEW Trap(x, y);
    }
    
    int originX, originY;
    bool isInitToShow;
    void initToShow();
    void destroyMyself();
};

class Trap_S03 : public TrapBase {
public:
    wySPXSprite *light, *baozha;
    bool isBaoing;
    int curColor;
    float colorTimer;
    
    Trap_S03(float x, float y) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;

        if (sGlobal->isRealEndless) {
            switch (sGlobal->combatRound) {
                case 1:
                    hp = sGlobal->ENEMY_HP_EASY[e_id];
                    atk = sGlobal->ENEMY_ATK_EASY[e_id];
                    gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                    break;
                case 2:
                    hp = sGlobal->ENEMY_HP_HARD[e_id];
                    atk = sGlobal->ENEMY_ATK_HARD[e_id];
                    gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                    break;
                case 3:
                    hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                    atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                    gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                    break;
                default:
                    hp = sGlobal->ENEMY_HP_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_HP_POWER, sGlobal->combatRound - 3);
                    atk = sGlobal->ENEMY_ATK_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                    gold = sGlobal->ENEMY_GOLD_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_GOLD_POWER, sGlobal->combatRound - 3);
                    break;
            }
//            LOGE("combat round:%d, hp:%d, atk:%d, gold:%d", sGlobal->combatRound, hp, atk, gold);
        } else {
            switch (sGlobal->currentSModeDifficulty) {
                case DIFFICULTY_NORMAL:
                    hp = sGlobal->ENEMY_HP_EASY[e_id];
                    atk = sGlobal->ENEMY_ATK_EASY[e_id];
                    gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                    break;
                case DIFFICULTY_HEROIC:
                    hp = sGlobal->ENEMY_HP_HARD[e_id];
                    atk = sGlobal->ENEMY_ATK_HARD[e_id];
                    gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                    break;
                case DIFFICULTY_HARD:
                    hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                    atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                    gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                    break;
                default:
                    hp = sGlobal->ENEMY_HP_EASY[e_id];
                    atk = sGlobal->ENEMY_ATK_EASY[e_id];
                    gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                    break;
            }
        }
        atk *= sGlobal->ENEMY_ATK_MULTI;
        hp *= sGlobal->ENEMY_HP_MULTI;
        gold *= sGlobal->ENEMY_GOLD_MULTI;
        
        obj_type = TYPE_TRAP;
        
        float boxWidth = DP(25)*sGlobal->scaleX;
        float boxHeight = DP(5)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x+DP(4)*sGlobal->scaleX), m_box2d->pixel2Meter(y + DP(40)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_staticBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));	
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;	
        fixtureDef.density = 0.0f;   
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = true;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_TRAP];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_TRAP];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_TRAP];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);

        offsetX = 0;
        offsetY = 0;
        
        isBaoing = false;
        
        curColor = 0;
        colorTimer = 0;
        
        originX = x;
        originY = y;
        isInitToShow = false;
    }
    
    ~Trap_S03() {
        
    }
    
    void update(float dt) {
        if (!isInitToShow) {
            isInitToShow = true;
            initToShow();
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

        if (!container->isVisible()) {
            container->setVisible(true);
        } else {
//            colorTimer += dt;
//            if (colorTimer > 0.2f) {
//                if (curColor == 0) {
//                    curColor = 1;
//                    spxSprite->setColor(wyc3b(255, 100, 100));
//                } else {
//                    curColor = 0;
//                    spxSprite->setColor(wyc3b(255, 255, 255));
//                }
//                colorTimer = 0;
//            }
        }

        if (isBaoing) {
            baozha->tick(dt);
        }
    }
    
    static Trap_S03 *make(float x, float y) {
        return WYNEW Trap_S03(x, y);
    }
    
    void onHitHero() {
        if (!isBaoing) {
            sGlobal->playEffect(se_bomb);
            isBaoing = true;
            baozha->setVisible(true);
            spxSprite->setVisible(false);
            light->setVisible(false);
        }
    }
    
    int originX, originY;
    bool isInitToShow;
    
    void initToShow() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, 2);
        
        spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
        spxSprite->setForceTickMode(true);
        spxSprite->setUnitInterval(0.1f);
        spxSprite->setLoopCount(0);
        spxSprite->setScaleX(sGlobal->scaleX*0.7f);
        spxSprite->setScaleY(sGlobal->scaleY*0.7f);
        spxSprite->playAnimation(SPX_DILEI);
        spxSprite->setVisible(true);
        spxSprite->setAlpha(255);
        spxSprite->setPosition(0, 0);
        spxSprite->setRotation(0);
        container->addChildLocked(spxSprite);
        
        light = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
        light->setForceTickMode(true);
        light->setUnitInterval(0.1f);
        light->setLoopCount(0);
        light->setPosition(DP(-2)*sGlobal->scaleX, DP(32)*sGlobal->scaleX);
        light->playAnimation(SPX_DILEI_LIGHT_1);
        container->addChildLocked(light, -1);
        float fs = sGlobal->scaleX*0.8f;
        float ts = sGlobal->scaleX*1.2f;
        light->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fs, fs, ts, ts), wyScaleTo::make(0.6f, ts, ts, fs, fs), NULL)));
        light->runAction(wyRepeatForever::make(wyRotateBy::make(1.2f, 360)));
        light->setColor(wyc3b(255, 30, 30));
        light->setAlpha(200);
        
        baozha = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
        baozha->setForceTickMode(true);
        baozha->setUnitInterval(0.1f);
        baozha->setLoopCount(0);
        baozha->setScaleX(sGlobal->scaleX*1.6f);
        baozha->setScaleY(sGlobal->scaleY*1.6f);
        baozha->setVisible(false);
        baozha->setPosition(DP(10)*sGlobal->scaleX, 0);
        baozha->playAnimation(SPX_DILEI_BAOZHA);
        baozha->setAlpha(255);
        baozha->setRotation(0);
        container->addChildLocked(baozha);
        
        container->setPosition(originX+DP(4)*sGlobal->scaleX, originY+DP(30)*sGlobal->scaleY);
    }
    
    void destroyMyself() {
//        LOGE("trap2,3 destoryed...");
        baozha->stopAllActions();
        baozha->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, baozha);
        
        light->stopAllActions();
        light->setAFCSpriteCallback(NULL, NULL);
//        light->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
        sGlobal->pushToCache(sGlobal->effects_cache, light);
        
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        spxSprite->setColor(wyc3b(255, 255, 255));
        sGlobal->pushToCache(sGlobal->effects_cache, spxSprite);
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        this->release();
        
        return;
    }
};


