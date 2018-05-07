//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "EffectLayer.h"
#include "Global.h"
#include "Intro.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Boss3.h"

static void onAnimationEnded(wyAnimation* anim, void* data) {
    EffectLayer* layer = (EffectLayer*)data;
    layer->hurtSprite->setVisible(false);
    layer->isShowingHurtAnim = false;
}

EffectLayer::EffectLayer(int mapIndex) {
    sGlobal->effectLayer = this;

    isShowingHurtAnim = false;

    this->setTouchEnabled(false);

    hurtNode = wySpriteBatchNode::make(MyTextureManager::makePNG("dialog_deco.png"));
    hurtNode->setPosition(0, 0);
    addChildLocked(hurtNode);
    hurtNode->setVisible(false);

//	wySpriteEx *hurt_top = wyZwoptexManager::getInstance()->makeSpriteEx("main_game_01", "role_hurt_top.png");
    wySpriteEx *hurt_top = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "penxue.png");
	hurtNode->addChildLocked(hurt_top);
	hurt_top->setScaleX((float)wyDevice::winWidth/hurt_top->getContentSize().width);
	hurt_top->setScaleY(DP(20)*sGlobal->scaleY/hurt_top->getContentSize().height);
	hurt_top->setAnchor(0,1.0f);
	hurt_top->setPosition(0, wyDevice::winHeight);
    hurt_top->setVisible(false);

//	wySpriteEx *hurt_bottom = wyZwoptexManager::getInstance()->makeSpriteEx("main_game_01", "role_hurt_bottom.png");
    wySpriteEx *hurt_bottom = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "penxue.png");
	hurtNode->addChildLocked(hurt_bottom);
	hurt_bottom->setScaleX(hurt_top->getScaleX());
	hurt_bottom->setScaleY(hurt_top->getScaleY());
	hurt_bottom->setAnchor(0,0);
	hurt_bottom->setPosition(0, 0);
    hurt_bottom->setFlipY(true);
    hurt_bottom->setVisible(false);

//	wySpriteEx *hurt_left = wyZwoptexManager::getInstance()->makeSpriteEx("main_game_01", "role_hurt_left.png");
    wySpriteEx *hurt_left = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "penxue.png");
    hurt_left->setRotation(-90);
	hurtNode->addChildLocked(hurt_left);
	hurt_left->setScaleX((wyDevice::winHeight-DP(0)*sGlobal->scaleY)/hurt_left->getContentSize().width);
	hurt_left->setScaleY(DP(50)*sGlobal->scaleX/hurt_left->getContentSize().height);
	hurt_left->setAnchor(0.5f,1.0f);
	hurt_left->setPosition(0, wyDevice::winHeight/2);
//    hurt_left->setPosition(100, 100);

//	wySpriteEx *hurt_right = wyZwoptexManager::getInstance()->makeSpriteEx("main_game_01", "role_hurt_right.png");
    wySpriteEx *hurt_right = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "penxue.png");
    hurt_right->setRotation(90);
	hurtNode->addChildLocked(hurt_right);
	hurt_right->setScaleX((wyDevice::winHeight-DP(0)*sGlobal->scaleY)/hurt_right->getContentSize().width);
	hurt_right->setScaleY(DP(50)*sGlobal->scaleX/hurt_right->getContentSize().height);
	hurt_right->setAnchor(0.5f,1.0f);
	hurt_right->setPosition(wyDevice::winWidth, wyDevice::winHeight/2);
    
    isEffecting = false;
    
//    hurtNode = wySpriteEx::make(MyTextureManager::makePNG("allscreenblood.png"));
//    hurtNode->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
//    hurtNode->setContentSize(wyDevice::winWidth, wyDevice::winHeight);
//    hurtNode->setAutoFit(true);
//    addChildLocked(hurtNode);
//    hurtNode->setVisible(false);

//    emitter = wyParticleLoader::load("particle_leaf1.plist");
//    //            emitter->setStartColorVariance(51, 51, 37, 255, 0, 0, 0, 0);
//    emitter->setParticlePositionVariance(0, 0, 0, wyDevice::winHeight/2);
//    addChildLocked(emitter, 200);
//    emitter->setPositionType(GROUPED);
//    emitter->setPosition(wyDevice::winWidth+DP(40)*sGlobal->scaleX, 0);
//    emitter->setScaleX(sGlobal->scaleX);
//    emitter->setScaleY(sGlobal->scaleY);
//    emitter->setEmissionRate(3);
//    emitter->setDuration(randRange(2, 5));
//    
//    emitterTimer = 0;
//    emitterInterval = emitter->getDuration()+randRange(3, 6);
    
    this->bgIndex = mapIndex /100;
    if (bgIndex == 2) {
        batContainer = wyNode::make();
        batContainer->setPosition(wyDevice::winWidth*1.2f, wyDevice::winHeight-DP(200)*sGlobal->scaleY);
        addChildLocked(batContainer);
        
        bat1 = getBatSprite(1.0f, 0, 0);
        bat2 = getBatSprite(0.8f, 30, 10);
        bat3 = getBatSprite(0.8f, -30, 10);
        bat4 = getBatSprite(0.7f, 60, 20);
        bat5 = getBatSprite(0.7f, -60, 20);
        bat6 = getBatSprite(0.9f, 5, 30);
        
        onBatFlyEnd(NULL);
    }
    
    if (bgIndex == 3) {
        cloudContainer = wyNode::make();
        cloudContainer->setPosition(wyDevice::winWidth*1.2f, 0);
        addChildLocked(cloudContainer);
        
        c1 = getCloudSprite(3.0f, 0, 0);
        c2 = getCloudSprite(3.2f, 150, 50);
        c3 = getCloudSprite(3.2f, 300, 20);
        
        onCloudMoveEnd(NULL);
    }
    
    if (bgIndex == 1 || bgIndex == 2) {
        emitter = wyParticleLoader::load("particle_snow_slow.plist");
        emitter->setParticlePositionVariance(0, 0, wyDevice::winWidth, 0);
        addChildLocked(emitter, 3);
        emitter->setPosition(wyDevice::winWidth*0.2f, wyDevice::winHeight*2);
        emitter->setPositionType(GROUPED);
        emitter->setScaleX(sGlobal->scaleX);
        emitter->setScaleY(sGlobal->scaleY);
        emitter->setLifeVariance(4.0, 0);
        emitter->setStartSizeVariance(30, 20);
    }
}

EffectLayer::~EffectLayer() {
    
}

wySPXSprite* EffectLayer::getBatSprite(float sx, float posx, float posy) {
    wySPXSprite *bat = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), 7);
    bat->setForceTickMode(true);
    bat->setUnitInterval(0.16f);
    bat->setScaleX(sx*sGlobal->scaleX);
    bat->setScaleY(sx*sGlobal->scaleY);
    bat->setPosition(DP(posx)*sGlobal->scaleX, DP(posy)*sGlobal->scaleY);
    batContainer->addChildLocked(bat, 11);
    bat->setLoopCount(-1);
    bat->setColor(wyc3b(0, 0, 0));
    
    return bat;
}

void EffectLayer::onBatFlyEnd(wyTargetSelector* ts){
    int seed = rand()%3;
    switch (seed) {
        case 0:
            batContainer->setPosition(wyDevice::winWidth*1.2f, wyDevice::winHeight-DP(randRange(250, 350))*sGlobal->scaleY);
            batContainer->stopAllActions();
            batContainer->runAction(wySequence::make(wyDelayTime::make(randRange(5, 10)), wyShow::make(), wyMoveBy::make(randRange(10, 15)*0.1f, -wyDevice::winWidth, DP(400)*sGlobal->scaleY), wyCallFunc::make(wyTargetSelector::make(this, SEL(EffectLayer::onBatFlyEnd))), NULL));
            break;
        case 1:
            batContainer->setPosition(wyDevice::winWidth-DP(randRange(400, 600)*sGlobal->scaleX), wyDevice::winHeight+DP(50)*sGlobal->scaleY);
            batContainer->stopAllActions();
            batContainer->runAction(wySequence::make(wyDelayTime::make(randRange(5, 10)), wyShow::make(), wyMoveBy::make(randRange(10, 15)*0.1f, wyDevice::winWidth, -DP(450)*sGlobal->scaleY), wyCallFunc::make(wyTargetSelector::make(this, SEL(EffectLayer::onBatFlyEnd))), NULL));
            break;
        case 2:
            batContainer->setPosition(-DP(100)*sGlobal->scaleX, wyDevice::winHeight-DP(randRange(300,400))*sGlobal->scaleY);
            batContainer->stopAllActions();
            batContainer->runAction(wySequence::make(wyDelayTime::make(randRange(5, 10)), wyShow::make(), wyMoveBy::make(randRange(10, 15)*0.1f, wyDevice::winWidth, DP(500)*sGlobal->scaleY), wyCallFunc::make(wyTargetSelector::make(this, SEL(EffectLayer::onBatFlyEnd))), NULL));
            break;
        default:
            break;
    }
    batContainer->setVisible(false);
}

wySPXSprite * EffectLayer::getCloudSprite(float sx, float posx, float posy) {
    wySPXSprite *cloud = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), rand()%2+5);
    cloud->setForceTickMode(true);
    cloud->setUnitInterval(0.16f);
    cloud->setScaleX(sx*sGlobal->scaleX);
    cloud->setScaleY(sx*sGlobal->scaleY);
    cloud->setPosition(DP(posx)*sGlobal->scaleX, DP(posy)*sGlobal->scaleY);
    cloudContainer->addChildLocked(cloud, 11);
    cloud->setLoopCount(-1);
    
    return cloud;
}

void EffectLayer::onCloudMoveEnd(wyTargetSelector* ts) {
    cloudContainer->setPosition(wyDevice::winWidth*1.2f, DP(randRange(20, 150))*sGlobal->scaleY);
    
    c2->setPosition(DP(randRange(100, 200))*sGlobal->scaleX, DP(randRange(0, 100))*sGlobal->scaleY);
    c3->setPosition(DP(randRange(200, 400))*sGlobal->scaleX, DP(randRange(0, 100))*sGlobal->scaleY);
    
    c1->setRotation(randRange(0, 45));
    c2->setRotation(randRange(0, 45));
    c3->setRotation(randRange(0, 45));
}

void EffectLayer::showHurtAnim() {
 //   if (isShowingHurtAnim)
 //       return;

 //   hurtSprite->setVisible(true);
 //   wyIntervalAction *a = wySequence::make(wyDelayTime::make(1.0f), wyHide::make(), NULL);
	//hurtSprite->runAction(a);

	if (isShowingHurtAnim)
        return;

    hurtNode->setVisible(true);
    wyIntervalAction *a = wySequence::make(wyDelayTime::make(1.0f), wyHide::make(), NULL);
	hurtNode->runAction(a);
}

static void onCoinEmitterEnded(wyAction* action, void* data) {
//    LOGE("hide coin emitter...");
    wyParticleSystem* emitter = (wyParticleSystem*)data;
    
    emitter->setVisible(false);
    emitter->stopSystem();
    emitter->stopAllActions();
//    emitter->resetSystem();
    
    sGlobal->pushToCache(sGlobal->coin_emitterArray, emitter);
}

static void onEnemyDeadCoinAnimEnded(wyAction* action, void* data) {
//    LOGE("onEnemyDeadCoinAnimEnded...");
    sGlobal->mainGameLayer->removeChildLocked((wyNode*)data, true);
}

void EffectLayer::showEatCoinEffect(float x, float y, int gold, int type) {
//    LOGE("coin cache len:%d", sGlobal->coin_emitterArray->num);
    
    wyParticleSystem *coinEmitter = (wyParticleSystem *) sGlobal->popFromCache(sGlobal->coin_emitterArray);
    
//    wyParticleSystem *coinEmitter = wyParticleLoader::load("particle_coin.plist");
//    coinEmitter->setAutoRemoveOnFinish(true);
    
    coinEmitter->setVisible(true);
    coinEmitter->setScaleX(sGlobal->scaleX);
    coinEmitter->setScaleY(sGlobal->scaleY);
    coinEmitter->resetSystem();
    addChildLocked(coinEmitter);
    coinEmitter->setPosition(x + sGlobal->mainGameLayer->getPositionX(), y + sGlobal->mainGameLayer->getPositionY());
    
    wyIntervalAction *a = wyDelayTime::make(1.0f);
    wyActionCallback callback = {
        NULL,
        onCoinEmitterEnded,
        NULL
    };
    a->setCallback(&callback, coinEmitter);
    coinEmitter->runAction(a);
    
    
    if (type != COIN) {
        wyNode *deadCoinContainer = wyNode::make();
        deadCoinContainer->setPosition(x-DP(0)*sGlobal->scaleX, y+DP(0)*sGlobal->scaleY);
        sGlobal->mainGameLayer->addChildLocked(deadCoinContainer, 103);
        
        wySpriteEx *deadCoin = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
        deadCoin->setScaleX(0.7f*sGlobal->scaleX);
        deadCoin->setScaleY(0.7f*sGlobal->scaleY);
        deadCoin->setPosition(DP(0)*sGlobal->scaleX, 0);
        deadCoinContainer->addChildLocked(deadCoin);
        
        char temp[5];
        sprintf(temp, "+%d", gold);
        wyBitmapFontLabel *deadCoinTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        deadCoinTitle->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
        deadCoinTitle->setAlignment(wyBitmapFontLabel::LEFT);
        deadCoinTitle->setColor(wyc3b(255, 189, 0));
        deadCoinTitle->setAnchor(0, 0.5f);
        deadCoinTitle->setPosition(DP(20)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
        deadCoinContainer->addChildLocked(deadCoinTitle);
        
        wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(1.0f, 0, DP(40)*sGlobal->scaleY), NULL);
        wyActionCallback callback = {
            NULL,
            onEnemyDeadCoinAnimEnded
        };
        action->setCallback(&callback, deadCoinContainer);
        deadCoinContainer->runAction(action);
        deadCoinTitle->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
        deadCoin->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
    }

}

void EffectLayer::reset() {
    
}

void EffectLayer::showDeadCoinEffect(float x, float y, int gold) {
//    wyNode *deadCoinContainer = wyNode::make();
//    deadCoinContainer->setPosition(x-DP(36)*sGlobal->scaleX, y+DP(70)*sGlobal->scaleY);
//    sGlobal->mainGameLayer->addChildLocked(deadCoinContainer, 103);
//    
//    wySpriteEx *deadCoin = wySpriteEx::make(MyTextureManager::makePNG("coin_icon.png"));
//    deadCoin->setScaleX(0.7f*sGlobal->scaleX);
//    deadCoin->setScaleY(0.7f*sGlobal->scaleY);
//    deadCoin->setPosition(DP(0)*sGlobal->scaleX, 0);
//    deadCoinContainer->addChildLocked(deadCoin);
//    
//    char temp[5];
//    sprintf(temp, "+%d", gold);
//    wyBitmapFontLabel *deadCoinTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
//    deadCoinTitle->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
//    deadCoinTitle->setAlignment(wyBitmapFontLabel::LEFT);
////    deadCoinTitle->setColor(goldColor);
//    deadCoinTitle->setAnchor(0, 0.5f);
//    deadCoinTitle->setPosition(DP(20)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
//    deadCoinContainer->addChildLocked(deadCoinTitle);
//    
//    wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(1.0f, 0, DP(40)*sGlobal->scaleY), NULL);
//    wyActionCallback callback = {
//        NULL,
//        onEnemyDeadCoinAnimEnded
//    };
//    action->setCallback(&callback, deadCoinContainer);
//    deadCoinContainer->runAction(action);
//    deadCoinTitle->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
//    deadCoin->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
}

static void onEnemyDeadZuanShiAnimEnded(wyAction* action, void* data) {
    sGlobal->mainGameLayer->removeChildLocked((wyNode*)data, true);
}

void EffectLayer::showDeadZuanShiEffect(float x, float y, int zuanshi) {
//    wyNode *deadZuanshiContainer = wyNode::make();
//    deadZuanshiContainer->setPosition(x-DP(36)*sGlobal->scaleX, y+DP(70)*sGlobal->scaleY);
//    sGlobal->mainGameLayer->addChildLocked(deadZuanshiContainer, 103);
//    
//    wySpriteEx *deadZuanshi = wySpriteEx::make(MyTextureManager::makePNG("cash_icon.png"));
//    deadZuanshi->setScaleX(0.7f*sGlobal->scaleX);
//    deadZuanshi->setScaleY(0.7f*sGlobal->scaleY);
//    deadZuanshi->setPosition(DP(0)*sGlobal->scaleX, 0);
//    deadZuanshiContainer->addChildLocked(deadZuanshi);
//    
//    char temp[5];
//    sprintf(temp, "+%d", zuanshi);
//    wyBitmapFontLabel *deadZuanshiTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
//    deadZuanshiTitle->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
//    deadZuanshiTitle->setAlignment(wyBitmapFontLabel::LEFT);
////    deadZuanshiTitle->setColor(zuanshiColor);
//    deadZuanshiTitle->setPosition(DP(20)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
//    deadZuanshiTitle->setAnchor(0, 0.5f);
//    deadZuanshiContainer->addChildLocked(deadZuanshiTitle);
//    
//    wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(1.0f, 0, DP(40)*sGlobal->scaleY), NULL);
//    wyActionCallback callback = {
//        NULL,
//        onEnemyDeadZuanShiAnimEnded
//    };
//    action->setCallback(&callback, deadZuanshiContainer);
//    deadZuanshiContainer->runAction(action);
//    deadZuanshiTitle->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
//    deadZuanshi->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
}

void EffectLayer::offsetBy(float dx, float dy) {
//    if (emitter != NULL) {
//        emitter->setPosition(emitter->getPositionX(), wyDevice::winHeight + dy*0.9f);
//    }
//    batContainer->translate(MIN(0.5f, dx*10), 0);
    
//    batContainer->setPosition(batContainer->getPositionX(), wyDevice::winHeight-DP(200)*sGlobal->scaleY+dy*0.9f);
    if (bgIndex == 3) {
        cloudContainer->translate(dx*1.5f, 0);
        cloudContainer->setPosition(cloudContainer->getPositionX(), dy*1.0f);
//        LOGE("cloud:%f", cloudContainer->getPositionX());
        if (cloudContainer->getPositionX() < -wyDevice::winWidth) {
            onCloudMoveEnd(NULL);
        }
    }
    
    if (bgIndex == 1 || bgIndex == 2) {
//        emitter->translate(dx, 0);
        emitter->setPosition(emitter->getPositionX(), wyDevice::winHeight*2+dy*1.0f);
    }
}

void EffectLayer::updateTimer(float dt) {
//    emitterTimer += dt;
//    if (emitterTimer >= emitterInterval) {
//        emitter->setDuration(randRange(2,5));
//        emitter->resetSystem();
//        
//        emitterTimer = 0;
//        emitterInterval = emitter->getDuration() + randRange(3, 6);
//    }
    if (bgIndex == 2 && batContainer->isVisible()) {
        bat1->tick(dt);
        bat2->tick(dt);
        bat3->tick(dt);
        bat4->tick(dt);
        bat5->tick(dt);
        bat6->tick(dt);
    }
}

