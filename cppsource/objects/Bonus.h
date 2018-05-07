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

class Bonus : public BaseObject {
    
public:
    bool hasGravity;
    bool isInBox;
    int gold;
    int type;
    
    Bonus();
    virtual ~Bonus();
    
//    virtual void update(float dt);
    
//    static Bonus *make(float x, float y, int type, bool isInBox) {
//        return new Bonus(x, y, type, isInBox, false);
//    }
//    
//    static Bonus *make(float x, float y, int type, bool isInBox, bool hasGravity) {
//        return new Bonus(x, y, type, isInBox, hasGravity);
//    }
    
//    bool isInBox;
    
    virtual void show() {
        
    }
    
    virtual void beEaten() {
        
    }
    
    virtual void destroyBonus() {
        
    }
    
//    int gold;
};

class BonusCoin : public Bonus {
    
public:
    wySPXSprite *guang, *coinSprite;
    float orgx, orgy;
    bool isInitToShow;
    
    static BonusCoin *make(float x, float y, int type, bool isInBox) {
        return new BonusCoin(x, y, type, isInBox, false);
    }
    
    static BonusCoin *make(float x, float y, int type, bool isInBox, bool hasGravity) {
        return new BonusCoin(x, y, type, isInBox, hasGravity);
    }
    
    BonusCoin(float x, float y, int type, bool isInBox, bool hasGravity) : Bonus(){
        this->hasGravity = hasGravity;
        this->type = type;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();

        switch (type) {
            case COIN:
                gold = sGlobal->SMALL_COIN_GOLD;
                break;
            case BIGCOIN:
                gold = sGlobal->MIDDLE_COIN_GOLD;
                break;
            case BIGBIGCOIN: {
                gold = sGlobal->LARGE_COIN_GOLD;
                break;
            }
            default:
                break;
        }
        obj_type = TYPE_BONUS;

        float ofy;
        if (!isInBox) {
            ofy = 0;
//			container->setPosition(x, y - DP(4)*sGlobal->scaleY);
            orgx = x;
            orgy = y - DP(4)*sGlobal->scaleY;
        } else {
            ofy = 80;
//			container->setPosition(x, y + DP(80)*sGlobal->scaleY);
            orgx = x;
            orgy = y + DP(80)*sGlobal->scaleY;
        }
        
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y+DP(ofy)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape dynamicBox;
        float wd = m_box2d->pixel2Meter(DP(14) * sGlobal->scaleX);
        dynamicBox.SetAsBox(wd, m_box2d->pixel2Meter(DP(16)*sGlobal->scaleY));	
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        
        if (!hasGravity) {
            fixtureDef.density = 0.0f;   
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.isSensor = true;
            fixtureDef.filter.maskBits = MASK_BITS[TYPE_BONUS];
            fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BONUS];
            fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BONUS];
            
            body = world->CreateBody(&bd);
            body->CreateFixture(&fixtureDef);
            body->SetTransform(bd.position, 0);
            body->SetActive(false);
            body->SetAwake(false);
            body->SetGravityScale(0);
        } else {
            fixtureDef.density = 1.0f;   
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 1.0f;
            fixtureDef.isSensor = false;
            fixtureDef.filter.maskBits = MASK_BITS[TYPE_BONUS] + CAT_BITS[TYPE_GROUND];
            fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BONUS];
            fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BONUS];
            
            body = world->CreateBody(&bd);
            body->CreateFixture(&fixtureDef);
            body->SetTransform(bd.position, 0);
            body->SetActive(false);
            body->SetAwake(false);
        }
        
//        this->isInBox = isInBox;
        
        isEaten = false;
        
        sGlobal->total_gold += gold;
//        LOGE("total gold:%d", sGlobal->total_gold);
        isInitToShow = false;
    }
    
    ~BonusCoin() {
        
    }
    
    void update(float dt) {
        if (!isInitToShow) {
            isInitToShow = true;
            show();
            sGlobal->mainGameLayer->addToEnable(body);
            return;
        }
        
        if (isEaten) {
            return;
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

        coinSprite->tick(dt);
        if (hasGravity) {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        }
    }
    
    static void onBonusMoveUpEnded(wyAction* action, void* data) {
        BonusCoin* bonus = (BonusCoin*)data;
        wyMoveBy* m = wyMoveBy::make(1.0f, 0, DP(6));
        wyIntervalAction* t = wySequence::make(m, (wyFiniteTimeAction*)m->reverse(), NULL);
        wyAction* a = wyRepeatForever::make(t);
        bonus->container->runAction(a);
    }
    
    void show() {
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container);
        container->setPosition(orgx, orgy);
        
        coinSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        coinSprite->setForceTickMode(true);
        coinSprite->setPosition(0, 0);
        coinSprite->setVisible(true);
        container->addChildLocked(coinSprite);
        
        guang = NULL;
        switch (type) {
            case COIN:
                coinSprite->playAnimation(SPX_COIN_SMALL);
                coinSprite->setScaleX(sGlobal->scaleX*0.7f);
                coinSprite->setScaleY(sGlobal->scaleY*0.7f);
                break;
            case BIGCOIN: {
                coinSprite->playAnimation(SPX_COIN_MIDDLE);
                coinSprite->setScaleX(sGlobal->scaleX*1.0f);
                coinSprite->setScaleY(sGlobal->scaleY*1.0f);
                coinSprite->setUnitInterval(0.2f);
                coinSprite->setForceTickMode(true);
                coinSprite->setLoopCount(-1);
//                coinSprite->setPaused(true);

                guang = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
                guang->setForceTickMode(true);
                guang->setUnitInterval(0.1f);
                guang->setLoopCount(0);
                guang->playAnimation(SPX_DILEI_LIGHT_1);
                guang->setColor(wyc3b(234, 219, 154));
                container->addChildLocked(guang, -1);
                
                float fs = sGlobal->scaleX*1.0f;
                float ts = sGlobal->scaleX*1.4f;
                guang->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fs, fs, ts, ts), wyScaleTo::make(0.6f, ts, ts, fs, fs), NULL)));
                guang->runAction(wyRepeatForever::make(wyRotateBy::make(2.0f, 360)));
                break;
            }
            case BIGBIGCOIN: {
                coinSprite->playAnimation(SPX_COIN_BIG);
                coinSprite->setScaleX(sGlobal->scaleX*1.3f);
                coinSprite->setScaleY(sGlobal->scaleY*1.3f);
                coinSprite->setUnitInterval(0.2f);
                coinSprite->setForceTickMode(true);
                coinSprite->setLoopCount(-1);
//                coinSprite->setPaused(true);
                
                guang = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
                guang->setForceTickMode(true);
                guang->setUnitInterval(0.1f);
                guang->setLoopCount(0);
                guang->playAnimation(SPX_DILEI_LIGHT_1);
                container->addChildLocked(guang, -1);
                guang->setColor(wyc3b(234, 219, 154));
                float fs = sGlobal->scaleX*1.6f;
                float ts = sGlobal->scaleX*2.0f;
                guang->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fs, fs, ts, ts), wyScaleTo::make(0.6f, ts, ts, fs, fs), NULL)));
                guang->runAction(wyRepeatForever::make(wyRotateBy::make(2.0f, 360)));
                break;
            }
            default:
                break;
        }
        
        if (!hasGravity) {
            wyMoveBy* m = wyMoveBy::make(1.0f, 0, DP(8)*sGlobal->scaleY);
            wyIntervalAction* t = wySequence::make(m, (wyFiniteTimeAction*)m->reverse(), NULL);
            wyAction* action = wyRepeatForever::make(t);
            container->runAction(action);
        }
    }
    
    void beEaten() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        isEaten = true;
        sGlobal->playEffect(se_coin);
        sGlobal->mainGameLayer->addToDestroy(body);
        
        sGlobal->level_gold_normal += this->gold;
        
        int finalPlusGold = this->gold * (1+sGlobal->skillMulti[SKILL_GREED]);
        if (sGlobal->isDoubleCoin) {
            finalPlusGold *= 2;
        }
        sGlobal->hudLayer->addGold(finalPlusGold);
        sGlobal->level_gold += finalPlusGold;
        sGlobal->level_total_gold += finalPlusGold;
        sGlobal->totalCollectedCoins += finalPlusGold;
        
        sGlobal->effectLayer->showEatCoinEffect(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y), finalPlusGold, type);
        
        sGlobal->hudLayer->hasCollectCoin = true;
        
        if (sGlobal->isRealEndless) {
            for (int i = 0; i < 5; i++) {
                int achvNo = ACHV_COIN_1000+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//                    if (achvNo == ACHV_COIN_50000 && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                        continue;
//                    }
//                    if (achvNo == ACHV_COIN_100000 && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                        continue;
//                    }
                    
                    sGlobal->achv_progress[achvNo] += finalPlusGold;//this->gold;
                    if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
            
            for (int i = 0; i < 5; i++) {
                int achvNo = ACHV_COIN_400_ONE+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//                    if (achvNo == ACHV_COIN_3200_ONE && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                        continue;
//                    }
//                    if (achvNo == ACHV_COIN_4800_ONE && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                        continue;
//                    }
                    
                    sGlobal->achv_progress_temp[achvNo]+= finalPlusGold;//this->gold;
                    if (sGlobal->achv_progress_temp[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
        }
    }
    
    void destroyBonus() {
//        LOGE("destroy coin...");
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        coinSprite->stopAllActions();
        coinSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, coinSprite);
        
        if (guang != NULL) {
            guang->stopAllActions();
            guang->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, guang);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        world->DestroyBody(body);
        delete this;
    }
    
    bool isEaten;
};

class BonusExtra : public Bonus {
    
public:
    wySPXSprite *guang;
    wySpriteEx *bonusSprite;
    float orgx, orgy;
    bool isInitToShow;
    
    static BonusExtra *make(float x, float y, int type, bool isInBox) {
        return new BonusExtra(x, y, type, isInBox, false);
    }
    
    static BonusExtra *make(float x, float y, int type, bool isInBox, bool hasGravity) {
        return new BonusExtra(x, y, type, isInBox, hasGravity);
    }
    
    BonusExtra(float x, float y, int type, bool isInBox, bool hasGravity) : Bonus(){
        this->hasGravity = hasGravity;
        this->type = type;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        switch (this->type) {
            case BONUS_BIAO:
//                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "bonus_biao.png");
                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "bonus_biao.png");//wySpriteEx::make(MyTextureManager::makePNG("bonus_biao.png"));
                break;
            case BONUS_YING:
//                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "bonus_ying.png");
                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "bonus_ying.png");//wySpriteEx::make(MyTextureManager::makePNG("bonus_ying.png"));
                break;
            case BONUS_XIEZI:
//                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "bonus_xiezi.png");
                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "bonus_xie.png");//wySpriteEx::make(MyTextureManager::makePNG("bonus_xie.png"));
                break;
            case BONUS_XUE:
//                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "bonus_xue.png");
                bonusSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "bonus_xue.png");//wySpriteEx::make(MyTextureManager::makePNG("bonus_xue.png"));
                break;
            default:
                break;
        }
        bonusSprite->setScaleX(sGlobal->scaleX);
        bonusSprite->setScaleY(sGlobal->scaleY);
        bonusSprite->retain();
        
        obj_type = TYPE_BONUS;
//        container->setVisible(false);
        
        float ofy;
        ofy = 80;
        orgx = x;
        orgy = y + DP(80)*sGlobal->scaleY;
        
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + DP(ofy)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape dynamicBox;
        float wd = m_box2d->pixel2Meter(DP(14) * sGlobal->scaleX);
        dynamicBox.SetAsBox(wd, m_box2d->pixel2Meter(DP(16)*sGlobal->scaleY));	
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        
        if (!hasGravity) {
            fixtureDef.density = 0.0f;   
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.isSensor = true;
            fixtureDef.filter.maskBits = MASK_BITS[TYPE_BONUS];
            fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BONUS];
            fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BONUS];
            
            body = world->CreateBody(&bd);
            body->CreateFixture(&fixtureDef);
            body->SetTransform(bd.position, 0);
            body->SetActive(false);
            body->SetAwake(false);
            body->SetGravityScale(0);
        } else {
            fixtureDef.density = 1.0f;   
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 1.0f;
            fixtureDef.isSensor = false;
            fixtureDef.filter.maskBits = MASK_BITS[TYPE_BONUS];
            fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BONUS];
            fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BONUS];
            
            body = world->CreateBody(&bd);
            body->CreateFixture(&fixtureDef);
            body->SetTransform(bd.position, 0);
            body->SetActive(false);
            body->SetAwake(false);
        }
        
        this->isInBox = isInBox;
        gold = 0;
        
        isInitToShow = false;
    }
    
    ~BonusExtra() {
        
    }
    
    void update(float dt) {
        if (!isInitToShow) {
            isInitToShow = true;
            show();
            sGlobal->mainGameLayer->addToEnable(body);
            return;
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        if (hasGravity) {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        }
    }
    
    static void onBonusMoveUpEnded(wyAction* action, void* data) {
        BonusExtra* bonus = (BonusExtra*)data;
        wyMoveBy* m = wyMoveBy::make(1.0f, 0, DP(6));
        wyIntervalAction* t = wySequence::make(m, (wyFiniteTimeAction*)m->reverse(), NULL);
        wyAction* a = wyRepeatForever::make(t);
        bonus->container->runAction(a);
    }
    
    void show() {
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container);
        container->setPosition(orgx, orgy);
        container->addChildLocked(bonusSprite);
        bonusSprite->release();
        
        guang = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
        guang->setForceTickMode(true);
        guang->setUnitInterval(0.1f);
        guang->setLoopCount(0);
        guang->playAnimation(SPX_DILEI_LIGHT_1);
        container->addChildLocked(guang, -1);
        float fs = sGlobal->scaleX*1.6f;
        float ts = sGlobal->scaleX*2.0f;
        guang->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fs, fs, ts, ts), wyScaleTo::make(0.6f, ts, ts, fs, fs), NULL)));
        guang->runAction(wyRepeatForever::make(wyRotateBy::make(1.2f, 360)));
        
        if (!hasGravity) {
            container->setVisible(true);
            
            if (isInBox) {
                wyIntervalAction *a = wyMoveBy::make(0.3f, 0, DP(30)*sGlobal->scaleY);
                wyActionCallback callback = {
                    NULL,
                    onBonusMoveUpEnded
                };
                a->setCallback(&callback, this);
                container->runAction(a);
            } else {
                wyMoveBy* m = wyMoveBy::make(1.0f, 0, DP(6));
                wyIntervalAction* t = wySequence::make(m, (wyFiniteTimeAction*)m->reverse(), NULL);
                wyAction* action = wyRepeatForever::make(t);
                container->runAction(action);
            }
        } else {
            container->setVisible(true);
        }
    }
    
    void beEaten() {
        if (sGlobal->mainGameLayer->hero->isInDead) {
            return;
        }
        
        sGlobal->mainGameLayer->addToDestroy(body);
        sGlobal->mainGameLayer->hero->doBonus(this->type);
    }
    
    void destroyBonus() {
//        LOGE("destroy bonus...");
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();

        if (guang != NULL) {
            guang->stopAllActions();
            guang->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, guang);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        world->DestroyBody(body);
        delete this;
    }
};
