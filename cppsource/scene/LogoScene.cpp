//
//  LogoScene.cpp
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "LogoScene.h"
#include "wyUtils.h"
#include "Global.h"
#include "CGScene.h"
#include "VungleVedioController.h"

static void onGamePaused(void* data) {
    if (sGlobal->needPauseGame && sGlobal->hudLayer != NULL) {
        sGlobal->hudLayer->onPauseBtnClick(NULL);
    }
}

LogoScene::LogoScene() {
    wyColorLayer *bg = wyColorLayer::make();
    bg->setColor(wyc4b(255, 255, 255, 255));
    addChildLocked(bg);
    
    wyTexture2D* tex1 = MyTextureManager::makeJPG("zendroid_logo.jpg");
    sprite1 = wySprite::make(tex1);
    sprite1->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    float sy = wyDevice::winHeight/sprite1->getHeight();
    sprite1->setScaleX(sy);
    sprite1->setScaleY(sy);
    addChildLocked(sprite1);
    sprite1->setVisible(true);
    
    setKeyEnabled(false);
//    wySPXSprite *langkoushui = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 0);
//    langkoushui->setForceTickMode(true);
//	langkoushui->setUnitInterval(0.15f);
//    langkoushui->setScaleX(1.0f*sGlobal->scaleX);
//    langkoushui->setScaleY(1.0f*sGlobal->scaleY);
//    addChildLocked(langkoushui);
//    langkoushui->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    
    static wyDirectorLifecycleListener dll = {
        NULL,
        NULL,
        NULL,
        onGamePaused,
        NULL,
        NULL,
        NULL,
    };
    wyDirector::getInstance()->addLifecycleListener(&dll, this);
    sGlobal->needPauseGame = false;
    
    VoteUsLayer::updatePlayTimes();
    VungleVedioController::init();
}

LogoScene::~LogoScene() {
    wyTextureManager::getInstance()->removeTexture("zendroid_logo.jpg");
}

void LogoScene::onEnter() {
    wyScene::onEnter();
    
    
}

void LogoScene::onEnterTransitionDidFinish() {
    wyScene::onEnterTransitionDidFinish();
    

#if ANDROID
    if (checkGamePackageName() != 1) {
        return;
    }
    
    if (checkSignature() != 1) {
        return;
    }
#endif
    
    sGlobal->initGame();
    sGlobal->loadAllImagesAsync(this);
    
//    wyAction * action = wySequence::make(
//                                         wyShow::make(),
//                                         wyDelayTime::make(0.5f),
//                                         wyCallFunc::make(wyTargetSelector::make(this, SEL(LogoScene::loadingCallBack))),
//                                         NULL);
//    sprite1->runAction(action);
}

void LogoScene::loadingCallBack(wyTargetSelector* ts){
    sGlobal->initGame();
    sGlobal->loadAllImagesAsync();
//    sGlobal->loadAllImages(); 
    
    wyAction * action = wySequence::make(wyDelayTime::make(0.5f),
                                         wyCallFunc::make(wyTargetSelector::make(this, SEL(LogoScene::loadingCallBack1))),
                                         NULL);
    sprite1->runAction(action);
}

void LogoScene::loadingCallBack1(wyTargetSelector* ts){
//    wyDirector* director = wyDirector::getInstance();
//    wyScene *scene = new GameIntroScene();
//    scene->autoRelease();
//    director->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));

//    wyDirector* director = wyDirector::getInstance();
//    wyScene *scene = new CGScene(true, false);
//    scene->autoRelease();
//    director->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));
}

void LogoScene::onLoadTexEnd() {
    wyDirector* director = wyDirector::getInstance();
    wyScene *scene = new CGScene(true, false);
    scene->autoRelease();
    director->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));
}



