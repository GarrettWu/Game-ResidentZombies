//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

//sharyu: sprite默认锚点是0.5, box2d默认锚点也是0.5

#include "Trap.h"
#include "Global.h"

Trap::Trap(float x, float y){
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    int e_id = ENEMY_XIANJING1;

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
//        LOGE("combat round:%d, hp:%d, atk:%d, gold:%d", sGlobal->combatRound, hp, atk, gold);
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
    
    float boxWidth = DP(10)*sGlobal->scaleX;
    float boxHeight = 1;
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x+DP(0)*sGlobal->scaleY), m_box2d->pixel2Meter(y+DP(54)*sGlobal->scaleY));
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
    body->SetActive(false);
    body->SetAwake(false);

    originX = x;
    originY = y;
    isInitToShow = false;
}

Trap::~Trap() {
}

void Trap::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (!isInitToShow) {
        isInitToShow = true;
        initToShow();
    }
    
    if (!spxSprite->isVisible()) {
        spxSprite->setVisible(true);
        sGlobal->mainGameLayer->addToEnable(body);
    }
    spxSprite->tick(dt);
}

void Trap::initToShow() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
    spxSprite->setForceTickMode(true);
	spxSprite->setUnitInterval(0.15f);
    spxSprite->setLoopCount(-1);
    spxSprite->setScaleX(sGlobal->scaleX*0.6f);
    spxSprite->setScaleY(sGlobal->scaleY*0.6f);
    spxSprite->playAnimation(SPX_DICI);
    spxSprite->setVisible(false);
    sGlobal->mainGameLayer->addChildLocked(spxSprite);
    
    spxSprite->setPosition(originX, originY + DP(43)*sGlobal->scaleY);
}

void Trap::destroyMyself() {
//    LOGE("trap destoryed...");
    spxSprite->stopAllActions();
    spxSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(sGlobal->effects_cache, spxSprite);

    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    world->DestroyBody(body);
    
    this->release();
    
    return;
}
