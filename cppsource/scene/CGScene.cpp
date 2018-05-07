#include "CGScene.h"
#include "wyUtils.h"
#include "LoadingScene.h"
#include "Global.h"
#include "LevelSelectLayer.h"
#include "AllMenusLayer.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
//#include "DailyRewardLayer.h"

static void onFireEnded(wyAFCSprite* sprite, void* data) {
    CGScene *scene = (CGScene *) data;
    if (sprite->getCurrentAnimationIndex() == 1) {
        if (scene->currentLoopCount == 5) {
            sprite->setLoopCount(0);
            sprite->playAnimation(0);
            scene->isFiring = false;
            scene->fireCDTimer = 0;
        } else {
            scene->currentLoopCount+=1;
        }
    }
}

static void onFireChanged(wyAFCSprite* sprite, void* data) {
//    if (sprite->getCurrentFrame() == 1 || sprite->getCurrentFrame() == 3) {
//        sGlobal->playEffect("se_gun_gatling.mp3");
//    }
}

CGScene::CGScene(bool needPlayBGM, bool needUnlockNandu) {
    sp_homeBg = wySprite::make(MyTextureManager::makePNG("start_scene_bg.png"));
    sp_homeBg->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    sp_homeBg->setScaleX(sGlobal->scaleX*0.9f);
    sp_homeBg->setScaleY(sGlobal->scaleY*0.9f);
    addChildLocked(sp_homeBg, 0);
    
    wySPXSprite *bg_guang = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 11);//wySprite::make(MyTextureManager::makePNG("start_scene_fire.png"));
    bg_guang->setPosition(wyDevice::winWidth/2, DP(80)*sGlobal->scaleY);
    bg_guang->setScale(wyDevice::winWidth/bg_guang->getFrameRect().width);
//    bg_guang->setScaleY(sGlobal->scaleY*0.9f);
    addChildLocked(bg_guang, 0);
    bg_guang->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(2.0f, 255, 50), wyFadeTo::make(2.0f, 50, 255), NULL)));
    
    wyQuadParticleSystem *emitter = wyParticleLoader::load("particle_snow.plist");
    emitter->setParticlePositionVariance(0, 0, wyDevice::winWidth*1.5f, 0);
    addChildLocked(emitter, 0);
    emitter->setPosition(wyDevice::winWidth*0.2f, wyDevice::winHeight);
    emitter->setPositionType(GROUPED);
    emitter->setScaleX(sGlobal->scaleX);
    emitter->setScaleY(sGlobal->scaleY);
    emitter->setLifeVariance(1.0, 0);
    emitter->setEmissionRate(100);
    emitter->setStartSizeVariance(6, 6);
    
    emitter = wyParticleLoader::load("particle_snow_slow.plist");
    emitter->setParticlePositionVariance(0, 0, wyDevice::winWidth, 0);
    addChildLocked(emitter, 3);
    emitter->setPosition(wyDevice::winWidth*0.2f, wyDevice::winHeight);
    emitter->setPositionType(GROUPED);
    emitter->setScaleX(sGlobal->scaleX);
    emitter->setScaleY(sGlobal->scaleY);
    emitter->setLifeVariance(2.0, 0);
    emitter->setStartSizeVariance(20, 20);

//    emitter->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    
    float dur = 5.0f;
    float rot = 4;
    dimian_right = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 9);//wySprite::make(MyTextureManager::makePNG("dimian_right.png"));
    dimian_right->setPosition(wyDevice::winWidth/2+DP(0)*sGlobal->scaleX, DP(80)*sGlobal->scaleY);
    dimian_right->setScaleX(sGlobal->scaleX*0.9f);
    dimian_right->setScaleY(sGlobal->scaleY*0.9f);
//    dimian_right->setAnchor(1.0, 0);
    addChildLocked(dimian_right, 0);
    float r_w = dimian_right->getFrameRect().width;
    float r_h = dimian_right->getFrameRect().height;
    dimian_right->setPosition(wyDevice::winWidth/2+DP(-40)*sGlobal->scaleX+r_w*dimian_right->getScaleX(), DP(80)*sGlobal->scaleY-r_h*dimian_right->getScaleY());
    dimian_right->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f), wyRotateTo::make(dur, rot, 0), wyRotateTo::make(dur, 0, rot), NULL)));
    
    dimian_left = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 8);//wySprite::make(MyTextureManager::makePNG("dimian_left.png"));
    dimian_left->setScaleX(sGlobal->scaleX*0.85f);
    dimian_left->setScaleY(sGlobal->scaleY*0.85f);
//    dimian_left->setAnchor(0, 0);
    addChildLocked(dimian_left, 0);
    float l_w = dimian_left->getFrameRect().width;
    float l_h = dimian_left->getFrameRect().height;
    dimian_left->setPosition(wyDevice::winWidth/2+DP(65)*sGlobal->scaleX-l_w*dimian_left->getScaleX(), wyDevice::winHeight/2-DP(70)*sGlobal->scaleY-l_h*dimian_left->getScaleY());
    dimian_left->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f), wyRotateTo::make(dur, rot, 0), wyRotateTo::make(dur, 0, rot), NULL)));
    
    left = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 0);
    left->setForceTickMode(true);
	left->setUnitInterval(0.12f);
    left->setScaleX(1.0f*sGlobal->scaleX);
    left->setScaleY(1.0f*sGlobal->scaleY);
    addChildLocked(left, 2);
    left->setLoopCount(5);
    left->setPosition(wyDevice::winWidth/2-DP(205)*sGlobal->scaleX, wyDevice::winHeight/2-DP(70)*sGlobal->scaleY);
    wyAFCSpriteCallback callback = {
        onFireChanged,
        onFireEnded,
    };
    left->setAFCSpriteCallback(&callback, this);
    float fss = 1.0f*sGlobal->scaleX;
    float tss = 0.94f*sGlobal->scaleX;
//    left->runAction(wySequence::make(wySkewTo::make(3.0f, 0,0,fss,tss), wySkewTo::make(3.0f, 0, 0, tss, fss), NULL));
    left->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f),wyRotateTo::make(dur, 0, -4), wyRotateTo::make(dur, -4, 0), NULL)));
    left->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f),wyMoveBy::make(dur, DP(-6)*sGlobal->scaleX, DP(6)*sGlobal->scaleX), wyMoveBy::make(dur, DP(6)*sGlobal->scaleX, DP(-6)*sGlobal->scaleX), NULL)));
    left->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f),wyScaleTo::make(dur, fss,fss,fss,tss), wyScaleTo::make(dur, fss,tss,fss,fss), NULL)));
    
    isFiring = false;
    fireCDTimer = 0;

    rightContainer = wyNode::make();
    addChildLocked(rightContainer);
    rightContainer->setPosition(wyDevice::winWidth/2+DP(200)*sGlobal->scaleX, wyDevice::winHeight/2-DP(25)*sGlobal->scaleY);
    fss = 0.94f;
    tss = 1.0f;
    rightContainer->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f),wyRotateTo::make(dur, 4, 0), wyRotateTo::make(dur, 0, 4), NULL)));
    rightContainer->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f),wyMoveBy::make(dur, DP(-6)*sGlobal->scaleX, DP(-6)*sGlobal->scaleX), wyMoveBy::make(dur, DP(6)*sGlobal->scaleX, DP(6)*sGlobal->scaleX), NULL)));
    rightContainer->runAction(wyRepeatForever::make(wySequence::make(wyDelayTime::make(0.0f),wyScaleTo::make(dur, fss,fss,fss,tss), wyScaleTo::make(dur, fss,tss,fss,fss), NULL)));
    
    wySPXSprite *right = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 2);
    right->setScaleX(sGlobal->scaleX*1.0f);
    right->setScaleY(sGlobal->scaleY*1.0f);
    rightContainer->addChildLocked(right, 0);
    
    daoguang = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 3);
    daoguang->setForceTickMode(true);
	daoguang->setUnitInterval(0.1f);
    daoguang->setScaleX(1.0f*sGlobal->scaleX);
    daoguang->setScaleY(1.0f*sGlobal->scaleY);
    rightContainer->addChildLocked(daoguang, 0);
    daoguang->setLoopCount(0);
    daoguang->setPosition(right->getPositionX()+DP(114)*sGlobal->scaleX, right->getPositionY()+DP(180)*sGlobal->scaleY);
    daoguang->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    
    nvkoushui = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 4);
    nvkoushui->setForceTickMode(true);
	nvkoushui->setUnitInterval(0.4f);
    nvkoushui->setScaleX(1.0f*sGlobal->scaleX);
    nvkoushui->setScaleY(1.0f*sGlobal->scaleY);
    rightContainer->addChildLocked(nvkoushui, 0);
    nvkoushui->setLoopCount(0);
    nvkoushui->setPosition(right->getPositionX()-DP(50)*sGlobal->scaleX, right->getPositionY()-DP(6)*sGlobal->scaleY);
    
    langkoushui = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 6);
    langkoushui->setForceTickMode(true);
	langkoushui->setUnitInterval(0.15f);
    langkoushui->setScaleX(1.0f*sGlobal->scaleX);
    langkoushui->setScaleY(1.0f*sGlobal->scaleY);
    rightContainer->addChildLocked(langkoushui, 1);
    langkoushui->setLoopCount(0);
    langkoushui->setPosition(right->getPositionX()-DP(70)*sGlobal->scaleX, right->getPositionY()-DP(138)*sGlobal->scaleY);
    langkoushui->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(3.0f, -DP(4)*sGlobal->scaleX, DP(6)*sGlobal->scaleY), wyDelayTime::make(1.0f), wyMoveBy::make(3.0f, DP(4)*sGlobal->scaleX, -DP(6)*sGlobal->scaleY), NULL)));
    
//    langxiaba = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 5);
//    langxiaba->setForceTickMode(true);
//	langxiaba->setUnitInterval(0.15f);
//    langxiaba->setScaleX(1.0f*sGlobal->scaleX);
//    langxiaba->setScaleY(1.0f*sGlobal->scaleY);
//    rightContainer->addChildLocked(langxiaba, 0);
//    langxiaba->setLoopCount(0);
//    langxiaba->setPosition(langkoushui->getPositionX()-DP(0)*sGlobal->scaleX, langkoushui->getPositionY()-DP(0)*sGlobal->scaleY);
//    langxiaba->runAction(wyRepeatForever::make(wySequence::make(wyRotateBy::make(3.0f, 30), wyDelayTime::make(1.0f), wyRotateBy::make(3.0f, -30), NULL)));
//    
//    langxiaba1 = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 7);
//    langxiaba1->setForceTickMode(true);
//	langxiaba1->setUnitInterval(0.15f);
//    langxiaba1->setScaleX(1.0f*sGlobal->scaleX);
//    langxiaba1->setScaleY(1.0f*sGlobal->scaleY);
//    rightContainer->addChildLocked(langxiaba1, 1);
//    langxiaba1->setLoopCount(0);
//    langxiaba1->setPosition(langkoushui->getPositionX()-DP(0)*sGlobal->scaleX, langkoushui->getPositionY()-DP(0)*sGlobal->scaleY);
    
    scheduleNextAnim();
    scheduleNextAnim1();
    
    wySpriteEx *top = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "start_scene_title.png");//wySprite::make(MyTextureManager::makePNG("start_scene_title.png"));
    top->setPosition(wyDevice::winWidth/2-DP(30)*sGlobal->scaleX, wyDevice::winHeight/2+DP(120)*sGlobal->scaleY);
    top->setScaleX(sGlobal->scaleX*0.6f);
    top->setScaleY(sGlobal->scaleY*0.6f);
    addChildLocked(top, 1);
    
    showNanduUnlockNotice();
    
    if (sGlobal->isFirstStart) {
        createTouch2Start();
        createButtons();
        setTouchEnabled(true);
        
        
    } else {
        setTouchEnabled(false);
        
        init1();
    }
    
    if (sGlobal->isTutorialFromMenu) {
        sGlobal->isTutorialFromMenu = false;
        wyLayer *layer = (wyLayer*) (new AllMenusLayer())->autoRelease();
        addChildLocked(layer, 101, 0);
    }
    
    
    isMenuEnterAnimPlayed = false;
    isDestroied = false;
    isEnterAnimationFinished = false;
    
//    wyAudioManager::getInstance()->playBackgroundMusic("bgm_main.mp3", false, -1);
//    wyAudioManager::getInstance()->playBackgroundMusic("se_gun_gatling.mp3", false, -1);
    
    setKeyEnabled(true);
    setKeyPriority(1);

#if IOS
    ObjCCalls::getInstance()->queryAndSpendScore();
#endif
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(CGScene::updateCoin));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);
    
    if (needPlayBGM) {
        sGlobal->playBGM(0);
    }
    
//    wyUtils::playVideo("zendroid.mp4");
}

CGScene::~CGScene() {
    isDestroied = true;
    //wyTextureManager::getInstance()->removeAllTextures();
}

void CGScene::onEnter() {
    wyScene::onEnter();
}

void CGScene::onEnterTransitionDidFinish() {
    wyScene::onEnterTransitionDidFinish();
#if ANDROID
    if (sGlobal->isFirstStart) {
        showChartBoost();
    } else {
        if (rand()%4 == 0) {
            showChartBoost();
        }
    }
#endif
    sGlobal->isFirstStart = false;
    
    sGlobal->playBGM(0);
}

void CGScene::createTouch2Start() {
    touch2Start = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_touch2start.png");//wySpriteEx::make(MyTextureManager::makePNG("btn_touch2start.png"));
    touch2Start->setScaleX(sGlobal->scaleX*0.5f);
    touch2Start->setScaleY(sGlobal->scaleY*0.5f);
    touch2Start->setPosition(wyDevice::winWidth / 2, DP(100)*sGlobal->scaleY);
    addChildLocked(touch2Start, 100);
    
    touch2Start->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.5f, 200, 100), wyFadeTo::make(0.5f, 100, 200), NULL)));
}

void CGScene::createButtons() {
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_story_mode.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_story_mode.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    title->setScale(SP(24)/sGlobal->descFontSize);
    btn_sMode = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(CGScene::onSModeBtnClicked)), mask, title);
    btn_sMode->setScaleX(sGlobal->scaleX*0.55f/0.8f);
    btn_sMode->setScaleY(sGlobal->scaleY*0.55f/0.8f);
    btn_sMode->setMultiTouchClickable(false);
    btn_sMode->setInterceptTouch(true);
    btn_sMode->setTouchPriority(1000);
    addChildLocked(btn_sMode, 100);
    btn_sMode->setVisible(false);
    btn_sMode->setAnchor(0, 0);
//    btn_sMode->setPosition(wyDevice::winWidth/2-btn_sMode->getWidth()/2*btn_sMode->getScaleX()-wyDevice::winWidth, wyDevice::winHeight/2-btn_sMode->getHeight()/2*btn_sMode->getScaleY() - DP(10)*sGlobal->scaleY);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_endless_mode.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_endless_mode.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    title->setScale(SP(24)/sGlobal->descFontSize);
    btn_eMode = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(CGScene::onEModeBtnClicked)), mask, title);
    btn_eMode->setScaleX(sGlobal->scaleX*0.55f/0.8f);
    btn_eMode->setScaleY(sGlobal->scaleY*0.55f/0.8f);
//    btn_eMode->setPosition(wyDevice::winWidth / 2-wyDevice::winWidth, btn_sMode->getPositionY() - DP(65)*sGlobal->scaleY);
    btn_eMode->setMultiTouchClickable(false);
    btn_eMode->setInterceptTouch(true);
    btn_eMode->setTouchPriority(1000);
    addChildLocked(btn_eMode, 100);
    btn_eMode->setVisible(false);
    btn_eMode->setAnchor(0, 0);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_stuffs.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_stuffs.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    title->setScale(SP(24)/sGlobal->descFontSize);
    btn_allMenu = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(CGScene::onAllMenusBtnClicked)), mask, title);
    btn_allMenu->setScaleX(sGlobal->scaleX*0.6f/0.8f);
    btn_allMenu->setScaleY(sGlobal->scaleY*0.6f/0.8f);
//    btn_allMenu->setPosition(wyDevice::winWidth / 2, DP(22)*sGlobal->scaleY-wyDevice::winWidth);
    btn_allMenu->setMultiTouchClickable(false);
    btn_allMenu->setInterceptTouch(true);
    btn_allMenu->setTouchPriority(1000);
    addChildLocked(btn_allMenu, 100);
    btn_allMenu->setVisible(false);
    
    sModeGantanhao = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao.png");//wySprite::make(MyTextureManager::makePNG("gantanhao.png"));
    sModeGantanhao->setVisible(false);
    addChildLocked(sModeGantanhao, 100);
    
    eModeGantanhao = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao.png");
    eModeGantanhao->setPosition(btn_eMode->getPositionX()+DP(60)*sGlobal->scaleX, btn_eMode->getPositionY()+DP(16)*sGlobal->scaleY);
    eModeGantanhao->setVisible(false);
    addChildLocked(eModeGantanhao, 100);
    
    menuGantanhao = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao.png");
    menuGantanhao->setPosition(btn_allMenu->getPositionX()+DP(60)*sGlobal->scaleX, btn_allMenu->getPositionY()+DP(16)*sGlobal->scaleY);
    menuGantanhao->setVisible(false);
    addChildLocked(menuGantanhao, 100);
    
    wySpriteEx *sp = wySpriteEx::make(MyTextureManager::makePNG("gplus_icon_64.png"));//wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_upgrade_gun.png");
    mask = wySpriteEx::make(MyTextureManager::makePNG("gplus_icon_64.png"));
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_gplus = ControlButton::make(sp, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(CGScene::onGPlusButtonClick)), mask, NULL);
    btn_gplus->setScaleX(sGlobal->scaleX*0.8f);
    btn_gplus->setScaleY(sGlobal->scaleY*0.8f);
    btn_gplus->setMultiTouchClickable(false);
    btn_gplus->setInterceptTouch(true);
    btn_gplus->setTouchPriority(1000);
    addChildLocked(btn_gplus, 100);
//    btn_gplus->setPosition(DP(40)*sGlobal->scaleX, wyDevice::winHeight-DP(40)*sGlobal->scaleY);
    btn_gplus->setVisible(false);
}

void CGScene::onGPlusButtonClick(wyTargetSelector* ts) {
//    LOGE("sharyu123");
    int hightscore = MAX(0, MAX(MAX(sGlobal->distRecNormal, sGlobal->distRecHeroic), sGlobal->distRecInfernal));
//    hightscore = MAX_INT;
//    LOGE("123123submit score:%d", hightscore);
#if ANDROID
    submitScoreAndShowLB(hightscore);
#endif
}

void CGScene::updateCoin(wyTargetSelector *ts) {
    //    coinSprite->tick(ts->getDelta());
    float dt = ts->getDelta();
    
    fireCDTimer += dt;
    if (isFiring) {
        left->tick(ts->getDelta());
    } else {
        if (fireCDTimer > 2.0f) {
            isFiring = true;
            fireCDTimer = 0;
            currentLoopCount = 1;
            left->setLoopCount(5);
            left->playAnimation(1);
        }
    }
    
    daoguang->tick(dt);
    nvkoushui->tick(dt);

    if (isNextAnimScheduled) {
        timer1 += dt;
        if (timer1>=nextTime1) {
            daoguang->playAnimation(3);
            scheduleNextAnim();
        }
    }
    
    if (isNextAnimScheduled1) {
        timer2 += dt;
        if (timer2>=nextTime2) {
            nvkoushui->playAnimation(4);
            scheduleNextAnim1();
        }
    }
}

void CGScene::scheduleNextAnim() {
    isNextAnimScheduled = true;
    nextTime1 = randRange(3, 6);
    timer1 = 0;
}

void CGScene::scheduleNextAnim1() {
    isNextAnimScheduled1 = true;
    nextTime2 = randRange(3, 6);
    timer2 = 0;
}

bool CGScene::keyUp(wyKeyEvent& event) {
    switch(event.keyCode) {
        case KEYCODE_BACK:
#if ANDROID
            showExit();
#endif
            return true;
        default:
            break;
    }
    
    return wyScene::keyUp(event);
}

void CGScene::onOK(wyTargetSelector* ts) {
    wyDirector::getInstance()->popScene();
}

bool CGScene::touchesBegan(wyMotionEvent& e) {
    if (!isMenuEnterAnimPlayed) {
        isMenuEnterAnimPlayed = true;
        wyAction * action = wySequence::make(wyBlink::make(0.4f, 3),
                                             wyDelayTime::make(0.1f),
                                             wyCallFunc::make(wyTargetSelector::make(this, SEL(CGScene::playMenuEnterAnim))),
                                             NULL);
        touch2Start->runAction(action);
    }
    return true;
}

void CGScene::onTutorialOk(wyTargetSelector* ts, void *data) {
//    sGlobal->isRealEndless = false;
//    wyLayer *layer = (wyLayer*) (new NanDuSelectLayer())->autoRelease();
//    addChildLocked(layer, 101, 0);
    
    sGlobal->isRealEndless = false;
    sGlobal->currentSModeDifficulty = DIFFICULTY_NORMAL;
    
    wyDirector* director = wyDirector::getInstance();
    wyScene *scene = new LoadingScene(0, 0);
    scene->autoRelease();
    director->replaceScene(scene);
    
//    PopupDialog *popupDialog = (PopupDialog*) data;
//    popupDialog->getParent()->removeChildLocked(popupDialog, true);
}

void CGScene::onTutorialCancel(wyTargetSelector* ts, void *data) {
    sGlobal->isTrainingEnd = 1;
    MyPrefs::setInt(DB_IS_TRAINING_END, 1);
    
//    sGlobal->isRealEndless = false;
//    wyLayer *layer = (wyLayer*) (new NanDuSelectLayer())->autoRelease();
//    addChildLocked(layer, 101, 0);
//    
//    PopupDialog *popupDialog = (PopupDialog*) data;
//    popupDialog->getParent()->removeChildLocked(popupDialog, true);
    
    sGlobal->isRealEndless = false;
    sGlobal->currentSModeDifficulty = DIFFICULTY_NORMAL;
    
    wyDirector* director = wyDirector::getInstance();
    wyScene *scene = new LoadingScene(0, 101);
    scene->autoRelease();
    director->replaceScene(scene);
}

void CGScene::onSModeBtnClicked(wyTargetSelector* ts) {
    if (sGlobal->isTrainingEnd) {
        sGlobal->isRealEndless = false;
        wyLayer *layer = (wyLayer*) (new NanDuSelectLayer())->autoRelease();
        addChildLocked(layer, 101, 0);
    } else {
        PopupDialog *popupDialog = new PopupDialog();
        popupDialog->autoRelease();
        popupDialog->setContent("play tutorial first?\nstrongly recommended!");
        popupDialog->content->setPosition(popupDialog->content->getPositionX()+DP(15)*sGlobal->scaleX, popupDialog->content->getPositionY()-DP(5)*sGlobal->scaleY);
        popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(CGScene::onTutorialOk), popupDialog));
        popupDialog->setCancelButtonSelector(wyTargetSelector::make(this, SEL_p(CGScene::onTutorialCancel), popupDialog));
        addChildLocked(popupDialog, 102, 0);
    }
    
    if (sModeGantanhao->isVisible()) {
        sModeGantanhao->stopAllActions();
        sModeGantanhao->setVisible(false);
    }
}

void CGScene::onEModeBtnClicked(wyTargetSelector* ts) {
    sGlobal->isRealEndless = true;
    wyLayer *layer = (wyLayer*) (new NanDuSelectLayer())->autoRelease();
    this->addChildLocked(layer, 101, 0);
    
    if (eModeGantanhao->isVisible()) {
        eModeGantanhao->stopAllActions();
        eModeGantanhao->setVisible(false);
    }
}

void CGScene::onAllMenusBtnClicked(wyTargetSelector *ts) {
    wyLayer *layer = (wyLayer*) (new AllMenusLayer())->autoRelease();
    addChildLocked(layer, 102, 0);
}

void CGScene::playMenuEnterAnim(wyTargetSelector* ts) {
    touch2Start->setVisible(false);
    
    btn_sMode->setVisible(true);
    float posx = wyDevice::winWidth/2-btn_sMode->getWidth()/2*btn_sMode->getScaleX();
    float posy = wyDevice::winHeight/2-btn_sMode->getHeight()/2*btn_sMode->getScaleY() - DP(10)*sGlobal->scaleY;
    btn_sMode->setPosition(posx, posy);
    wyAction *action = wyEaseElasticOut::make(0.55f, wyMoveTo::make(0.55f, posx-wyDevice::winWidth, posy, posx, posy));
    btn_sMode->runAction(action);
//    wyAction *action1 = wySequence::make(wyDelayTime::make(0.4f), wySkewBy::make(0.06f, 4, 0), wySkewBy::make(0.06f, -4, 0), NULL);
//    btn_sMode->runAction(action1);
    
    float fx = 0.7*sGlobal->scaleX;
    float tx = 0.9*sGlobal->scaleX;
    if (needShowSModeGantanhao) {
        sModeGantanhao->setPosition(posx+btn_sMode->getWidth()*btn_sMode->getScaleX()-DP(10)*sGlobal->scaleX, posy+btn_sMode->getHeight()*btn_sMode->getScaleY()-DP(10)*sGlobal->scaleY);
        sModeGantanhao->setScale(fx);
        sModeGantanhao->runAction(wySequence::make(wyDelayTime::make(0.5f), wyShow::make(), wyFadeIn::make(0.3f), NULL));
        sModeGantanhao->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
    }
    
    btn_eMode->setVisible(true);
    posx = wyDevice::winWidth/2-btn_eMode->getWidth()/2*btn_eMode->getScaleX();
    posy = wyDevice::winHeight/2-btn_eMode->getHeight()/2*btn_eMode->getScaleY() - DP(70)*sGlobal->scaleY;
    btn_eMode->setPosition(posx+wyDevice::winWidth, posy);
    action = wyEaseElasticOut::make(0.55f, wyMoveTo::make(0.55f, posx+wyDevice::winWidth, posy, posx, posy));
    btn_eMode->runAction(action);

    if (needShowEModeGantanhao) {
        eModeGantanhao->setPosition(posx+btn_eMode->getWidth()*btn_eMode->getScaleX()-DP(10)*sGlobal->scaleX, posy+btn_eMode->getHeight()*btn_eMode->getScaleY()-DP(10)*sGlobal->scaleY);
        eModeGantanhao->setScale(fx);
        eModeGantanhao->runAction(wySequence::make(wyDelayTime::make(0.5f), wyShow::make(), wyFadeIn::make(0.3f), NULL));
        eModeGantanhao->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
    }
    
    btn_allMenu->setVisible(true);
    posx = wyDevice::winWidth/2;
    posy = DP(30)*sGlobal->scaleY;
    btn_allMenu->setPosition(posx, posy-DP(50)*sGlobal->scaleY);
    action = wySequence::make(wyDelayTime::make(0.6f), wyEaseElasticOut::make(0.6f, wyMoveTo::make(0.6f, posx, posy-DP(50)*sGlobal->scaleY, posx, posy)), NULL);
    btn_allMenu->runAction(action);
    
    btn_gplus->setVisible(true);
    posx = DP(50)*sGlobal->scaleX;
    posy = wyDevice::winHeight-DP(50)*sGlobal->scaleY;
    btn_gplus->setPosition(posx, posy+DP(120)*sGlobal->scaleY);
    action = wySequence::make(wyDelayTime::make(0.6f), wyEaseElasticOut::make(0.6f, wyMoveTo::make(0.6f, posx, posy+DP(120)*sGlobal->scaleY, posx, posy)), NULL);
    btn_gplus->runAction(action);
}

void CGScene::init1() {
    createButtons();
    
    float posx = wyDevice::winWidth/2-btn_sMode->getWidth()/2*btn_sMode->getScaleX();
    float posy = wyDevice::winHeight/2-btn_sMode->getHeight()/2*btn_sMode->getScaleY() - DP(10)*sGlobal->scaleY;
    btn_sMode->setPosition(posx, posy);
    
    float fx = 0.8f*sGlobal->scaleX;
    float tx = 1.0*sGlobal->scaleX;
    if (needShowSModeGantanhao) {
        sModeGantanhao->setVisible(true);
        sModeGantanhao->setPosition(posx+btn_sMode->getWidth()*btn_sMode->getScaleX()-DP(10)*sGlobal->scaleX, posy+btn_sMode->getHeight()*btn_sMode->getScaleY()-DP(10)*sGlobal->scaleY);
        sModeGantanhao->setScale(fx);
        sModeGantanhao->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
    }
    
    posx = wyDevice::winWidth/2-btn_eMode->getWidth()/2*btn_eMode->getScaleX();
    posy = wyDevice::winHeight/2-btn_eMode->getHeight()/2*btn_eMode->getScaleY() - DP(70)*sGlobal->scaleY;
    btn_eMode->setPosition(posx, posy);
    
    if (needShowEModeGantanhao) {
        eModeGantanhao->setVisible(true);
        eModeGantanhao->setPosition(posx+btn_eMode->getWidth()*btn_eMode->getScaleX()-DP(10)*sGlobal->scaleX, posy+btn_eMode->getHeight()*btn_eMode->getScaleY()-DP(10)*sGlobal->scaleY);
        eModeGantanhao->setScale(fx);
        eModeGantanhao->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
    }
    
    posx = wyDevice::winWidth/2;
    posy = DP(30)*sGlobal->scaleY;
    btn_allMenu->setPosition(posx, posy);
    
    posx = DP(50)*sGlobal->scaleX;
    posy = wyDevice::winHeight-DP(50)*sGlobal->scaleY;
    btn_gplus->setPosition(posx, posy);
    
    btn_sMode->setVisible(true);
    btn_eMode->setVisible(true);
    btn_allMenu->setVisible(true);
    btn_gplus->setVisible(true);
}

void CGScene::showNanduUnlockNotice() {
    bool flag1, flag2, flag3, flag4, flag5;
    bool needShowDialog = false;
    
    flag1 = sGlobal->isSModeUnlockable(DIFFICULTY_HEROIC);
    flag2 = sGlobal->isSModeUnlockable(DIFFICULTY_HARD);
    needShowSModeGantanhao = (flag1 || flag2);

    flag3 = sGlobal->isEModeUnlockable(DIFFICULTY_NORMAL);
    flag4 = sGlobal->isEModeUnlockable(DIFFICULTY_HEROIC);
    flag5 = sGlobal->isEModeUnlockable(DIFFICULTY_HARD);
    needShowEModeGantanhao = (flag3 || flag4 || flag5);
    
    if (flag1 && !wyPrefs::getBool("flag1", false)) {
        needShowDialog = true;
        wyPrefs::setBool("flag1", true);
    }
    if (flag2 && !wyPrefs::getBool("flag2", false)) {
        needShowDialog = true;
        wyPrefs::setBool("flag2", true);
    }
    if (flag3 && !wyPrefs::getBool("flag3", false)) {
        needShowDialog = true;
        wyPrefs::setBool("flag3", true);
    }
    if (flag4 && !wyPrefs::getBool("flag4", false)) {
        needShowDialog = true;
        wyPrefs::setBool("flag4", true);
    }
    if (flag5 && !wyPrefs::getBool("flag5", false)) {
        needShowDialog = true;
        wyPrefs::setBool("flag5", true);
    }
    
    if (needShowDialog) {
        PopupDialog *popupDialog = new PopupDialog();
        popupDialog->autoRelease();
        popupDialog->showOnlyOkBtn();
        popupDialog->setContent(OTHER_STR[STR_DIF_ULK]);
        addChildLocked(popupDialog, 102, 0);
        popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(CGScene::onPopupDailogClose), popupDialog));
    } else {
        if (sGlobal->currentProgress[DIFFICULTY_HARD] == 310 && sGlobal->s03_score[DIFFICULTY_HARD*10+10] != 0 && !sGlobal->isGameComplete) {
            PopupDialog *popupDialog = new PopupDialog();
            popupDialog->autoRelease();
            popupDialog->showOnlyOkBtn();
            popupDialog->setContent(OTHER_STR[STR_STORYMODE_COMPLETE]);
            addChildLocked(popupDialog, 102, 0);
            popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(CGScene::onPopupDailogClose), popupDialog));
            
            sGlobal->isGameComplete = 1;
            MyPrefs::setInt(DB_IS_GAME_COMPLETE, 1);
        } else {
            showDailyRewardAndVote();
        }
    }
}

void CGScene::onPopupDailogClose(wyTargetSelector *ts, void *data) {
    showDailyRewardAndVote();
    
    PopupDialog *popupDialog = (PopupDialog*) data;
    popupDialog->getParent()->removeChildLocked(popupDialog, true);
}

void CGScene::showDailyRewardAndVote() {
    int ret = DailyRewardLayer::loginDay(this);
    if (!ret) {
        if (sGlobal->isFirstStart) {
            DailyRewardLayer *layer = new DailyRewardLayer(true, true);
            layer->autoRelease();
            addChildLocked(layer, 101, 0);
        } else {
            VoteUsLayer::showVoteUsLayer(this);
        }
    }
}


