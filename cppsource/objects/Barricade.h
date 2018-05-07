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
#include "HeroBullet.h"

class Barricade : public Enemy {
    
public:
    Barricade(float x, float y);
    virtual ~Barricade();
    
    virtual void update(float dt);
    
    static Barricade *make(float x, float y) {
        return WYNEW Barricade(x, y);
    }
    
    void dead();
    void hurt(float y);
    void dealHitByDart(float y, b2Body *bulletBody);
    void dealHitByLiudan(b2Body *bulletBody);
    bool canBeHurt();
    
    int originX, originY;
    void initToShow();
    void destroyMyself();
    void onHitHero();
};

class Barricade_S02 : public Enemy {
    
public:
    bool isBounce;
    int shakeTime;
    
    Barricade_S02(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL, int b1 = 0, int b2 = 0) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd, false){
        sub_type = 0;
        
        e_id = ENEMY_ZHANGAI2;

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

        if (objectsGroup == NULL || obj == NULL) {
            isBounce = true;
        } else {
            const char *bounce = objectsGroup->getObjectProperty(obj, "bounce");
            if (bounce != NULL && atoi(bounce) == 0) {
                isBounce = false;
            } else {
                isBounce = true;
            }
        }
        
        if (b1 == 1) {
            isBounce = b2 == 1 ? true : false;
        }

        obj_type = TYPE_ENEMY;
        isMoving = false;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        float boxWidth = DP(50)*sGlobal->scaleX;
        float boxHeight = DP(50)*sGlobal->scaleY;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + boxHeight + DP(32)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        float wd = m_box2d->pixel2Meter(boxWidth);
        this->halfBodyHeightInMeter = m_box2d->pixel2Meter(boxHeight);
        staticBox.SetAsBox(wd, m_box2d->pixel2Meter(boxHeight));
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;	
        fixtureDef.density = 0.0f;   
        fixtureDef.friction = 0.0f;
        if (isBounce) {
            if (sGlobal->isRealEndless) {
                fixtureDef.restitution = 0.5f;
            } else {
                fixtureDef.restitution = 0.9f;
            }
        } else {
            fixtureDef.restitution = 0.0f;
        }
        fixtureDef.isSensor = false;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetTransform(bd.position, 0);
        body->SetActive(false);
        body->SetAwake(false);
        
        deadSprite = NULL;

        offsetY = -DP(6)*sGlobal->scaleY;
        
        shakeTime = 0;
        
        originX = x;
        originY = y;
        
        isInitToShow = false;
        spxCacheArray = sGlobal->effects_cache;
        
        stunnable = false;
        canKnockBack = false;
        
        ofyInMeter = 0;
    }
    
    ~Barricade_S02() {
        
    }
    
    void update(float dt) {
        if (!isInitToShow) {
            isInitToShow = true;
            initToShow();
            return;
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        b2World* world = m_box2d->getWorld();
        
        if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {//
            float dist = distance;
            if (dist <= -DP(50)*sGlobal->scaleX) {
                sGlobal->nearestEnemy = NULL;
                sGlobal->nearestDistance = 1000;
            } else {
                if (dist < sGlobal->nearestDistance && dist > 0) {
                    sGlobal->nearestEnemy = this;
                    sGlobal->nearestDistance = dist;
                }
            }
        }
        
        float prevY = container->getPositionY();
        
        if (isDead) {
//            body->SetLinearVelocity(b2Vec2(0, 0));
//            spxSprite->tick(dt);
//            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
            
            if (bloodSprite->isVisible()) {
                bloodSprite->tick(dt);
                bloodSprite->setPosition(container->getPositionX()-DP(20)*sGlobal->scaleX, bloodSprite->getPositionY() + (container->getPositionY() - prevY));
            }
            
            if (deadSprite->isVisible()) {
                deadSprite->tick(dt);
                deadSprite->setPosition(container->getPositionX()-DP(0), container->getPositionY()- DP(0));
            }
            return;
        }
        
        distance = m_box2d->meter2Pixel(body->GetPosition().x)+offsetX-m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x);
        
        switch (appearType) {
            case APPEAR_NORMAL: {
                if (distance < appearDistance) {
                    if (!container->isVisible()) {
                        container->setVisible(true);
                        sGlobal->mainGameLayer->addToEnable(body);
                    }
                    spxSprite->tick(dt);
                    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
                }
                break;
            }
            default:
                break;
        }
        
        if (container->isVisible() && cmdCount > 0) {
            isAppeared = true;
            isReadyToCmd = true;
            if (cmdList[0] == APPEAR_NORMAL) {
                curCmd++;
            }
        }
        
        if (isReadyToCmd && !isCmdFinished) {
            if (curCmd == cmdCount) {
                isCmdFinished = true;
                isTiming = false;
//                LOGE("cmd end");
                return;
            }

            switch (cmdList[curCmd]) {
                case CMD_WALK:
                    if (isTiming) {
                        cmdTimer += dt;
                        if (cmdTimer >= cmdArg1List[curCmd]) {
                            isTiming = false;
                            cmdTimer = 0;
                            curCmd++;
                            isDone = false;
                            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
//                            spxSprite->playAnimation(1);
                            spxSprite->stopAllActions();
                            spxSprite->setRotation(0);
                        }
                    } else {
                        isTiming = true;
                        cmdTimer = 0;
                        body->SetLinearVelocity(b2Vec2(cmdArg2List[curCmd], cmdArg3List[curCmd]));
                        
                        if (fabs(cmdArg2List[curCmd]+0.0f) < 0.000001f && fabs(cmdArg2List[curCmd]+0.0f) < 0.000001f) {
                            playAnim(1);
                        } else {
                            if (cmdArg2List[curCmd] > 0.0f) {
                                spxSprite->runAction(wyRepeatForever::make(wyRotateBy::make(1.0f, 360)));
                            } else {
                                spxSprite->runAction(wyRepeatForever::make(wyRotateBy::make(1.0f, -360)));
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        
        if (bloodSprite->isVisible()) {
            bloodSprite->tick(dt);
            bloodSprite->setPosition(container->getPositionX()-DP(20)*sGlobal->scaleX, bloodSprite->getPositionY() + (container->getPositionY() - prevY));
        }
        
        if (deadSprite->isVisible()) {
            deadSprite->tick(dt);
            deadSprite->setPosition(container->getPositionX(), container->getPositionY());
        }
        
//        if (shadow->isVisible()) {
//            shadow->setPosition(container->getPositionX(), container->getPositionY()-DP(45)*sGlobal->scaleY);
//        }
        
        if (body->GetPosition().y < 0) {
            sGlobal->mainGameLayer->addToDestroy(body);
        }
    }
    
    static Barricade_S02 *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL, int b1 = 0, int b2 = 0) {
        return WYNEW Barricade_S02(x, y, objectsGroup, obj, myCmd, b1, b2);
    }
    
    void dead() {
        if (isDead)
            return;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
//        if (!bloodSprite->isVisible()) {
//            bloodSprite->setVisible(true);
//            bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
//            bloodSprite->setPosition(container->getPositionX()-DP(20)*sGlobal->scaleX, container->getPositionY()+DP(24)*sGlobal->scaleY);
//        }
        
        bloodSprite->setVisible(false);
        shadow->setVisible(false);
        
        if (!deadSprite->isVisible()) {
            deadSprite->setVisible(true);
        }
        
        spxSprite->setVisible(false);
        showDeadEffect();
        
        sGlobal->mainGameLayer->addToDisable(body);
        
        isDead = true;
        
        sGlobal->playEffect(se_bomb);
    }
    
    bool canBeHurt() {
        return true;
    }
    
    void dealHitByDart(float y, b2Body *bulletBody) {
        if (hp < 0 || !canBeHurt()) {
            return;
        }
        
        if (canBeMissed()) {
            return;
        } else {
            HeroBullet *bullet = (HeroBullet *)bulletBody->GetUserData();
            bullet->dealHitEnemy(y, this);
        }
    }
    
    void dealHitByLiudan(b2Body *bulletBody) {
        if (hp < 0 || isHurtByFire || !canBeHurt()) {
            return;
        }
        
        isHurtByFire = true;
        
        int damage = sGlobal->skillMulti[SKILL_MISSLE];
        if (hp - damage <= 0) {
            hp = 0;
            dead();
        } else {
            hp -= damage;

            spxSprite->setColor(wyc3b(255, 0, 0));
            wyIntervalAction *delay = wyDelayTime::make(0.2f);
            wyActionCallback callback = {
                NULL,
                onHurtDelayEnded,
                NULL
            };
            delay->setCallback(&callback, spxSprite);
            delay->setTag(101);
            spxSprite->runAction(delay);
        }
    }
    
    void hurt(float y) {
//        if (!bloodSprite->isVisible()) {
            bloodSprite->setVisible(true);
            bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
            bloodSprite->setPosition(container->getPositionX(), container->getPositionY()+DP(24)*sGlobal->scaleY);
//        }
    }
    
    
    void handleCollision(b2Body *actorB) {
    }
    
    void beginContact(b2Body *actorB) {
        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
        int tagB = userDataB->obj_type;
        switch (tagB) {
            case TYPE_GROUND: {
                if (shakeTime < 3) {
                    sGlobal->mainGameLayer->hero->isShaking = true;
                    shakeTime++;
                }
                shadow->setVisible(true);
                break;
            }
            default:
                break;
        }
    }
    
    void endContact(b2Body *actorB) {
        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
        int tagB = userDataB->obj_type;
        switch (tagB) {
            case TYPE_GROUND: {
                shadow->setVisible(false);
                break;
            }
            default:
                break;
        }
    }
    
    int originX, originY;
    
    void initToShow() {
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(false);
        sGlobal->mainGameLayer->addChildLocked(container);
        
        spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
        spxSprite->setForceTickMode(true);
        spxSprite->setUnitInterval(0.1f);
        spxSprite->setLoopCount(0);
        spxSprite->setScaleX(sGlobal->scaleX*0.6f);
        spxSprite->setScaleY(sGlobal->scaleY*0.6f);
        spxSprite->playAnimation(SPX_SHITOU);
        spxSprite->setVisible(true);
        spxSprite->setPosition(0, 0);
        container->addChildLocked(spxSprite);
        
        bloodSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        sGlobal->mainGameLayer->addChildLocked(bloodSprite, 103);
        bloodSprite->setForceTickMode(true);
        bloodSprite->setUnitInterval(0.1f);
        bloodSprite->setLoopCount(0);
        bloodSprite->setFlipX(false);
        bloodSprite->setScaleX(1.0f*sGlobal->scaleX);
        bloodSprite->setScaleY(1.0f*sGlobal->scaleY);
        bloodSprite->setVisible(false);
        wyAFCSpriteCallback callback1 = {
            NULL,
            onBloodEnded
        };
        bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
        bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
        
        deadSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        sGlobal->mainGameLayer->addChildLocked(deadSprite, 102);
        deadSprite->setForceTickMode(true);
        deadSprite->setUnitInterval(0.03f);
        deadSprite->setLoopCount(0);
        deadSprite->setScaleX(2.0f*sGlobal->scaleX);
        deadSprite->setScaleY(2.0f*sGlobal->scaleY);
        wyAFCSpriteCallback callback = {
            NULL,
            onBDeadEnded
        };
        deadSprite->setVisible(false);
        deadSprite->setAFCSpriteCallback(&callback, this);
        deadSprite->playAnimation(SPX_WUTI_BAOZHA);
        
        shadow = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
        shadow->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "shadow.png"));
        shadow->setScaleX(sGlobal->scaleX*2.5f);
        shadow->setScaleY(sGlobal->scaleY*1.5f);
        container->addChildLocked(shadow, -1);
        shadow->setVisible(false);
        shadow->setAlpha(100);
        
        deadSprite->setPosition(originX+DP(15)*sGlobal->scaleX, originY+DP(60)*sGlobal->scaleY);
        bloodSprite->setPosition(originX+DP(15)*sGlobal->scaleX, originY+DP(10)*sGlobal->scaleY);
        container->setPosition(originX, originY + DP(50)*sGlobal->scaleY*0.8f+DP(36)*sGlobal->scaleY);
        
        isAppeared = true;
    }
    
    void destroyMyself() {
        isDestroied = true;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }

        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);

        if (bloodSprite != NULL) {
            bloodSprite->stopAllActions();
            bloodSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
        }
        
        if (deadSprite != NULL) {
            deadSprite->stopAllActions();
            deadSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
        }
        
        if (shadow != NULL) {
            sGlobal->pushToCache(sGlobal->shadowSpriteCache, shadow);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
//        LOGE("6");
        this->release();
        
        return;
    }
};

class Barricade_S03 : public Enemy {
public:
    bool isDead;
    Barricade_S03(float x, float y) : Enemy(x, y, APPEAR_NORMAL, NULL, NULL, NULL, false) {
        y+= DP(42)*sGlobal->scaleY;
        
        e_id = ENEMY_ZHANGAI3;

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
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        obj_type = TYPE_ENEMY;
        isMoving = false;
        
        float boxWidth = DP(40)*sGlobal->scaleX*0.8f;
        float boxHeight = DP(50)*sGlobal->scaleY*0.8f;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + boxHeight));
        bd.userData = this;
        bd.type = b2_staticBody;
        
        b2PolygonShape staticBox;
        float wd = m_box2d->pixel2Meter(boxWidth);
        staticBox.SetAsBox(wd, m_box2d->pixel2Meter(boxHeight));	
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;	
        fixtureDef.density = 0.0f;   
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = true;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetTransform(bd.position, 0);
        body->SetActive(false);
        body->SetAwake(false);
        
        deadSprite = NULL;
        
        isDead = false;
        
        originX = x;
        originY = y;
        
        isInitToShow = false;
        spxCacheArray = sGlobal->effects_cache;
        
        stunnable = false;
        canKnockBack = false;
    }
    
    ~Barricade_S03() {
        
    }
    
    void update(float dt) {
        if (!isInitToShow) {
            isInitToShow = true;
            initToShow();
            return;
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        b2World* world = m_box2d->getWorld();
        
        if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {//
            float dist = distance;
            if (dist <= -DP(50)*sGlobal->scaleX) {
                sGlobal->nearestEnemy = NULL;
                sGlobal->nearestDistance = 1000;
            } else {
                if (dist < sGlobal->nearestDistance && dist > 0) {
                    sGlobal->nearestEnemy = this;
                    sGlobal->nearestDistance = dist;
                }
            }
        }
        
//        distance = m_box2d->meter2Pixel(body->GetPosition().x)+offsetX-m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x);
//        
//        if (distance < wyDevice::winWidth) {
            if (!isDead && !container->isVisible()) {
                container->setVisible(true);
                sGlobal->mainGameLayer->addToEnable(body);
            }
            spxSprite->tick(dt);
//        }
        
        if (bloodSprite->isVisible()) {
            bloodSprite->tick(dt);
        }
        
        if (deadSprite->isVisible()) {
            deadSprite->tick(dt);
        }
    }
    
    static Barricade_S03 *make(float x, float y) {
        return WYNEW Barricade_S03(x, y);
    }
    
    void dead() {
        if (isDead)
            return;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
//        if (!bloodSprite->isVisible()) {
//            bloodSprite->setVisible(true);
//            bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
//            bloodSprite->setPosition(container->getPositionX(), container->getPositionY()+DP(40)*sGlobal->scaleY);
//        }
        
        bloodSprite->setVisible(false);
        shadow->setVisible(false);
        
        if (!deadSprite->isVisible()) {
            deadSprite->setVisible(true);
        }
        
        spxSprite->setVisible(false);
        showDeadEffect();
        
        sGlobal->mainGameLayer->addToDisable(body);
        
        isDead = true;
    }
    
    bool canBeHurt() {
        return true;
    }
    
    void dealHitByDart(float y, b2Body *bulletBody) {
        if (hp < 0 || !canBeHurt()) {
            return;
        }
        
        if (canBeMissed()) {
            return;
        } else {
            HeroBullet *bullet = (HeroBullet *)bulletBody->GetUserData();
            bullet->dealHitEnemy(y, this);
        }
    }
    
    void dealHitByLiudan(b2Body *bulletBody) {
        if (hp < 0 || isHurtByFire || !canBeHurt()) {
            return;
        }
        
        isHurtByFire = true;
        
        int damage = sGlobal->skillMulti[SKILL_MISSLE];
        if (hp - damage <= 0) {
            hp = 0;
            dead();
        } else {
            hp -= damage;
//            wyAction *action = spxSprite->getAction(100);
//            if (action == NULL || action->isDone()) {
//                wyIntervalAction* a = wyFadeTo::make(0.2f, 255, 100);
//                wyIntervalAction* r = wySequence::make(a,
//                                                       (wyFiniteTimeAction*)a->reverse(),
//                                                       a->copy(),
//                                                       (wyFiniteTimeAction*)a->reverse(),
//                                                       NULL);
//                r->setTag(100);
//                spxSprite->runAction(r);
//            }
            spxSprite->setColor(wyc3b(255, 0, 0));
            wyIntervalAction *delay = wyDelayTime::make(0.2f);
            wyActionCallback callback = {
                NULL,
                onHurtDelayEnded,
                NULL
            };
            delay->setCallback(&callback, spxSprite);
            delay->setTag(101);
            spxSprite->runAction(delay);
        }
    }
    
    void hurt(float y) {
//        if (!bloodSprite->isVisible()) {
            bloodSprite->setVisible(true);
            bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
            bloodSprite->setPosition(container->getPositionX()-DP(15)*sGlobal->scaleX, container->getPositionY()+DP(30)*sGlobal->scaleY);
//        }
    }
    
    int originX, originY;
    
    void initToShow() {
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(false);
        sGlobal->mainGameLayer->addChildLocked(container);
        
        spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
        spxSprite->setForceTickMode(true);
        spxSprite->setUnitInterval(0.1f);
        spxSprite->setLoopCount(0);
        spxSprite->setScaleX(sGlobal->scaleX*0.8f);
        spxSprite->setScaleY(sGlobal->scaleY*0.8f);
        spxSprite->playAnimation(SPX_MUZHUANG);
        spxSprite->setVisible(true);
        spxSprite->setPosition(0, 0);
        container->addChildLocked(spxSprite);
        
        bloodSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        sGlobal->mainGameLayer->addChildLocked(bloodSprite, 103);
        bloodSprite->setForceTickMode(true);
        bloodSprite->setUnitInterval(0.1f);
        bloodSprite->setLoopCount(0);
        bloodSprite->setFlipX(false);
        bloodSprite->setScaleX(1.0f*sGlobal->scaleX);
        bloodSprite->setScaleY(1.0f*sGlobal->scaleY);
        bloodSprite->setVisible(false);
        wyAFCSpriteCallback callback1 = {
            NULL,
            onBloodEnded
        };
        bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
        bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
        
        deadSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        sGlobal->mainGameLayer->addChildLocked(deadSprite, 102);
        deadSprite->setForceTickMode(true);
        deadSprite->setUnitInterval(0.03f);
        deadSprite->setLoopCount(0);
        deadSprite->setScaleX(2.0f*sGlobal->scaleX);
        deadSprite->setScaleY(2.0f*sGlobal->scaleY);
        wyAFCSpriteCallback callback = {
            NULL,
            onBDeadEnded
        };
        deadSprite->setVisible(false);
        deadSprite->setAFCSpriteCallback(&callback, this);
        deadSprite->playAnimation(SPX_WUTI_BAOZHA);
        
        shadow = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
        shadow->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "shadow.png"));
        shadow->setScaleX(sGlobal->scaleX*3.0f);
        shadow->setScaleY(sGlobal->scaleY*2.6f);
        sGlobal->mainGameLayer->addChildLocked(shadow);
        shadow->setVisible(true);
        shadow->setAlpha(100);
        
        bloodSprite->setPosition(originX+DP(10)*sGlobal->scaleX, originY+DP(35)*sGlobal->scaleY);
        container->setPosition(originX, originY+DP(26)*sGlobal->scaleY);
        shadow->setPosition(originX, originY-DP(6)*sGlobal->scaleY);
        
        isAppeared = true;
    }
    
    void destroyMyself() {
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        if (bloodSprite != NULL) {
            bloodSprite->stopAllActions();
            bloodSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
        }
        
        if (deadSprite != NULL) {
            deadSprite->stopAllActions();
            deadSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
        }
        
        if (shadow != NULL) {
            sGlobal->pushToCache(sGlobal->shadowSpriteCache, shadow);
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        this->release();
        
        return;
    }
};
