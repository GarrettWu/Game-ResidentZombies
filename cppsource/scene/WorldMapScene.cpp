//
//  WorldMapScene.cpp
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "WorldMapScene.h"
#include "LoadingScene.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif

static void onPageClicked(ShPageControl* pageControl, int index, void* data) {
    WorldMapScene* layer = (WorldMapScene*)data;
    layer->onSeasonPhotoClicked(index);
//    LOGE("ex:%d", index);
    sGlobal->playEffect(se_button);
}

static void onPageChanged(ShPageControl* pageControl, int index, void* data) {
    WorldMapScene* layer = (WorldMapScene*)data;
    layer->switchSeason(index);
//    LOGE("ss:%d", index);
}

static void onPagePositionChanged(ShPageControl* pageControl, wyNode* page, float offset, void* data) {
    WorldMapScene *layer = (WorldMapScene*)data;
    if (page->getTag() == 0 && fabs(offset)<DP(16)*sGlobal->scaleX) {
        layer->switchSeason(0);
    }
    if (page->getTag() == 1 && fabs(offset)<DP(16)*sGlobal->scaleX) {
        layer->switchSeason(1);
    }
    if (page->getTag() == 2 && fabs(offset)<DP(16)*sGlobal->scaleX) {
        layer->switchSeason(2);
    }
    
}

void WorldMapScene::switchSeason(int seasonId) {
    if (seasonId == currentSeasonId) {
        return;
    }
    
    cLayer->setVisible(true);
//    cLayer->runAction(wySequence::make(wyFadeTo::make(0.1f, 0, 200), wyFadeTo::make(0.1f, 200, 0), NULL));
    cLayer->runAction(wyFadeOut::make(0.5f));
    
    currentSeasonId = seasonId;
    
    bgLayer0->setVisible(currentSeasonId == 0);
    bgLayer1->setVisible(currentSeasonId == 1);
    bgLayer2->setVisible(currentSeasonId == 2);
    
    updateSeasonInfo(seasonId);
}

void WorldMapScene::updateSeasonInfo(int seasonId) {
    switch (seasonId) {
        case 0: {
            maptitle->setText("ghost  village");
            int scount = sGlobal->countScount(1, sGlobal->currentSModeDifficulty);
            char temp[10];
            sprintf(temp, "%d/10", scount);
            mapInfo->setText(temp);
            break;
        }
        case 1: {
            maptitle->setText("lost   temple");
            int scount = sGlobal->countScount(2, sGlobal->currentSModeDifficulty);
            char temp[10];
            sprintf(temp, "%d/10", scount);
            mapInfo->setText(temp);
            break;
        }
        case 2: {
            maptitle->setText("blue   sea");
            
            int scount = sGlobal->countScount(3, sGlobal->currentSModeDifficulty);
            char temp[10];
            sprintf(temp, "%d/10", scount);
            mapInfo->setText(temp);
            break;
        }
        default:
            break;
    }
}

static void onCoverBlinkEnd(wyAction* action, void* data) {
    WorldMapScene* scene = (WorldMapScene*)data;
    scene->pageControl->setVisible(false);
    scene->levelNodeContainer->setVisible(true);
    scene->mapInfo->setVisible(false);
    scene->mapInfoGrade->setVisible(false);
    scene->startBtn->setVisible(true);
    if (scene->touchLayer != NULL && action != NULL) {
        scene->touchLayer->getParent()->removeChildLocked(scene->touchLayer, true);
        scene->touchLayer = NULL;
    }
    
    if (scene->toSelectNode != NULL) {
        scene->needSelDelay = true;
        scene->toSelectNode->onClick(NULL);
    }
    scene->isExpandingSeason = false;
}

static void onCoverBlinkEnd1(wyAction* action, void* data) {
    WorldMapScene* scene = (WorldMapScene*)data;
    PopupDialog *popupDialog = new PopupDialog();
    popupDialog->autoRelease();
    if (scene->lockSelIndex == 1) {
        popupDialog->setContent(OTHER_STR[STR_ULK_2ND_STADGE]);
    } else {
        popupDialog->setContent(OTHER_STR[STR_ULK_3RD_STADGE]);
    }
    popupDialog->showOnlyOkBtn();
    scene->addChildLocked(popupDialog, 101, 0);

    if (scene->touchLayer != NULL && action != NULL) {
        scene->touchLayer->getParent()->removeChildLocked(scene->touchLayer, true);
        scene->touchLayer = NULL;
    }

    scene->isExpandingSeason = false;
}

void WorldMapScene::onSeasonPhotoClicked(int index) {
    if (lockFlags[index] == false) {
        touchLayer = TouchLayer::make();
        wyDirector* director = wyDirector::getInstance();
        director->getRunningScene()->addChildLocked(touchLayer);
        
        wyNode *parent = pageControl->getFirstChild();
        wyNode *node = parent->getChildByTag(index);
        coverLayer->setPosition(node->getPositionX()+parent->getPositionX()+pageControl->getPositionX()-coverLayer->getWidth()/2, node->getPositionY()+parent->getPositionY()+pageControl->getPositionY()-coverLayer->getHeight()/2);
        coverLayer->setVisible(true);
        wyAction *a = wyFadeOut::make(0.3f);
        wyActionCallback callback = {
            NULL,
            onCoverBlinkEnd1,
            NULL
        };
        a->setCallback(&callback, this);
        coverLayer->runAction(a);
        
        lockSelIndex = index;
        
        return;
    }
    
    if (!isExpandingSeason) {
        expandSeason(index);
    }
}

void WorldMapScene::expandSeason(int seasonId, int levelId, bool needAnim) {
//    LOGE("levelId:%d", levelId);
    
    this->mapIndex = (seasonId+1)*100;
    currentMapId = -1;
    needSelDelay = false;

    int selIndex = 1;
    if (levelId != -1) {
        selIndex = levelId;
    } else {
        if (sGlobal->currentProgress[sGlobal->currentSModeDifficulty]/100-1 == seasonId) {
            selIndex = sGlobal->currentProgress[sGlobal->currentSModeDifficulty]%100;
        }
    }
    
    toSelectNode = NULL;
    for(int i = 0; i < levelNodeArr->num; ++i) {
        LevelNode* node = (LevelNode*)levelNodeArr->arr[i];
        node->updateNodeBySeasonId(seasonId+1);
        
        if (i+1 == selIndex) {
//            LOGE("selIndex:%d", selIndex);
            toSelectNode = node;
            //node->onClick(NULL);
        }
    }
    
    if (needAnim) {
        touchLayer = TouchLayer::make();
        wyDirector* director = wyDirector::getInstance();
        director->getRunningScene()->addChildLocked(touchLayer);
        
        wyNode *parent = pageControl->getFirstChild();
        wyNode *node = parent->getChildByTag(seasonId);
        coverLayer->setPosition(node->getPositionX()+parent->getPositionX()+pageControl->getPositionX()-coverLayer->getWidth()/2, node->getPositionY()+parent->getPositionY()+pageControl->getPositionY()-coverLayer->getHeight()/2);
        coverLayer->setVisible(true);
        wyAction *a = wyFadeOut::make(0.3f);
        wyActionCallback callback = {
            NULL,
            onCoverBlinkEnd,
            NULL
        };
        a->setCallback(&callback, this);
        coverLayer->stopAllActions();
        coverLayer->runAction(a);
        isExpandingSeason = true;
    } else {
        onCoverBlinkEnd(NULL, this);
    }
}

WorldMapScene::WorldMapScene(bool isExpand) {
    touchLayer = NULL;
    
    
    highlight = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "cursor.png");
    highlight->setColor(wyc3b(255, 85, 24));
    highlight->setScaleX(sGlobal->scaleX*0.9f);
    highlight->setScaleY(sGlobal->scaleY*1.0f);
    highlight->setVisible(false);
    highlight->setAnchor(0.5, 0.55);
    addChildLocked(highlight);
    
    photo_mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "photo_mask.png");
    photo_mask->setScaleX(sGlobal->scaleX*0.75f);
    photo_mask->setScaleY(sGlobal->scaleY*0.75f);
    photo_mask->setAnchor(1.0f, 0);
    photo_mask->setVisible(false);
    photo_mask->retain();
//    addChildLocked(photo_mask,99);
    
    levelNodeContainer = wyNode::make();
    addChildLocked(levelNodeContainer);
    levelNodeContainer->setVisible(false);
    
    levelNodeArr = wyArrayNew(10);
    for (int i = 0; i < 10; i++) {
        LevelNode *levelNode = new LevelNode(i+1);
        levelNode->autoRelease();
        levelNodeContainer->addChildLocked(levelNode,0,i);
        wyArrayPush(levelNodeArr, levelNode);
    }

    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    normal->setFlipX(true);
    mask->setFlipX(true);
	backBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(WorldMapScene::onBackButtonClick)), mask, NULL);
	backBtn->setPosition(DP(55)*sGlobal->scaleX, DP(38)*sGlobal->scaleY);
    backBtn->setInterceptTouch(true);
    backBtn->setTouchPriority(302);
    backBtn->setScaleX(sGlobal->scaleX*0.8f);
    backBtn->setScaleY(sGlobal->scaleY*0.8f);
    backBtn->setRotation(-90);
	addChildLocked(backBtn);

    highscore = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    highscore->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
    highscore->setAlignment(wyBitmapFontLabel::LEFT);
    highscore->setPosition(wyDevice::winWidth/2, DP(55)*sGlobal->scaleY);
//    highscore->setColor(wyc3b(255, 255, 0));
    highscore->setColor(wyc3b(220, 220, 220));
    addChildLocked(highscore);
    highscore->setVisible(false);
    
    levelDesc = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    levelDesc->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
    levelDesc->setAlignment(wyBitmapFontLabel::LEFT);
    levelDesc->setPosition(wyDevice::winWidth/2, DP(70)*sGlobal->scaleY);
//    levelDesc->setColor(wyc3b(253, 86, 30));
    levelDesc->setColor(wyc3b(220, 220, 220));
    addChildLocked(levelDesc);
    levelDesc->setVisible(false);
    levelDesc->setAlpha(0);

    normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_go.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_go.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
	startBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(WorldMapScene::onStartButtonClick)), mask, NULL);
	startBtn->setPosition(wyDevice::winWidth - DP(55)*sGlobal->scaleX, DP(38)*sGlobal->scaleY);
    startBtn->setInterceptTouch(true);
    startBtn->setTouchPriority(302);
	addChildLocked(startBtn);
    startBtn->setScaleX(sGlobal->scaleX*0.8f);
    startBtn->setScaleY(sGlobal->scaleY*0.8f);
    startBtn->setEnabled(false);
//    startBtn->setRotation(90);
    startBtn->setVisible(false);
//    startBtn->setDisabledState(disabled);
    
    this->setTouchEnabled(true);
    this->setTouchPriority(300);
    
    wyTimer* timer = wyTimer::make(wyTargetSelector::make(this, SEL(WorldMapScene::onUpdateBg)));
    scheduleLocked(timer);
    
    isTraining = false;
    
    wySprite* page1 = wySprite::make(MyTextureManager::makePNG("s01_shortcut.png"));
    wySprite* page2 = wySprite::make(MyTextureManager::makePNG("s02_shortcut.png"));
    wySprite* page3 = wySprite::make(MyTextureManager::makePNG("s03_shortcut.png"));
    
    page1->setScaleX(sGlobal->scaleX*1.3f);
    page1->setScaleY(sGlobal->scaleY*1.3f);
    
    page2->setScaleX(sGlobal->scaleX*1.3f);
    page2->setScaleY(sGlobal->scaleY*1.3f);
    
    page3->setScaleX(sGlobal->scaleX*1.3f);
    page3->setScaleY(sGlobal->scaleY*1.3f);
    
    pageControl = ShPageControl::make();
    ShPageControlCallback callback = {
        onPageClicked,
        onPageChanged,
        onPagePositionChanged,
    };
    pageControl->setCallback(&callback, this);
    pageControl->setPageSpacing(DP(80)*sGlobal->scaleX);
    pageControl->addPage(page1);
    pageControl->addPage(page2);
    pageControl->addPage(page3);
    pageControl->setTouchPriority(301);
    addChildLocked(pageControl);
    
    pageControl->setPosition(0, DP(20)*sGlobal->scaleY);
    
    page1->setTag(0);
    page2->setTag(1);
    page3->setTag(2);
    
    bgLayer0 = new GameBackgroundLayer(100);
    bgLayer0->autoRelease();
    addChildLocked(bgLayer0, -1);
    bgLayer0->setVisible(true);
    
    bgLayer1 = new GameBackgroundLayer(200);
    bgLayer1->autoRelease();
    addChildLocked(bgLayer1, -1);
    bgLayer1->setVisible(false);
    
    bgLayer2 = new GameBackgroundLayer(300);
    bgLayer2->autoRelease();
    addChildLocked(bgLayer2, -1);
    bgLayer2->setVisible(false);
    
    wySpriteEx *bgmask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_mask.png");//wySpriteEx::make(MyTextureManager::makePNG("level_select_bg_mask.png", WY_TEXTURE_PIXEL_FORMAT_RGBA8888));
    bgmask->setContentSize(wyDevice::winWidth, wyDevice::winHeight);
    bgmask->setAutoFit(true);
    bgmask->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(bgmask, -1);
//    bgmask->setVisible(false);
    
    cLayer = wyColorLayer::make();
    cLayer->setColor(wyc4b(0, 0, 0, 255));
    addChildLocked(cLayer, -1);
    cLayer->setVisible(false);

    
    wySpriteEx *ws_deco_1 = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_deco_1.png");
    ws_deco_1->setScale(wyDevice::winWidth/ws_deco_1->getWidth());
    ws_deco_1->setAnchor(0, 1.0f);
    ws_deco_1->setPosition(0, wyDevice::winHeight);
    addChildLocked(ws_deco_1, -1);
    
    wySpriteEx *ws_deco_2 = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_deco_2.png");
    ws_deco_2->setScale(ws_deco_1->getScale());
    ws_deco_2->setAnchor(0, 1.0f);
    ws_deco_2->setPosition(0, wyDevice::winHeight-ws_deco_1->getHeight()*ws_deco_1->getScale());
    addChildLocked(ws_deco_2, -1);

    wySpriteEx *ws_deco_3 = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_deco_3.png");
    ws_deco_3->setScale(ws_deco_1->getScale());
    ws_deco_3->setAnchor(1.0, 1.0f);
    ws_deco_3->setPosition(wyDevice::winWidth, wyDevice::winHeight-ws_deco_1->getHeight()*ws_deco_1->getScale());
    addChildLocked(ws_deco_3, -1);

    wySpriteEx *ws_deco_4 = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_deco_1.png");
    ws_deco_4->setScale(ws_deco_1->getScale());
    ws_deco_4->setFlipY(true);
    ws_deco_4->setFlipX(true);
    ws_deco_4->setAnchor(1.0f, 0);
    ws_deco_4->setPosition(wyDevice::winWidth, 0);
    addChildLocked(ws_deco_4, -1);
    
    wySpriteEx *ws_deco_5 = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_deco_2.png");
    ws_deco_5->setScale(ws_deco_1->getScale());
    ws_deco_5->setFlipY(true);
    ws_deco_5->setFlipX(true);
    ws_deco_5->setAnchor(1.0, 0);
    ws_deco_5->setPosition(wyDevice::winWidth, ws_deco_4->getHeight()*ws_deco_4->getScale());
    addChildLocked(ws_deco_5, -1);
    
    wySpriteEx *ws_deco_6 = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "ws_deco_3.png");
    ws_deco_6->setScale(ws_deco_1->getScale());
    ws_deco_6->setFlipY(true);
    ws_deco_6->setFlipX(true);
    ws_deco_6->setAnchor(0, 0);
    ws_deco_6->setPosition(0, ws_deco_4->getHeight()*ws_deco_4->getScale());
    addChildLocked(ws_deco_6, -1);

    
    coverLayer = wyColorLayer::make();
    coverLayer->setColor(wyc4b(255, 255, 255, 255));
    addChildLocked(coverLayer, 99);
    coverLayer->setVisible(false);
    coverLayer->setContentSize(page1->getWidth()*page1->getScaleX(), page1->getHeight()*page1->getScaleY());

    maptitle = wyBitmapFontLabel::make(sGlobal->font_title, "a");
    maptitle->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
    maptitle->setAlignment(wyBitmapFontLabel::LEFT);
    maptitle->setPosition(wyDevice::winWidth/2+DP(5)*sGlobal->scaleX, wyDevice::winHeight-DP(45)*sGlobal->scaleY);
    maptitle->setColor(wyc3b(255, 255, 255));
    addChildLocked(maptitle);
    
    mapInfo = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    mapInfo->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
    mapInfo->setAlignment(wyBitmapFontLabel::LEFT);
    mapInfo->setPosition(wyDevice::winWidth/2+DP(0)*sGlobal->scaleX, DP(95)*sGlobal->scaleY);
    mapInfo->setColor(wyc3b(220, 220, 220));
    addChildLocked(mapInfo);
    
    mapInfoGrade = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s_small.png");
    mapInfoGrade->setScaleX(sGlobal->scaleX*1.0f);
    mapInfoGrade->setScaleY(sGlobal->scaleY*1.0f);
    mapInfoGrade->setPosition(mapInfo->getPositionX()+DP(50)*sGlobal->scaleX, mapInfo->getPositionY()+DP(2)*sGlobal->scaleX);
    addChildLocked(mapInfoGrade);
    
    wySpriteEx* nandu;
    wyBitmapFontLabel* nanduDesc;
    switch (sGlobal->currentSModeDifficulty) {
        case DIFFICULTY_NORMAL:
            nandu = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy1.png");
            nanduDesc = wyBitmapFontLabel::make(sGlobal->font_desc, "easy");
            break;
        case DIFFICULTY_HEROIC:
            nandu = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard1.png");
            nanduDesc = wyBitmapFontLabel::make(sGlobal->font_desc, "hard");
            break;
        case DIFFICULTY_HARD:
            nandu = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png");
            nanduDesc = wyBitmapFontLabel::make(sGlobal->font_desc, "carzy");
            break;
        default:
            nandu = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy1.png");
            nanduDesc = wyBitmapFontLabel::make(sGlobal->font_desc, "easy");
            break;
    }
    nandu->setScaleX(sGlobal->scaleX*0.5f);
    nandu->setScaleY(sGlobal->scaleY*0.5f);
    nandu->setPosition(DP(56)*sGlobal->scaleX, wyDevice::winHeight-DP(40)*sGlobal->scaleY);
    addChildLocked(nandu);
    
//    nanduDesc->setAnchor(0, 0.5f);
//    nanduDesc->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
//    nanduDesc->setPosition(DP(86)*sGlobal->scaleX, wyDevice::winHeight-DP(43)*sGlobal->scaleY);
//    addChildLocked(nanduDesc);
    
    initLocks();
    
    currentNode = NULL;
    currentMapId = -1;
    isExpandingSeason = false;
    
//    needUnlockS2 = true;
    if (needUnlockS2) {
        touchLayer = TouchLayer::make();
        addChildLocked(touchLayer);
        
        currentSeasonId = 0;
        updateSeasonInfo(currentSeasonId);
        
        bgLayer0->setVisible(currentSeasonId == 0);
        bgLayer1->setVisible(currentSeasonId == 1);
        bgLayer2->setVisible(currentSeasonId == 2);
        
        pageControl->setInitialPage(currentSeasonId);
    }
    else if (needUnlockS3) {
        touchLayer = TouchLayer::make();
        addChildLocked(touchLayer);
        
        currentSeasonId = 1;
        updateSeasonInfo(currentSeasonId);
        
        bgLayer0->setVisible(currentSeasonId == 0);
        bgLayer1->setVisible(currentSeasonId == 1);
        bgLayer2->setVisible(currentSeasonId == 2);
        
        pageControl->setInitialPage(currentSeasonId);
    } else {
        if (isExpand) {
            currentSeasonId = sGlobal->index/100-1;
            updateSeasonInfo(currentSeasonId);
            pageControl->gotoPage(currentSeasonId);
            
            bgLayer0->setVisible(currentSeasonId == 0);
            bgLayer1->setVisible(currentSeasonId == 1);
            bgLayer2->setVisible(currentSeasonId == 2);
            
//            LOGE("curIndex:%d", sGlobal->index);
            expandSeason(currentSeasonId, sGlobal->index%100, false);
        } else {
            currentSeasonId = sGlobal->currentProgress[sGlobal->currentSModeDifficulty]/100-1;
            updateSeasonInfo(currentSeasonId);
            
            bgLayer0->setVisible(currentSeasonId == 0);
            bgLayer1->setVisible(currentSeasonId == 1);
            bgLayer2->setVisible(currentSeasonId == 2);
            
            pageControl->setInitialPage(currentSeasonId);
        }
    }
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(WorldMapScene::updateWorld));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);
}

WorldMapScene::~WorldMapScene() {
    wyArrayDestroy(levelNodeArr);
    photo_mask->release();
    delete [] lockFlags;
}

void WorldMapScene::onEnter() {
    wyScene::onEnter();
}

void WorldMapScene::onEnterTransitionDidFinish() {
    wyScene::onEnterTransitionDidFinish();
    
    sGlobal->playBGM(0);
    
    if (needUnlockS2) {
        pageControl->gotoPage(1, true, 1.0f);
        
        lockContainerS2->runAction(wySequence::make(wyDelayTime::make(1.0f), wyBlink::make(1.0f, 3), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(WorldMapScene::onUnlockNeedBlink), 1)), wyFadeOut::make(1.0f, true), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(WorldMapScene::onUnlockEnd), 1)), NULL));
    }
    else if (needUnlockS3) {
        pageControl->gotoPage(2, true, 1.0f);
        
        lockContainerS3->runAction(wySequence::make(wyDelayTime::make(1.0f), wyBlink::make(1.0f, 3), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(WorldMapScene::onUnlockNeedBlink), 2)), wyFadeOut::make(1.0f, true), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(WorldMapScene::onUnlockEnd), 2)), NULL));
    }
}

bool WorldMapScene::touchesBegan(wyMotionEvent& event) {
    return true;
}

void WorldMapScene::onUnlockEnd(wyTargetSelector* ts, int data){
    lockFlags[data] = true;
    
    if (data == 2) {
        PopupDialog *popupDialog = new PopupDialog();
        popupDialog->autoRelease();
        popupDialog->showOnlyOkBtn();
        char temp[200];
        sprintf(temp, "congratulations, you have also unlocked %s endless mode!", DIFFICULTY_STR[sGlobal->currentSModeDifficulty]);
        popupDialog->setContent(temp);
        addChildLocked(popupDialog, 102, 0);
    }
    
    if (touchLayer != NULL) {
        touchLayer->getParent()->removeChildLocked(touchLayer, true);
        touchLayer = NULL;
    }
}

void WorldMapScene::onUnlockNeedBlink(wyTargetSelector* ts, int data){
    wyNode *parent = pageControl->getFirstChild();
    wyNode *node = parent->getChildByTag(data);
    coverLayer->setPosition(node->getPositionX()+parent->getPositionX()+pageControl->getPositionX()-coverLayer->getWidth()/2, node->getPositionY()+parent->getPositionY()+pageControl->getPositionY()-coverLayer->getHeight()/2);
    wyAction *a = wyFadeTo::make(0.5f, 255, 0);
    coverLayer->runAction(a);
//    coverLayer->setAlpha(200);
    coverLayer->setVisible(true);
}

void WorldMapScene::initLocks() {
    lockFlags = new bool[3];
    lockFlags[0] = true;
    lockFlags[1] = false;
    lockFlags[2] = false;
    
    needUnlockS2 = false;
    needUnlockS3 = false;
    
    lockContainerS2 = NULL;
    lockContainerS3 = NULL;
    
    int curProgress = sGlobal->currentProgress[sGlobal->currentSModeDifficulty];
    if (curProgress <= 110) {
        wyNode *parent = pageControl->getFirstChild();
        wyNode *node = parent->getChildByTag(1);
        
        lockContainerS2 = wyNode::make();
        parent->addChildLocked(lockContainerS2);
        lockContainerS2->setPosition(node->getPositionX(), node->getPositionY());
        
        wySPXSprite *lock = wySPXSprite::make("s03.sprite", false, MyTextureManager::makePNG("s03.png"), 0);
        lock->setScaleX(sGlobal->scaleX*0.78f);
        lock->setScaleY(sGlobal->scaleY*0.83f);
        lockContainerS2->addChildLocked(lock, 0);
        
        //进度在1－？，并且1－10有分数，说明第二大关未解锁
        if (sGlobal->s01_grade[sGlobal->currentSModeDifficulty*10+10] != 0) {
            sGlobal->currentProgress[sGlobal->currentSModeDifficulty] = 201;
            sGlobal->arrayToStr(DB_PROGRESS, sGlobal->currentProgress, 3);
            
            needUnlockS2 = true;
        }
    
        node = parent->getChildByTag(2);
        
        lockContainerS3 = wyNode::make();
        parent->addChildLocked(lockContainerS3);
        lockContainerS3->setPosition(node->getPositionX(), node->getPositionY());
        
        lock = wySPXSprite::make("s03.sprite", false, MyTextureManager::makePNG("s03.png"), 0);
        lock->setScaleX(sGlobal->scaleX*0.78f);
        lock->setScaleY(sGlobal->scaleY*0.83f);
        lockContainerS3->addChildLocked(lock, 0);
    }
    else if (curProgress > 110 && curProgress <= 210) {
        wyNode *parent = pageControl->getFirstChild();
        wyNode *node = parent->getChildByTag(2);
        
        lockContainerS3 = wyNode::make();
        parent->addChildLocked(lockContainerS3);
        lockContainerS3->setPosition(node->getPositionX(), node->getPositionY());
        
        wySPXSprite *lock = wySPXSprite::make("s03.sprite", false, MyTextureManager::makePNG("s03.png"), 0);
        lock->setScaleX(sGlobal->scaleX*0.78f);
        lock->setScaleY(sGlobal->scaleY*0.83f);
        lockContainerS3->addChildLocked(lock, 0);
        
        //进度在2－？，并且2－10有分数，说明第三大关未解锁
        if (sGlobal->s02_grade[sGlobal->currentSModeDifficulty*10+10] != 0) {
            sGlobal->currentProgress[sGlobal->currentSModeDifficulty] = 301;
            sGlobal->arrayToStr(DB_PROGRESS, sGlobal->currentProgress, 3);
            
            needUnlockS3 = true;
        }
        
        lockFlags[1] = true;
    }
    else if (curProgress > 210) {
        lockFlags[1] = true;
        lockFlags[2] = true;
    }
    
    if (sGlobal->isDebug) {
        lockFlags[1] = true;
        lockFlags[2] = true;
    }
}

static void onCollapsePage(wyAction* action, void* data) {
    WorldMapScene* scene = (WorldMapScene*)data;
    scene->pageControl->setVisible(true);
    scene->levelNodeContainer->setVisible(false);
    scene->highlight->setVisible(false);
}

void WorldMapScene::onBackButtonClick(wyTargetSelector* ts) {
    if (pageControl->isVisible()) {
//        wyDirector* director = wyDirector::getInstance();
//        wyScene *scene = new LoadingScene(SCENE_MENU, 0);
//        scene->autoRelease();
//        director->replaceScene(scene);
        
        sGlobal->gotoCGScene();
    } else {
//        float yy = DP(180)*sGlobal->scaleY;
//        for (int i = 0; i < 10; i++) {
//            LevelNode *levelNode = (LevelNode*)levelNodeContainer->getChildByTag(i);
//            wyAction *a = wyMoveTo::make(0.2f, levelNode->posx, levelNode->posy, wyDevice::winWidth/2, wyDevice::winHeight-yy-DP(70)*sGlobal->scaleY);
//            if (i == 0) {
//                wyActionCallback callback = {
//                    NULL,
//                    onCollapsePage,
//                    NULL
//                };
//                a->setCallback(&callback, this);
//            }
//            levelNode->runAction(a);
//        }
        
        mapInfo->setVisible(true);
        mapInfoGrade->setVisible(true);
        pageControl->setVisible(true);
        levelNodeContainer->setVisible(false);
        highlight->setVisible(false);
        highscore->setVisible(false);
        levelDesc->setVisible(false);
        startBtn->setVisible(false);
    }
}

void WorldMapScene::onStartButtonClick(wyTargetSelector* ts) {
//    sGlobal->playEffect("se_button.mp3");
    
    startBtn->setEnabled(false);
    
//    wyDirector* director = wyDirector::getInstance();
//    wyScene *scene = new LoadingScene(SCENE_UPGRADE, currentMapId);
//    scene->autoRelease();
//    director->replaceScene(scene);
    
    sGlobal->isRealEndless = false;
    sGlobal->gotoUpgradeScene(currentMapId);
}

void WorldMapScene::updateWorld(wyTargetSelector *ts) {
    float dt = ts->getDelta();
    if (bgLayer0->isVisible()) {
        bgLayer0->update(dt);
    }
    if (bgLayer1->isVisible()) {
        bgLayer1->update(dt);
    }

    if (bgLayer2->isVisible()) {
        bgLayer2->update(dt);
    }
}

void WorldMapScene::onUpdateBg(wyTargetSelector* ts) {
    if (bgLayer0->isVisible()) {
        bgLayer0->offsetBy(-DP(1), 0);
    }
    
    if (bgLayer1->isVisible()) {
        bgLayer1->offsetBy(-DP(1), 0);
    }
    
    if (bgLayer2->isVisible()) {
        bgLayer2->offsetBy(-DP(1), 0);
    }
}

void WorldMapScene::hideHighlight() {
    highlight->setVisible(false);
    highlight->stopAllActions();
}

void WorldMapScene::showHighlight() {
    highlight->stopAction(0);
    wyAction *tint = wyRepeatForever::make(wySequence::make(wyTintTo::make(1.0f, 255, 85, 24, 180, 85, 24), wyTintTo::make(1.0f, 180, 85, 24, 255, 85, 24), NULL));
    tint->setTag(0);
    highlight->runAction(tint);
    
    photo_mask->stopAllActions();
    if (needSelDelay) {
        needSelDelay = false;
//        photo_mask->runAction(wySequence::make(wyDelayTime::make(0.2f), wyShow::make(), wyFadeOut::make(0.3f), NULL));
    } else {
        photo_mask->setVisible(true);
        photo_mask->runAction(wyFadeOut::make(0.35f));
    }
//    wyIntervalAction *a = wyScaleTo::make(2.0f, sGlobal->scaleX*2.0f, sGlobal->scaleY*2.0f, sGlobal->scaleX*2.0f*1.2f, sGlobal->scaleY*2.0f*1.2f);
//    highlight->runAction(wyRepeatForever::make(wySequence::make(a, (wyIntervalAction *)a->reverse(), NULL)));
}

void WorldMapScene::showInfo(int objId, float x, float y, float px, float py, LevelNode *node, bool needAnim) {
//    if (!sGlobal->isDebug) {
////        if (objId % 100 == 10) {
////            int realId = objId - 2;
////            if (realId > sGlobal->currentProgress) {
////                return;
////            }
////        }
////        else
//        if (objId > sGlobal->currentProgress) {
//            return;
//        }
//    }
    
    if (objId == currentMapId%100) {
//        hideHighlight();
//        currentMapId = -2;
//        startBtn->setEnabled(false);
//        highscore->setVisible(false);
        highlight->setVisible(true);
        if (needAnim) {
            showHighlight();
        }
    } else {
        currentMapId = objId+this->mapIndex;
        
        if (currentMapId > sGlobal->currentProgress[sGlobal->currentSModeDifficulty] && !sGlobal->isDebug) { // 
            startBtn->setEnabled(false);
            wySpriteEx *disabled = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_go_disabled.png");
            startBtn->setNormalState(disabled);
        } else {
            startBtn->setEnabled(true);
            wySpriteEx *sp = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_go.png");
            startBtn->setNormalState(sp);
        }
        
//        highlight->setPosition(x+DP(0)*sGlobal->scaleY, y-DP(0)*sGlobal->scaleY);
        if (highlight->isVisible()) {
//            wyIntervalAction *a = wyMoveTo::make(0.15f, highlight->getPositionX(), highlight->getPositionY(), x, y);
//            a->setTag(1);
//            highlight->stopAction(1);
//            highlight->runAction(a);
            highlight->setPosition(x+DP(0)*sGlobal->scaleY, y-DP(0)*sGlobal->scaleY);
        } else {
            highlight->setPosition(x+DP(0)*sGlobal->scaleY, y-DP(0)*sGlobal->scaleY);
        }
        highlight->setVisible(true);
        if (needAnim) {
            showHighlight();
        }
        
        wyNode *parent = photo_mask->getParent();
        if (parent != NULL) {
            parent->removeChildLocked(photo_mask, false);
        }
        node->addChildLocked(photo_mask);
        photo_mask->setPosition(node->photo->getPositionX()+DP(36)*sGlobal->scaleX, node->photo->getPositionY()-DP(35)*sGlobal->scaleX);

//        startBtn->setEnabled(true);
        highscore->setVisible(true);
        int seasonId = currentMapId / 100;
        int mapId = currentMapId % 100;
        int scoreIndex = mapId+sGlobal->currentSModeDifficulty*10;
        
        int number_hs = 0, number_pt;
        switch (seasonId) {
            case 1:
                number_hs = sGlobal->s01_score[scoreIndex];
                break;
            case 2:
                number_hs = sGlobal->s02_score[scoreIndex];
                break;
            case 3:
                number_hs = sGlobal->s03_score[scoreIndex];
                break;
            default:
                break;
        }
        char temp[50];
        sprintf(temp, "%s: %d", sGlobal->getStr(STR_HIGHESTSCORE), number_hs);
        highscore->setText(temp);
        highscore->setVisible(true);
        
        levelDesc->setText("the butcher is coming");
        levelDesc->setVisible(true);
    }
//    LOGE("curMapId: %d", currentMapId);
}

void WorldMapScene::showTraining() {
//    handPointer = wySPXSprite::make("hand_pointer.sprite", false, MyTextureManager::makePNG("hand_pointer.png"), 0);
//    handPointer->setForceTickMode(true);
//	handPointer->setUnitInterval(0.3f);
//    handPointer->setLoopCount(-1);
//    handPointer->setScaleX(sGlobal->scaleX*0.8f);
//    handPointer->setScaleY(sGlobal->scaleY*0.8f);
//    LevelNode *node = (LevelNode *)this->getChildByTag(0);
//    handPointer->setPosition(node->getPositionX() + node->bg->getContentSize().width * sGlobal->scaleX / 2 + DP(30)*sGlobal->scaleX, node->getPositionY() + node->bg->getContentSize().height * sGlobal->scaleY / 2 - DP(30)*sGlobal->scaleY);
//    addChildLocked(handPointer, 1);
//    
//    wyTimer *timer = wyTimer::make(wyTargetSelector::make(this, SEL(WorldMapScene::update1)));
//    scheduleLocked(timer);
//    
//    backBtn->setEnabled(false);
}

void WorldMapScene::showTraining1() {
    handPointer->setPosition(startBtn->getPositionX() + DP(30)*sGlobal->scaleX, startBtn->getPositionY() - DP(30)*sGlobal->scaleY);
    
    ((LevelNode*)this->getChildByTag(0))->isEnabled = false;
}

void WorldMapScene::update1(wyTargetSelector *ts) {
    handPointer->tick(ts->getDelta());
}

bool WorldMapScene::keyUp(wyKeyEvent& event) {
    switch(event.keyCode) {
        case KEYCODE_BACK:
            
            return true;
        default:
            break;
    }
    
    return wyScene::keyUp(event);
}

LevelNode::LevelNode(int mapIndex) {
    int seasonId = 0;
    int mapId = mapIndex;
    this->mapIndex = mapIndex;
    this->orgIndex = mapIndex;
    isLocked = false;
    isEnabled = true;
    
    char temp[20];
    
    bg = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
	ControlButton *button2 = ControlButton::make(bg, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(LevelNode::onClick)), mask, NULL);
    button2->setAnchor(0.5, 0.5);
    button2->setInterceptTouch(true);
    button2->setTouchPriority(301);
    button2->setScaleX(sGlobal->scaleX*0.75f);
    button2->setScaleY(sGlobal->scaleY*0.75f);
	addChildLocked(button2);
    button2->needDelay = false;
    
    float width = bg->getWidth()*button2->getScaleX();
    float height = bg->getHeight()*button2->getScaleY();
    
    sprintf(temp, "%d_%d.png", 1, mapId);
    photo = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", temp);
//    photo->setAnchor(1.0f, 0);
    addChildLocked(photo);
    photo->setScaleX(sGlobal->scaleX*0.75f);
    photo->setScaleY(sGlobal->scaleY*0.75f);
    photo->setPosition(DP(-1)*sGlobal->scaleX, DP(7)*sGlobal->scaleY);

    sprintf(temp, "%d - %d", 1, mapId);
    levelNo = wyBitmapFontLabel::make(sGlobal->font_desc_noborder, temp);
    levelNo->setScale(SP(15)*sGlobal->scaleX/sGlobal->descFontSize);
    levelNo->setAlignment(wyBitmapFontLabel::LEFT);
    levelNo->setRotation(-6);
    levelNo->setColor(wyc3b(100, 100, 100));
    levelNo->setPosition(DP(4)*sGlobal->scaleX, -height/2+DP(18)*sGlobal->scaleX);
    addChildLocked(levelNo);
    
    grade = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s_small.png");
    grade->setScaleX(sGlobal->scaleX*1.0f);
    grade->setScaleY(sGlobal->scaleY*1.0f);
    grade->setPosition(DP(32)*sGlobal->scaleX, DP(34)*sGlobal->scaleX);
    addChildLocked(grade, 98);
    
    lock = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "lock.png");
    lock->setScaleX(sGlobal->scaleX*0.7f);
    lock->setScaleY(sGlobal->scaleY*0.7f);
    lock->setPosition(width/2-DP(12)*sGlobal->scaleX, -height/2+DP(12)*sGlobal->scaleX);
    addChildLocked(lock, 99);

    int i = mapIndex-1;
    int j = i / 5;
    float margin = DP(10)*sGlobal->scaleX;
    float yy = DP(135)*sGlobal->scaleY;
    this->setPosition((wyDevice::winWidth-margin*2)/10*(i%5*2+1) + margin, wyDevice::winHeight - yy - DP(140)*j*sGlobal->scaleY);
    posx = this->getPositionX();
    posy = this->getPositionY();
//    this->setAnchor(0.5f, 0.5f);
//    this->setContentSize(bg->getWidth()*bg->getScaleX(), bg->getHeight()*bg->getHeight());
}

LevelNode::~LevelNode() {
    
}

void LevelNode::updateNodeBySeasonId(int seasonId) {
    curSeasonId = seasonId;
    orgIndex = seasonId*100+mapIndex;
    int scoreIndex = mapIndex+sGlobal->currentSModeDifficulty*10;
//    LOGE("orgIndex:%d, progress:%d", orgIndex, sGlobal->currentProgress[sGlobal->currentSModeDifficulty]);
    
    char temp[20];
    sprintf(temp, "%d - %d", seasonId, this->mapIndex);
    levelNo->setText(temp);
    
    sprintf(temp, "%d_%d.png", seasonId, this->mapIndex);
    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", temp);
    wyRect r = frame->getRect();
    photo->setTextureRect(r);
    
    switch (seasonId) {
        case 1:
            getGradeSprite(sGlobal->s01_grade[scoreIndex]);
            break;
        case 2:
            getGradeSprite(sGlobal->s02_grade[scoreIndex]);
            break;
        case 3:
            getGradeSprite(sGlobal->s03_grade[scoreIndex]);
            break;
        default:
            break;
    }
    
    if (orgIndex > sGlobal->currentProgress[sGlobal->currentSModeDifficulty]) {
//        if (!isLocked) {
//            setDisableColor(photo);
//            setDisableColor(bg);
//            setDisableColor(levelNo);
//        }
        
        isLocked = true;
        lock->setVisible(true);
    } else {
//        if (isLocked) {
//            setNormalColor(photo);
//            setNormalColor(bg);
//            setNormalColor(levelNo);
//        }
        lock->setVisible(false);
        isLocked = false;    }
}

void LevelNode::setGrade(int gradeNo) {
    switch (gradeNo) {
        case GRADE_NONE:
            grade->setText("");
            break;
        case GRADE_S:
            grade->setText("s");
            break;
        case GRADE_A:
            grade->setText("a");
            break;
        case GRADE_B:
            grade->setText("b");
            break;
        case GRADE_C:
            grade->setText("c");
            break;
        default:
            grade->setText("");
            break;
    }
}

void LevelNode::onClick(wyTargetSelector* ts) {
//    if (ts != NULL) {
//        sGlobal->playEffect("se_button.mp3");
//    }
    
    bool needAnim = true;

    WorldMapScene *layer = ((WorldMapScene*) this->getParent()->getParent());
    
    if (layer->currentMapId%100 == mapIndex) {
//        this->stopAllActions();
//        this->setScale(1.0f);
        if (!isLocked) {
            layer->onStartButtonClick(NULL);
        }
        
        wyPoint pos = wyp(this->getPositionX(), this->getPositionY() + DP(3)/0.75f*sGlobal->scaleY);
        layer->showInfo(mapIndex, pos.x, pos.y, this->getPositionX()+photo->getPositionX(), this->getPositionY()+photo->getPositionY(), this, needAnim);
        return;
    }
    
    if (layer->currentNode != NULL) {
        layer->currentNode->stopAllActions();
        layer->currentNode->setScale(1.0f);
    }

    wyPoint pos = wyp(this->getPositionX(), this->getPositionY() + DP(3)/0.75f*sGlobal->scaleY);
    layer->showInfo(mapIndex, pos.x, pos.y, this->getPositionX()+photo->getPositionX(), this->getPositionY()+photo->getPositionY(), this, needAnim);
    layer->currentNode = this;
    
    this->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, 1.0f, 1.0f, 1.06f, 1.06f), wyScaleTo::make(0.6f, 1.06f, 1.06f, 1.0f, 1.0f), NULL)));
}

void LevelNode::getGradeSprite(int gradeId) {
    wySpriteEx *sprite;
    bool visible = true;
    switch (gradeId) {
        case GRADE_NONE:
            visible = false;
            break;
        case GRADE_S: {
            wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "score_s_small.png");
            wyRect r = frame->getRect();
            grade->setTextureRect(r);
            break;
        }
        case GRADE_A: {
            wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "score_a_small.png");
            wyRect r = frame->getRect();
            grade->setTextureRect(r);
            break;
        }
        case GRADE_B: {
            wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "score_b_small.png");
            wyRect r = frame->getRect();
            grade->setTextureRect(r);
            break;
        }
        case GRADE_C: {
            wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "score_c_small.png");
            wyRect r = frame->getRect();
            grade->setTextureRect(r);
            break;
        }
        default:
            break;
    }
    grade->setVisible(visible);
//    return sprite;
}

