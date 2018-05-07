//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "LoadingScene.h"
#include "Global.h"
#include "MainScene.h"
#include "CGScene.h"
#include "UpgradeScene.h"
#include "WorldMapScene.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#include "ObjCCalls.h"
#endif

LoadingScene::LoadingScene(int sceneId, int mapIndex) {
    sGlobal->stopBGM();

    sGlobal->needPauseGame = false;
    
    this->sceneId = sceneId;
    this->mapIndex = mapIndex;
    m_busying = false;

    int seed = rand()%3;
    int animIdx;
    char spxName[30];
    char spxImg[30];
    switch (seed) {
        case 0:
            animIdx = ENEMY_0_TIP[rand()%7];
            sprintf(spxName, "enemy_0.sprite");
            sprintf(spxImg, "enemy_0.png");
            break;
        case 1:
            animIdx = ENEMY_1_TIP[rand()%8];
            sprintf(spxName, "enemy_1.sprite");
            sprintf(spxImg, "enemy_1.png");
            break;
        case 2:
            animIdx = ENEMY_2_TIP[rand()%7];
            sprintf(spxName, "enemy_2.sprite");
            sprintf(spxImg, "enemy_2.png");
            break;
        default:
            break;
    }
    loading = wySPXSprite::make(spxName, false, MyTextureManager::makePNG(spxImg), animIdx);
    loading->setForceTickMode(true);
    loading->setUnitInterval(0.2f);
    loading->setLoopCount(-1);
    addChildLocked(loading);
    loading->setPosition(wyDevice::winWidth/2 + DP(0)*sGlobal->scaleX, wyDevice::winHeight/2-DP(0)*sGlobal->scaleY);
    loading->setScaleX(sGlobal->scaleX*0.6f);
    loading->setScaleY(sGlobal->scaleY*0.6f);
//    loading->playAnimation(rand()%loading->getAnimationCount());

    int tipcount = sizeof(TIPS_STR)/sizeof(TIPS_STR[0]);
//    LOGE("tip count:%d", tipcount);
    wyBitmapFontLabel *tip = wyBitmapFontLabel::make(sGlobal->font_desc, TIPS_STR[rand()%tipcount]);
    tip->setScale(DP(13)*sGlobal->scaleX/sGlobal->descFontSize);
//    LOGE("scale:%f,%f", tip->getScale(),12*sGlobal->scaleX/sGlobal->descFontSize);
    tip->setAlignment(wyBitmapFontLabel::LEFT);
    tip->setPosition(loading->getPositionX(), loading->getPositionY()-DP(80)*sGlobal->scaleY);
    addChildLocked(tip);
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(LoadingScene::updateLogic));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);

    setKeyEnabled(false);
}

LoadingScene::~LoadingScene() {
//    delete loadingResId;
//    loadingResId = NULL;
}

void LoadingScene::updateLogic(wyTargetSelector* ts) {
    loading->tick(ts->getDelta());
}

void LoadingScene::onEnterTransitionDidFinish() {
    wyNode::onEnterTransitionDidFinish();
#if IOS
    ObjCCalls::getInstance()->hideAd();
#endif

#if ANDROID
    hideAd();
#endif
    
    if(m_busying)
        return;
    m_busying = true;
    
    wyThread::runThread(wyTargetSelector::make(this, SEL(LoadingScene::loadingThreadEntry)));
}

void LoadingScene::updateLoadHintLabel(int index) {

}

void LoadingScene::loadingThreadEntry(wyTargetSelector* sel) {
    wyTexture2D* tex;

    loadEffects(sceneId);
    
    sGlobal->loadAllImages();
    
    if (sceneId == SCENE_MAIN) {
        if (sGlobal->isRealEndless) {
            sGlobal->initEndlessMap(sGlobal->currentEModeDifficulty);
            mapIndex = sGlobal->currentSeasonId*100;
            sGlobal->generateTileMapCache();

        } else {
            sGlobal->isBoostOn = false;
            sGlobal->boostMulti = 1.0f;
            sGlobal->heroVelocityX = HERO_VEL_X;
        }

        wyScene *scene = new MainScene(mapIndex);
        scene->autoRelease();
        scene->retain();
        wyTimer *timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(LoadingScene::gotoNextScene), scene), 2.0f);
        timer->setOneShot(true);
        scheduleLocked(timer);
    }
    else if (sceneId == SCENE_UPGRADE) {
        
        wyScene *scene = new UpgradeScene(mapIndex);
        scene->autoRelease();
        scene->retain();
        wyTimer *timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(LoadingScene::gotoNextScene), scene), 0.8f);
        timer->setOneShot(true);
        scheduleLocked(timer);
    }
    else if (sceneId == SCENE_WORLD_MAP) {

        wyScene *scene = new WorldMapScene(false);
        scene->autoRelease();
        scene->retain();
        wyTimer *timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(LoadingScene::gotoNextScene), scene), 2.0f);
        timer->setOneShot(true);
        scheduleLocked(timer);
    }
    else if (sceneId == SCENE_MENU) {
        wyScene *scene = new CGScene(false, false);
        scene->autoRelease();
        scene->retain();
        wyTimer *timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(LoadingScene::gotoNextScene), scene), 0.8f);
        timer->setOneShot(true);
        scheduleLocked(timer);
    }
}

void LoadingScene::gotoNextScene(wyTargetSelector* ts, void *data) {
    loading->setPaused(true);
    
#if ANDROID
    callGC();
#endif
    
    wyScene *scene = (wyScene *)data;
    wyUtils::runOnGLThread(GotoSceneRunnable::make(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0))));
    m_busying = false;
    scene->release();
}

void LoadingScene::loadEffects(int sceneId) {

}
