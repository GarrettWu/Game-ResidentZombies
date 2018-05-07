//
//  UpgradeScene.cpp
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "UpgradeScene.h"
#include "CGScene.h"
#include "Global.h"
#include "ShopLayer.h"
#include "MainScene.h"
#include "LoadingScene.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#include "ArmsPageControl.h"
#include "MyDotPageIndicator.h"

UpgradeScene::UpgradeScene(int mapIndex) {
    wyParticleLoader::load("particle_train_skill.plist");
    
    gotoMapIndex = mapIndex;
    sGlobal->index = gotoMapIndex;
    
    m_bg = wySpriteEx::make(MyTextureManager::makePNG("sc_upgrade_bg.png"));
    m_bg->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    m_bg->setScaleX(wyDevice::winWidth/m_bg->getWidth());
    m_bg->setScaleY(wyDevice::winHeight/m_bg->getHeight());
    addChildLocked(m_bg, 0);
    
    h_line = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "sc_upgrade_bg_line_1.png");
    h_line->setScaleX(m_bg->getScaleX());
    h_line->setScaleY(m_bg->getScaleY());
    h_line->setAnchor(0.5, 0);
    h_line->setPosition(wyDevice::winWidth/2, DP(78)*h_line->getScaleY());
    addChildLocked(h_line, 2);
    
    v_line = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "sc_upgrade_bg_line.png");
    v_line->setScaleX(m_bg->getScaleX());
    v_line->setScaleY(m_bg->getScaleY());
    v_line->setAnchor(0.5, 0);
    v_line->setPosition(DP(482)*v_line->getScaleX(), h_line->getPositionY()+DP(2)*v_line->getScaleY());
    addChildLocked(v_line, 2);
    
    float padding = DP(10)*sGlobal->scaleX;
    leftX = v_line->getPositionX() + v_line->getWidth()*v_line->getScaleX()/2 + padding;
    rightX = wyDevice::winWidth - DP(15)*m_bg->getScaleX() - padding;
    panelWidth = rightX - leftX;
    middleX = leftX + panelWidth/2;

    createArmsPanel();
    createDetailPanelForArms();
    
    createSkillsPanel();
    createDetailPanelForSkills();
    
    createTabs();
    
    createFuncButtons();
    createShopButton();
    
    createMoneyPanel();
    createFreePanel();
    
    initNotice();
    
    char temp[20];
    char temp1[20];
    if (sGlobal->isRealEndless) {
        sprintf(temp, "endless");
        switch (sGlobal->currentEModeDifficulty) {
            case DIFFICULTY_NORMAL:
                sprintf(temp1, "%s", "easy   mode");
                break;
            case DIFFICULTY_HEROIC:
                sprintf(temp1, "%s", "hard   mode");
                break;
            case DIFFICULTY_HARD:
                sprintf(temp1, "%s", "crazy   mode");
                break;
            default:
                break;
        }
    } else {
        sprintf(temp, "level %d-%d", gotoMapIndex/100, gotoMapIndex%100);
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                sprintf(temp1, "%s", "easy   mode");
                break;
            case DIFFICULTY_HEROIC:
                sprintf(temp1, "%s", "hard   mode");
                break;
            case DIFFICULTY_HARD:
                sprintf(temp1, "%s", "crazy   mode");
                break;
            default:
                break;
        }
    }
    levelName = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    levelName->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
    levelName->setAlignment(wyBitmapFontLabel::LEFT);
    levelName->setPosition(leftX-DP(20)*sGlobal->scaleX, wyDevice::winHeight-DP(15)*sGlobal->scaleY);
    addChildLocked(levelName);
    levelName->setColor(wyc3b(255, 49, 0));

    levelMode = wyBitmapFontLabel::make(sGlobal->font_title, temp1);
    levelMode->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    levelMode->setAlignment(wyBitmapFontLabel::LEFT);
    levelMode->setPosition(levelName->getPositionX()+DP(0)*sGlobal->scaleX, wyDevice::winHeight-DP(40)*sGlobal->scaleY);
    levelMode->setColor(wyc3b(170, 170, 170));
    addChildLocked(levelMode);
    
    
//    sGlobal->isAdsRemoved = 1;
    if (sGlobal->isAdsRemoved) {
        tipcount = sizeof(TIPS_STR)/sizeof(TIPS_STR[0]);
        curTipIndex = rand()%tipcount;
        tip1 = wyBitmapFontLabel::make(sGlobal->font_desc, TIPS_STR[curTipIndex]);
        tip1->setScale(DP(13)*sGlobal->scaleX/sGlobal->descFontSize);
        tip1->setAlignment(wyBitmapFontLabel::LEFT);
        tip1->setPosition(wyDevice::winWidth/2, DP(40)*sGlobal->scaleY);
        addChildLocked(tip1);
        
        nextTipIndex = rand()%tipcount;
        while (nextTipIndex == curTipIndex) {
            nextTipIndex = rand()%tipcount;
        }
        tip2 = wyBitmapFontLabel::make(sGlobal->font_desc, TIPS_STR[nextTipIndex]);
        tip2->setScale(DP(13)*sGlobal->scaleX/sGlobal->descFontSize);
        tip2->setAlignment(wyBitmapFontLabel::LEFT);
        tip2->setPosition(wyDevice::winWidth/2, DP(40)*sGlobal->scaleY);
        addChildLocked(tip2);
        tip2->setAlpha(0);
        
        wyTargetSelector* ts = wyTargetSelector::make(this, SEL(UpgradeScene::updateTip));
        wyTimer* t = wyTimer::make(ts);
        scheduleLocked(t);
        
        tipTimer = 0;
        curTip = tip1;
        nextTip = tip2;
    }

    isTraining = false;
    if (sGlobal->isUpgradeTrained == 0) {
        isTraining = true;
    } else {
        if (sGlobal->isToolTrained == 0) {
            isTraining = true;
        }
    }
    
    currentTabIndex = -1;
    onTabArmsClick(NULL);

    setKeyEnabled(true);
    setKeyPriority(1);

    sGlobal->addCoinChangeListener(this);
    
    char temp2[30];
    addComma(sGlobal->getGold(), temp2);
    coin_num->setText(temp2);
    MyPrefs::setInt(DB_GOLD, sGlobal->getGold());
    
	addComma(sGlobal->zuanshi.getValue(), temp);
    cash_num->setText(temp);
    MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
    
    refreshAllArmsGantanhao();
    refreshAllSkillsGantanhao();
}

UpgradeScene::~UpgradeScene() {
    sGlobal->removeCoinChangeListener(this);
}

void UpgradeScene::updateTip(wyTargetSelector *ts) {
    tipTimer += ts->getDelta();
    if (tipTimer > 10.0f) {
        curTip->runAction(wySequence::make(wyFadeOut::make(1.0f), wyCallFunc::make(wyTargetSelector::make(this, SEL(UpgradeScene::changeNextTip))), NULL));
        nextTip->runAction(wyFadeIn::make(1.0f));
        
        wyBitmapFontLabel *tempNode = curTip;
        curTip = nextTip;
        nextTip = tempNode;
        tipTimer = 0;
    }
}

void UpgradeScene::changeNextTip(wyTargetSelector* ts){
    curTipIndex = nextTipIndex;
    nextTipIndex = rand()%tipcount;
    while (nextTipIndex == curTipIndex) {
        nextTipIndex = rand()%tipcount;
    }
//    LOGE("next tip:%d", nextTipIndex);
    nextTip->setText(TIPS_STR[nextTipIndex]);
}

bool UpgradeScene::keyUp(wyKeyEvent& event) {
    switch(event.keyCode) {
        case KEYCODE_BACK:
            return true;
        default:
            break;
    }
    
    return wyScene::keyUp(event);
}

void UpgradeScene::onEnter() {
    wyScene::onEnter();

    if (sGlobal->isUpgradeTrained == 0) {
        trainUpgradeWeapon();
    } else {
        if (sGlobal->isToolTrained == 0) {
            trainSkill();
        }
    }
    
    sGlobal->playBGM(0);
}

void UpgradeScene::onEnterTransitionDidFinish() {
    wyScene::onEnterTransitionDidFinish();
    
#if ANDROID
    showAd(1);
#endif
}

void UpgradeScene::draw() {
    
}

void UpgradeScene::createSkillsPanel() {
    skills_panel = wyNode::make();
    addChildLocked(skills_panel);
    
    int offsetX = MAX(0,(wyDevice::winWidth - 320/2*3*sGlobal->scaleY/0.75f)*0.6f);
    int ofx = offsetX / 4;

    wySpriteEx *normal, *pressed, *cover, *icon, *mask, *cover_mask, *btnmask;
    ControlButton *btn_skill;
    float coverWidth = DP(112);
    float coverHeight = DP(112);
    int x, y, k;
    for (int i = 0; i < 10; i++) {
        if (i > 4) {
            k = i - 1;
        } else {
            k = i;
        }
        
        x = DP(65)*sGlobal->scaleX + DP(133)*sGlobal->scaleX * (k%3) + ofx * (k%3+1);
        y = wyDevice::winHeight - DP(105)*sGlobal->scaleY - DP(100)*sGlobal->scaleY * (k/3);
        
        if (i == 4) {
            x -= DP(36)*sGlobal->scaleX;
            y += DP(28)*sGlobal->scaleX;
        }
        if (i == 5) {
            x += DP(42)*sGlobal->scaleX;
            y -= DP(20)*sGlobal->scaleX;
        }
        if (i == 6) {
            y -= DP(20)*sGlobal->scaleX;
        }
        
        //normal = wySpriteEx::make(MyTextureManager::makePNG("btn_skill_unselected.png"));
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");//wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
        btnmask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");//wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
        btnmask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        
        btn_skill = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(UpgradeScene::onSkillsBtnClick), i), btnmask, NULL);
        btn_skill->setScaleX(sGlobal->scaleX*0.5f);
        btn_skill->setScaleY(sGlobal->scaleY*0.5f);
        btn_skill->setPosition(x, y);
        btn_skill->setMultiTouchClickable(false);
        btn_skill->setInterceptTouch(true);
        btn_skill->setTouchPriority(101);
        skills_panel->addChildLocked(btn_skill, 2, i);
        btn_skill->needDelay = false;
        
        cover = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "cursor.png");
        cover->setColor(wyc3b(0, 0, 0));
        cover->setScaleX(sGlobal->scaleX*0.56f);
        cover->setScaleY(sGlobal->scaleY*0.56f);
        cover->setPosition(x, y);
        skills_panel->addChildLocked(cover, 0);

        float ox = 0, oy = 0;
        bool canUnlock = true;
        switch (i) {
            case SKILL_ROBOT:
                if (sGlobal->skillArray[SKILL_MISSLE].getValue() >= SKILL_ROBOT_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                ox = -6;
                oy = 2;
                break;
            case SKILL_SHIELD:
                if (sGlobal->skillArray[SKILL_ROBOT].getValue() >= SKILL_SHIELD_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                ox = -4;
                oy = 4;
                break;
            case SKILL_ARMOR:
                if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_ARMOR_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                ox = 2;
                oy = 0;
                break;
            case SKILL_STAMINA:
                if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_STAMINA_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                ox = 2;
                oy = 0;
                break;
            case SKILL_EVASION:
                if (sGlobal->skillArray[SKILL_STAMINA].getValue() >= SKILL_EVASION_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                ox = 0;
                oy = 2;
                break;
            case SKILL_CRITICAL:
                if (sGlobal->skillArray[SKILL_FRENZY].getValue() >= SKILL_CRITICAL_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                ox = -4;
                oy = 0;
                break;
            case SKILL_STUN:
                if (sGlobal->skillArray[SKILL_CRITICAL].getValue() >= SKILL_STUN_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                }
                break;
            default:
                break;
        }
        char temp[20];
        if (!canUnlock) {
            sprintf(temp, "skill_%d_disable.png", i);
            btn_skill->userData = 0;
        } else {
            sprintf(temp, "skill_%d.png", i);
            btn_skill->userData = 1;
        }
        
//        wyTexture2D *tex = MyTextureManager::makePNG(temp);
        icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", temp);//wySpriteEx::make(tex);
        icon->setScaleX(sGlobal->scaleX*0.55f);
        icon->setScaleY(sGlobal->scaleY*0.55f);
        icon->setPosition(x-DP(10+ox)*sGlobal->scaleX, y+DP(10+oy)*sGlobal->scaleY);
        skills_panel->addChildLocked(icon, 3, 300+i);

        mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", temp);
        mask->setScaleX(sGlobal->scaleX*0.55f);
        mask->setScaleY(sGlobal->scaleY*0.55f);
        mask->setPosition(icon->getPositionX(), icon->getPositionY());
        skills_panel->addChildLocked(mask, 4, 100+i);
        mask->setVisible(false);
        mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        
        sprintf(temp, "%d/30", sGlobal->skillArray[i].getValue());
        wyBitmapFontLabel *lv = wyBitmapFontLabel::make(sGlobal->font_desc_noborder, temp);
        lv->setScale(SP(13)*sGlobal->scaleX/sGlobal->descFontSize);
        lv->setAlignment(wyBitmapFontLabel::LEFT);
        lv->setPosition(x+coverWidth*btn_skill->getScaleX()/2-DP(25)*sGlobal->scaleX, y-coverHeight*btn_skill->getScaleY()/2+DP(5)*sGlobal->scaleX);
        lv->setAnchor(0.5, 0.5);
        lv->setColor(wyc3b(100, 100, 100));
        lv->setRotation(-6);
        skills_panel->addChildLocked(lv, 2, 200+i);
        if (sGlobal->skillArray[i].getValue() == 0) {
            lv->setVisible(false);
        }
        
        wySpriteEx *gantan = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao.png");//wySprite::make(MyTextureManager::makePNG("gantanhao.png"));
        gantan->setPosition(icon->getPositionX()+DP(32)*sGlobal->scaleX, icon->getPositionY()+DP(20)*sGlobal->scaleY);
        gantan->setVisible(false);
        skills_panel->addChildLocked(gantan, 10, 1000+i);
    }
    
    skills_selectedIndex = sGlobal->activeSkill;
    
    skills_cursor_container = wyNode::make();
    skills_panel->addChildLocked(skills_cursor_container, 1);
    wyNode *skill_0 = skills_panel->getChildByTag(skills_selectedIndex);
    skills_cursor_container->setPosition(skill_0->getPositionX(), skill_0->getPositionY());
    
    skills_cursor_center = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "cursor.png");
    skills_cursor_center->setScaleX(sGlobal->scaleX*0.6f);
    skills_cursor_center->setScaleY(sGlobal->scaleY*0.65f);
    skills_cursor_center->setColor(wyc3b(255, 85, 24));
    skills_cursor_container->addChildLocked(skills_cursor_center);
    
    wyAction *tint = wyRepeatForever::make(wySequence::make(wyTintTo::make(1.0f, 255, 85, 24, 180, 85, 24), wyTintTo::make(1.0f, 180, 85, 24, 255, 85, 24), NULL));
    tint->setTag(0);
    skills_cursor_center->runAction(tint);
    
    skill_badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "equipped.png");
    skill_badge->setScaleX(sGlobal->scaleX*0.65f);
    skill_badge->setScaleY(sGlobal->scaleY*0.65f);
    skills_panel->addChildLocked(skill_badge, 2);
    skill_badge->setPosition(skills_cursor_container->getPositionX()-DP(36)*sGlobal->scaleX, skills_cursor_container->getPositionY()+DP(36)*sGlobal->scaleY);
    
    skill_badge_mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "equipped.png");
    skill_badge_mask->setScaleX(sGlobal->scaleX*0.65f);
    skill_badge_mask->setScaleY(sGlobal->scaleY*0.65f);
    skills_panel->addChildLocked(skill_badge_mask, 2);
    skill_badge_mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    skill_badge_mask->setPosition(skill_badge->getPositionX(), skill_badge->getPositionY());
    
    lineLayer = new LineLayer(this);
    lineLayer->autoRelease();
    skills_panel->addChildLocked(lineLayer, -1);
    
    skills_panel->setVisible(false);
}

static void onArmsPageClicked(ArmsPageControl* pageControl, int index, void* data) {
    UpgradeScene* layer = (UpgradeScene*)data;
    
}

static void onArmsPageChanged(ArmsPageControl* pageControl, int index, void* data) {
    UpgradeScene* layer = (UpgradeScene*)data;
//    LOGE("onArmsPageChanged:%d", index);
    
    switch (index) {
        case 0:
            layer->btn_arms_next->setVisible(true);
            layer->btn_arms_prev->setVisible(false);
            break;
        case 1:
            layer->btn_arms_next->setVisible(true);
            layer->btn_arms_prev->setVisible(true);
            break;
        case 2:
            layer->btn_arms_next->setVisible(false);
            layer->btn_arms_prev->setVisible(true);
            break;
        default:
            break;
    }
    
    layer->currentPageNo = index;
}

static void onArmsPagePositionChanged(ArmsPageControl* pageControl, wyNode* page, float offset, void* data) {
    UpgradeScene *layer = (UpgradeScene*)data;
//    if (page->getTag() == 0 && fabs(offset)<DP(16)*sGlobal->scaleX) {
//        layer->switchSeason(0);
//    }
//    if (page->getTag() == 1 && fabs(offset)<DP(16)*sGlobal->scaleX) {
//        layer->switchSeason(1);
//    }
//    if (page->getTag() == 2 && fabs(offset)<DP(16)*sGlobal->scaleX) {
//        layer->switchSeason(2);
//    }
}

void UpgradeScene::onArmsNextPageClicked(wyTargetSelector *ts) {
//    LOGE("currentPageNo:%d", currentPageNo);
    if (currentPageNo == 2) {
        return;
    }
    currentPageNo++;
    pageControl->gotoPage(currentPageNo, true, 0.5f);
}

void UpgradeScene::onArmsPrevPageClicked(wyTargetSelector *ts) {
    if (currentPageNo == 0) {
        return;
    }
    currentPageNo--;
    pageControl->gotoPage(currentPageNo, true, 0.5f);
}


void UpgradeScene::createArmsPanel() {
    arms_pageWidth = DP(455)*v_line->getScaleX();
    arms_pageHeight = DP(310)*h_line->getScaleY();
    
    arms_panel = wyNode::make();
    addChildLocked(arms_panel);
    
    //屏蔽pagecontrol中的按钮
    wyColorLayer *touchLayer = wyColorLayer::make();
    arms_panel->addChildLocked(touchLayer, 1000);
    touchLayer->setTouchPriority(100);
    touchLayer->setInterceptTouch(true);
    touchLayer->setTouchEnabled(true);
//    touchLayer->setColor(wyc4b(255, 0, 0, 100));
    touchLayer->setContentSize(wyDevice::winWidth-DP(480)*v_line->getScaleX(), wyDevice::winHeight);
    touchLayer->setPosition(DP(480)*v_line->getScaleX(), 0);
    
    wySpriteEx *sp = wySpriteEx::make(MyTextureManager::makePNG("btn_arrow.png"));
    wySpriteEx *mask1 = wySpriteEx::make(MyTextureManager::makePNG("btn_arrow.png"));
    mask1->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_arms_next = ControlButton::make(sp, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onArmsNextPageClicked)), mask1, NULL);
    btn_arms_next->setScaleX(sGlobal->scaleX*0.3f);
    btn_arms_next->setScaleY(sGlobal->scaleY*0.3f);
    btn_arms_next->setMultiTouchClickable(false);
    btn_arms_next->setInterceptTouch(true);
    btn_arms_next->setTouchPriority(101);
    btn_arms_next->setPosition(arms_pageWidth+DP(5)*sGlobal->scaleY, arms_pageHeight/2+DP(100)*sGlobal->scaleY);
    arms_panel->addChildLocked(btn_arms_next, 100);
    btn_arms_next->needDelay = false;
    
    sp = wySpriteEx::make(MyTextureManager::makePNG("btn_arrow.png"));
    sp->setFlipX(true);
    mask1 = wySpriteEx::make(MyTextureManager::makePNG("btn_arrow.png"));
    mask1->setFlipX(true);
    mask1->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_arms_prev = ControlButton::make(sp, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onArmsPrevPageClicked)), mask1, NULL);
    btn_arms_prev->setScaleX(sGlobal->scaleX*0.3f);
    btn_arms_prev->setScaleY(sGlobal->scaleY*0.3f);
    btn_arms_prev->setMultiTouchClickable(false);
    btn_arms_prev->setInterceptTouch(true);
    btn_arms_prev->setTouchPriority(101);
    btn_arms_prev->setPosition(DP(35)*sGlobal->scaleX, arms_pageHeight/2+DP(100)*sGlobal->scaleY);
    arms_panel->addChildLocked(btn_arms_prev, 100);
    btn_arms_prev->needDelay = false;

    wySpriteEx *m_sprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
    m_sprite->setScaleX(sGlobal->scaleX*0.7f);
    m_sprite->setScaleY(sGlobal->scaleY*0.7f);
    arms_spWidth = m_sprite->getWidth()*m_sprite->getScaleX();
    arms_spHeight = m_sprite->getHeight()*m_sprite->getScaleY();
    arms_offsetX = (arms_pageWidth-arms_spWidth*3)/4;
    arms_offsetY = (arms_pageHeight-arms_spHeight*2)/3;
    
    arms_panel1 = wyColorLayer::make();
//    arms_panel1->setColor(wyc4b(255, 0, 0, 255));
    
    arms_panel2 = wyColorLayer::make();
//    arms_panel2->setColor(wyc4b(255, 0, 0, 255));
    
    arms_panel3 = wyColorLayer::make();
//    arms_panel3->setColor(wyc4b(255, 0, 0, 255));
    
    arms_panel1->setContentSize(10, 10);
    arms_panel2->setContentSize(10, 10);
    arms_panel3->setContentSize(10, 10);
    
    arms_panel1->setAnchor(0.5f, 0.5f);
    arms_panel2->setAnchor(0.5f, 0.5f);
    arms_panel3->setAnchor(0.5f, 0.5f);
    
    pageControl = ArmsPageControl::make();
    ArmsPageControlCallback callback = {
        onArmsPageClicked,
        onArmsPageChanged,
        onArmsPagePositionChanged,
    };
    pageControl->setCallback(&callback, this);
    pageControl->setPageSpacing(arms_pageWidth);
    pageControl->setContentSize(arms_pageWidth, arms_pageHeight);
    pageControl->setClipRect(wyr(DP(20)*sGlobal->scaleX, DP(85)*sGlobal->scaleY, arms_pageWidth, arms_pageHeight));
    pageControl->setPosition(DP(20)*sGlobal->scaleX, DP(85)*sGlobal->scaleY);

    pageControl->addPage(arms_panel1);
    pageControl->addPage(arms_panel2);
    pageControl->addPage(arms_panel3);
    pageControl->setTouchPriority(49);
//    pageControl->setColor(wyc4b(0, 255, 0, 100));
    arms_panel->addChildLocked(pageControl);
    
    arms_panel1->setTag(0);
    arms_panel2->setTag(1);
    arms_panel3->setTag(2);
    
    MyDotPageIndicator* indicator = MyDotPageIndicator::make(MyTextureManager::makePNG("dot_unselect.png"), MyTextureManager::makePNG("dot_select.png"));
    indicator->setPosition(arms_pageWidth/2, DP(6)*sGlobal->scaleY);
    indicator->setScaleX(0.42*sGlobal->scaleX);
    indicator->setScaleY(0.42*sGlobal->scaleY);
    indicator->setDotSpacing(24);
    pageControl->setPageIndicator(indicator);
    
    createArmsSubPanel(0);
    createArmsSubPanel(1);
    createArmsSubPanel(2);
    
    arms_selectedIndex = sGlobal->activeWeapon;
    
    int pId;
    int seq = sGlobal->WEAPON_ID2SEQ[sGlobal->activeWeapon];
    if (seq >= 6) {
        if (seq < 12) {
            pId = 1;
        } else {
            pId = 2;
        }
    } else {
        pId = 0;
    }
    pageControl->setInitialPage(pId);
    onArmsPageChanged(pageControl, pId, this);
    currentPageNo = pId;
    
    arms_cursor_container = wyNode::make();
    wyNode* targetPanel = getArmsPanelByWeaponIndex(arms_selectedIndex);
    targetPanel->addChildLocked(arms_cursor_container, 1);
    wyNode *skill_0 = targetPanel->getChildByTag(arms_selectedIndex);
    arms_cursor_container->setPosition(skill_0->getPositionX(), skill_0->getPositionY());
//    LOGE("ax:%f, ay:%f", skill_0->getAbsolutePosition().x, skill_0->getAbsolutePosition().y);
//    LOGE("ax:%f, ay:%f", skill_0->getAbsolutePosition().x, skill_0->getAbsolutePosition().y);
    
    arms_cursor_center = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "cursor.png");
    arms_cursor_center->setColor(wyc3b(255, 85, 24));
    arms_cursor_center->setScaleX(sGlobal->scaleX*0.75f);
    arms_cursor_center->setScaleY(sGlobal->scaleY*0.80f);
    arms_cursor_container->addChildLocked(arms_cursor_center);
    
    wyAction *tint = wyRepeatForever::make(wySequence::make(wyTintTo::make(1.0f, 255, 85, 24, 180, 85, 24), wyTintTo::make(1.0f, 180, 85, 24, 255, 85, 24), NULL));
    tint->setTag(0);
    arms_cursor_center->runAction(tint);
    
    equip_badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "equipped.png");
    equip_badge->setScaleX(sGlobal->scaleX*0.65f);
    equip_badge->setScaleY(sGlobal->scaleY*0.65f);
    targetPanel->addChildLocked(equip_badge, 2);
    equip_badge->setPosition(arms_cursor_container->getPositionX()-DP(38)*sGlobal->scaleX, arms_cursor_container->getPositionY()+DP(38)*sGlobal->scaleY);
    
    equip_badge_mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "equipped.png");
    equip_badge_mask->setScaleX(sGlobal->scaleX*0.65f);
    equip_badge_mask->setScaleY(sGlobal->scaleY*0.65f);
    targetPanel->addChildLocked(equip_badge_mask, 2);
    equip_badge_mask->setPosition(equip_badge->getPositionX(), equip_badge->getPositionY());
    equip_badge_mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
}

wyNode* UpgradeScene::getArmsPanelByWeaponIndex(int index) {
    int seq = sGlobal->WEAPON_ID2SEQ[index];
//    LOGE("weapon index:%d, %d", index, seq);
    if (seq >= 6) {
        if (seq < 12) {
//            LOGE("returning panel2...");
            return arms_panel2;
        } else {
//            LOGE("returning panel3...");
            return arms_panel3;
        }
    } else {
//        LOGE("returning panel1...");
        return arms_panel1;
    }
}

wyNode* UpgradeScene::getArmsPanelByPageIndex(int index) {
    switch (index) {
        case 0:
            return arms_panel1;
            break;
        case 1:
            return arms_panel2;
            break;
        case 2:
            return arms_panel3;
            break;
        default:
            break;
    }
}

void UpgradeScene::createArmsSubPanel(int pageId) {
    wyNode *panel = getArmsPanelByPageIndex(pageId);
    wySpriteBatchNode *starContainer = wySpriteBatchNode::make(MyTextureManager::makePNG("ui_en.png"));
    panel->addChildLocked(starContainer, 10, 3000);
    
    wySpriteEx *normal, *pressed, *cover, *btnmask;
    wyNode *icon, *mask;
    ControlButton *btn_skill;
    float coverWidth = DP(112);
    float coverHeight = DP(112);
    int x, y;
    int startNo = pageId*6;
    int endNo = MIN(16, pageId*6+6);
    for (int i = startNo; i < endNo; i++) {
        int weaponId = sGlobal->WEAPON_SEQ2ID[i];
//        LOGE("creating gun:%d, %d, %d", pageId, i, weaponId);
        switch (i%6) {
            case 0:
                x = -arms_offsetX-arms_spWidth;
                y = arms_offsetY/2+arms_spHeight/2;
                break;
            case 1:
                x = 0;
                y = arms_offsetY/2+arms_spHeight/2;
                break;
            case 2:
                x = arms_offsetX+arms_spWidth;
                y = arms_offsetY/2+arms_spHeight/2;
                break;
            case 3:
                x = -arms_offsetX-arms_spWidth;
                y = -arms_offsetY/2-arms_spHeight/2;
                break;
            case 4:
                x = 0;
                y = -arms_offsetY/2-arms_spHeight/2;
                break;
            case 5:
                x = arms_offsetX+arms_spWidth;
                y = -arms_offsetY/2-arms_spHeight/2;
                break;
            default:
                break;
        }
        y+=DP(20)*sGlobal->scaleY;
        
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");//wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
        btnmask =wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");//wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cover.png");
        btnmask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        btn_skill = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(UpgradeScene::onArmsBtnClick), weaponId), btnmask, NULL);
        btn_skill->setScaleX(sGlobal->scaleX*0.6f);
        btn_skill->setScaleY(sGlobal->scaleY*0.6f);
        btn_skill->setPosition(x, y);
        btn_skill->setMultiTouchClickable(false);
        btn_skill->setInterceptTouch(false);
        btn_skill->setTouchPriority(50);
        panel->addChildLocked(btn_skill, 2, weaponId);
        btn_skill->needDelay = false;
        
        cover = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "cursor.png");
        cover->setColor(wyc3b(0, 0, 0));
        cover->setScaleX(sGlobal->scaleX*0.65f);
        cover->setScaleY(sGlobal->scaleY*0.65f);
        cover->setPosition(x, y);
        panel->addChildLocked(cover, 0);
        
        icon = sGlobal->getArmsSpriteByWeaponIndex(weaponId);
        icon->setPosition(x-DP(4)*sGlobal->scaleX, y+DP(0)*sGlobal->scaleY);
        panel->addChildLocked(icon, 3);
        
        mask = sGlobal->getArmsSpriteByWeaponIndex(weaponId);
        mask->setPosition(icon->getPositionX(), icon->getPositionY());
        mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        mask->setVisible(false);
        panel->addChildLocked(mask, 4, 100+weaponId);
        
        char temp[20];
        sprintf(temp, "%d/10", sGlobal->weaponUnlockArray[weaponId].getValue());
        wyBitmapFontLabel *lv = wyBitmapFontLabel::make(sGlobal->font_desc_noborder, temp);
        lv->setScale(SP(15)*sGlobal->scaleX/sGlobal->descFontSize);
        lv->setAlignment(wyBitmapFontLabel::LEFT);
        lv->setPosition(x+coverWidth*btn_skill->getScaleX()/2-DP(30)*sGlobal->scaleX, y-coverHeight*btn_skill->getScaleY()/2+DP(18)*sGlobal->scaleX);
        lv->setAnchor(0.5, 0.5);
        lv->setColor(wyc3b(210, 210, 210));
        lv->setRotation(-17);
        panel->addChildLocked(lv, 2, 200+weaponId);
        if (sGlobal->weaponUnlockArray[weaponId].getValue() == 0) {
            lv->setVisible(false);
        }
        lv->setAlpha(0);
        
        wySpriteEx *gantan = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao.png");
        gantan->setPosition(icon->getPositionX()+DP(35)*sGlobal->scaleX, icon->getPositionY()-DP(20)*sGlobal->scaleY);
        gantan->setVisible(false);
        panel->addChildLocked(gantan, 10, 1000+weaponId);
//        LOGE("creating gantan:%d, %d", i, 1000+i);
        
        bool needHideAllStars = false;
        if (sGlobal->weaponUnlockArray[weaponId].getValue()==0) {
            needHideAllStars = true;
        }
        int idxx = sGlobal->weaponUnlockArray[weaponId].getValue()/2;
        for (int j = 0; j < 5; j++) {
            if (j >= idxx) {
                wySpriteEx *empty_star = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star_empty.png");
                empty_star->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*j, btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
                starContainer->addChildLocked(empty_star, 10, weaponId*1000+j);
                empty_star->setScaleX(sGlobal->scaleX*0.4f);
                empty_star->setScaleY(sGlobal->scaleY*0.4f);
                empty_star->setVisible(!needHideAllStars);
            } else {
                wySpriteEx *star = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star.png");
                star->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*j, btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
                starContainer->addChildLocked(star, 10);
                star->setScaleX(sGlobal->scaleX*0.4f);
                star->setScaleY(sGlobal->scaleY*0.4f);
            }
        }
        bool needHalfStar = (idxx*2 != sGlobal->weaponUnlockArray[weaponId].getValue());
        if (needHalfStar) {
            wySpriteEx *star = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star_left.png");
            star->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*(idxx+0), btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
            starContainer->addChildLocked(star, 10);
            star->setScaleX(sGlobal->scaleX*0.4f);
            star->setScaleY(sGlobal->scaleY*0.4f);
        }
    }
}

void UpgradeScene::createTabs() {
    wySpriteEx *normal, *mask;
    wyBitmapFontLabel *title;
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "arms");
    title->setScale(SP(26)/sGlobal->descFontSize);
    tab_arms_bg = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onTabArmsClick)), mask, title);
    tab_arms_bg->setScaleX(sGlobal->scaleX*0.8f);
    tab_arms_bg->setScaleY(sGlobal->scaleY*0.8f);
    tab_arms_bg->setPosition(DP(85)*sGlobal->scaleX, wyDevice::winHeight-DP(26)*sGlobal->scaleY);
    tab_arms_bg->setMultiTouchClickable(false);
    tab_arms_bg->setInterceptTouch(true);
    tab_arms_bg->setTouchPriority(101);
    addChildLocked(tab_arms_bg);
    tab_arms_bg->needDelay = false;
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "skills");
    title->setScale(SP(26)/sGlobal->descFontSize);
    tab_upgrades_bg = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onTabSkillsClick)), mask, title);
    tab_upgrades_bg->setScaleX(sGlobal->scaleX*0.8f);
    tab_upgrades_bg->setScaleY(sGlobal->scaleY*0.8f);
    tab_upgrades_bg->setPosition(tab_arms_bg->getPositionX()+DP(125)*sGlobal->scaleX, tab_arms_bg->getPositionY()-DP(0)*sGlobal->scaleY);
    tab_upgrades_bg->setMultiTouchClickable(false);
    tab_upgrades_bg->setInterceptTouch(true);
    tab_upgrades_bg->setTouchPriority(101);
    addChildLocked(tab_upgrades_bg);
    tab_upgrades_bg->needDelay = false;
    
    arms_gantanhao = wyNode::make();
    arms_gantanhao->setPosition(tab_arms_bg->getPositionX()+DP(45)*sGlobal->scaleX, tab_arms_bg->getPositionY()-DP(16)*sGlobal->scaleY);
    arms_gantanhao->setVisible(false);
    addChildLocked(arms_gantanhao);
    wySpriteEx *arms_gantanhao_sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao_empty.png");
    arms_gantanhao->addChildLocked(arms_gantanhao_sp);
    arms_gantanhao_number = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    arms_gantanhao_number->setScale(SP(22)/sGlobal->descFontSize);
    arms_gantanhao->addChildLocked(arms_gantanhao_number);
    
    skills_gantanhao = wyNode::make();
    skills_gantanhao->setPosition(tab_upgrades_bg->getPositionX()+DP(45)*sGlobal->scaleX, tab_upgrades_bg->getPositionY()-DP(16)*sGlobal->scaleY);
    skills_gantanhao->setVisible(false);
    addChildLocked(skills_gantanhao);
    wySpriteEx *skills_gantanhao_sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "gantanhao_empty.png");
    skills_gantanhao->addChildLocked(skills_gantanhao_sp);
    skills_gantanhao_number = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    skills_gantanhao_number->setScale(SP(22)/sGlobal->descFontSize);
    skills_gantanhao->addChildLocked(skills_gantanhao_number);
}

void UpgradeScene::createFuncButtons() {
    wySpriteEx *normal, *mask;
    
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");
    normal->setFlipX(true);
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    mask->setFlipX(true);
    btn_back = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onBackBtnClick)), mask, NULL);
    btn_back->setScaleX(sGlobal->scaleX*0.7f);
    btn_back->setScaleY(sGlobal->scaleY*0.7f);
    btn_back->setPosition(DP(55)*sGlobal->scaleX, DP(38)*sGlobal->scaleY);
    btn_back->setMultiTouchClickable(false);
    btn_back->setInterceptTouch(true);
    btn_back->setTouchPriority(101);
    btn_back->setRotation(-90);
    addChildLocked(btn_back);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_go.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_go.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_start = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onStartBtnClick)), mask, NULL);
    btn_start->setScaleX(sGlobal->scaleX*0.7f);
    btn_start->setScaleY(sGlobal->scaleY*0.7f);
    btn_start->setPosition(wyDevice::winWidth-DP(55)*sGlobal->scaleX, DP(38)*sGlobal->scaleY);
    btn_start->setMultiTouchClickable(false);
    btn_start->setInterceptTouch(true);
    btn_start->setTouchPriority(101);
    addChildLocked(btn_start);
}

void UpgradeScene::createShopButton() {

}

void UpgradeScene::createMoneyPanel() {
//    wyBitmapFont* font = wyBitmapFont::loadFont("font_tc_en"), BFF_ANGELCODE_XML);
    
    wySpriteEx *coin_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    coin_icon->setScaleX(sGlobal->scaleX*0.7f);
    coin_icon->setScaleY(sGlobal->scaleY*0.7f);
    coin_icon->setPosition(wyDevice::winWidth-DP(160)*sGlobal->scaleX, wyDevice::winHeight-DP(15)*sGlobal->scaleY);
    addChildLocked(coin_icon);
    
    coin_num = wyBitmapFontLabel::make(sGlobal->font_desc, "0");
    coin_num->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    coin_num->setAlignment(wyBitmapFontLabel::LEFT);
    coin_num->setPosition(coin_icon->getPositionX()+DP(20)*sGlobal->scaleX, coin_icon->getPositionY());
    coin_num->setAnchor(0, 0.5f);
    addChildLocked(coin_num);
    
    wySpriteEx *cash_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    cash_icon->setScaleX(sGlobal->scaleX*0.7f);
    cash_icon->setScaleY(sGlobal->scaleY*0.7f);
    cash_icon->setPosition(coin_icon->getPositionX()-DP(0)*sGlobal->scaleX, coin_icon->getPositionY()-DP(25)*sGlobal->scaleY);
    addChildLocked(cash_icon);
    
    cash_num = wyBitmapFontLabel::make(sGlobal->font_desc, "0");
    cash_num->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    cash_num->setAlignment(wyBitmapFontLabel::LEFT);
    cash_num->setPosition(cash_icon->getPositionX()+DP(20)*sGlobal->scaleX, cash_icon->getPositionY());
    cash_num->setAnchor(0, 0.5f);
    addChildLocked(cash_num);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_shop.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_shop.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_shop = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onShopBtnClick)), mask, NULL);
    btn_shop->setScaleX(sGlobal->scaleX*0.75f);
    btn_shop->setScaleY(sGlobal->scaleY*0.75f);
    btn_shop->setPosition(coin_icon->getPositionX()+DP(120)*sGlobal->scaleX, coin_icon->getPositionY()-DP(12)*sGlobal->scaleY);
    btn_shop->setMultiTouchClickable(false);
    btn_shop->setInterceptTouch(true);
    btn_shop->setTouchPriority(101);
    addChildLocked(btn_shop);
    
    shop_sale = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_sale.png");
    shop_sale->setScaleX(sGlobal->scaleX*0.7f);
    shop_sale->setScaleY(sGlobal->scaleY*0.7f);
    shop_sale->setAnchor(0.5f, 1.0f);
    shop_sale->setPosition(btn_shop->getPositionX(), btn_shop->getPositionY()-DP(10)*sGlobal->scaleY);
    addChildLocked(shop_sale);
    if (sGlobal->isBigPackSoldout) {
        shop_sale->setVisible(false);
    } else {
        shop_sale->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(1.0f, 0, -DP(6)*sGlobal->scaleY), wyMoveBy::make(1.0f, 0, DP(6)*sGlobal->scaleY), NULL)));
    }
}

void UpgradeScene::createFreePanel() {
    freeContainer = wyNode::make();
    freeContainer->setAnchor(0, 0);
    addChildLocked(freeContainer, 100);
    freeContainer->setPosition(DP(357)*sGlobal->scaleX, DP(102)*sGlobal->scaleY);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "free_btn_bg.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "free_btn_bg.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    free_button_bg = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onFreeBtnClick)), mask, NULL);
    free_button_bg->setScaleX(sGlobal->scaleX*0.56f);
    free_button_bg->setScaleY(sGlobal->scaleY*0.56f);
    free_button_bg->setMultiTouchClickable(false);
    free_button_bg->setInterceptTouch(true);
    free_button_bg->setTouchPriority(101);
    freeContainer->addChildLocked(free_button_bg);
    free_button_bg->setAnchor(1.0f, 0);

    free_star = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "free_star.png");
    free_star->setScaleX(sGlobal->scaleX*0.56f);
    free_star->setScaleY(sGlobal->scaleY*0.56f);
    freeContainer->addChildLocked(free_star,1);
    free_star->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    free_star->setVisible(false);
    
    free_word = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "free_title.png");
    free_word->setScaleX(sGlobal->scaleX*0.54f);
    free_word->setScaleY(sGlobal->scaleY*0.54f);
    freeContainer->addChildLocked(free_word,1);
    free_word->setPosition(-DP(44)*sGlobal->scaleX, DP(44)*sGlobal->scaleX);
//    free_word->setContentSize(free_word->getWidth()*free_word->getScale()*1.2f, free_word->getHeight()*free_word->getScale()*1.2f);
    free_word->runAction(wyRepeatForever::make(wySequence::make(wyRotateTo::make(1.0f, -10, 10), wyRotateTo::make(1.0f, 10, -10), NULL)));

//    free_light = wySpriteEx::make(MyTextureManager::makePNG("free_light.png"));
//    free_light->setScaleX(0.6f*sGlobal->scaleX);
//    free_light->setScaleY(0.6f*sGlobal->scaleY);
//    freeContainer->addChildLocked(free_light);
//    free_light->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });

    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(UpgradeScene::updateTimer));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);
    
    starTimer = 0;
    nextStarTime = 1.0f;
    lightTimer = 0;
    nextlightTime = 2.0f;
}

void UpgradeScene::updateTimer(wyTargetSelector *ts) {
    float dt = ts->getDelta();
    
    starTimer += dt;
    if (starTimer > nextStarTime) {
        free_star->runAction(wySequence::make(wyShow::make(), wyRotateBy::make(2.0f, 360*2), wyHide::make(), NULL));
        float tx = sGlobal->scaleX*0.8f;
        free_star->runAction(wySequence::make(wyScaleTo::make(1.0f, 0,0, tx,tx), wyScaleTo::make(1.0f, tx,tx, 0,0), NULL));
        
        starTimer = 0;
        nextStarTime = randRange(2, 3);
        if (rand()%2 == 0) {
            free_star->setPosition(-DP(20)*sGlobal->scaleX, DP(30)*sGlobal->scaleY);
        } else {
            free_star->setPosition(-DP(30)*sGlobal->scaleX, DP(20)*sGlobal->scaleY);
        }
        free_star->setVisible(true);
    }
    
//    lightTimer += dt;
//    if (lightTimer > nextlightTime) {
//        free_light->runAction(wyMoveTo::make(0.5f, -DP(50)*sGlobal->scaleX, DP(30)*sGlobal->scaleY, DP(0)*sGlobal->scaleX, DP(30)*sGlobal->scaleY));
//        free_light->runAction(wySequence::make(wyShow::make(), wyScaleTo::make(0.5f, 0.6f*sGlobal->scaleX, 0.3f*sGlobal->scaleX, 0.6f*sGlobal->scaleX, 0.8f*sGlobal->scaleX), wyHide::make(), NULL));
//        free_light->setScale(0.6f*sGlobal->scaleX);
//        
//        lightTimer = 0;
//        nextlightTime = randRange(2, 5);
//    }
}

void UpgradeScene::createDetailPanelForArms() {
    wySpriteEx *normal, *mask;
    
    arms_detail_panel = wyNode::make();
    addChildLocked(arms_detail_panel);
    
    arms_title = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    arms_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_title->setAlignment(wyBitmapFontLabel::LEFT);
    arms_title->setPosition(middleX, wyDevice::winHeight - DP(75)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(arms_title);
    
    arms_title_bg = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "upgrade_title_bg.png");
    arms_title_bg->setScaleX(0.6f*sGlobal->scaleX);
    arms_title_bg->setScaleY(0.6f*sGlobal->scaleY);
    arms_title_bg->setPosition(arms_title->getPositionX(), arms_title->getPositionY());
    arms_detail_panel->addChildLocked(arms_title_bg, -1);

    arms_desc = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    arms_desc->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_desc->setAlignment(wyBitmapFontLabel::LEFT);
    arms_desc->setLineWidth(panelWidth/arms_desc->getScale());
    arms_desc->setAnchor(0.0f, 1.0f);
    arms_desc->setPosition(leftX, arms_title->getPositionY() - DP(22)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(arms_desc);
    arms_desc->setColor(wyc3b(220, 220, 220));
    
    arms_desc_sub = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    arms_desc_sub->setScale(SP(12)*sGlobal->scaleX/36);
    arms_desc_sub->setAlignment(wyBitmapFontLabel::LEFT);
    arms_desc_sub->setAnchor(0.0f, 1.0f);
    arms_desc_sub->setPosition(leftX, arms_desc->getPositionY()-DP(32)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(arms_desc_sub);
    arms_desc_sub->setColor(wyc3b(0, 200, 0));
    
    atk_progress_title = wyBitmapFontLabel::make(sGlobal->font_desc, "atk: ");
    atk_progress_title->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    atk_progress_title->setAlignment(wyBitmapFontLabel::LEFT);
    atk_progress_title->setPosition(leftX, arms_desc->getPositionY()-DP(60)*sGlobal->scaleY);
    atk_progress_title->setAnchor(0, 0.5f);
    arms_detail_panel->addChildLocked(atk_progress_title);
    
    float titleWidth = DP(45)*sGlobal->scaleX;
    
    wySpriteEx *progress_border = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "progressbar_bottom.png");
    float progress_scalex = (panelWidth-titleWidth)/progress_border->getWidth();
    
    progress_border->setAnchor(0.0f, 0.5f);
    progress_border->setScaleX(progress_scalex);
    progress_border->setScaleY(0.7f*sGlobal->scaleY);
    progress_border->setPosition(atk_progress_title->getPositionX()+titleWidth, atk_progress_title->getPositionY()-DP(0)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(progress_border);
    
    atk_progress = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
    atk_progress->setStyle(HORIZONTAL_BAR_LR);
    atk_progress->setAnchor(0, 0.5f);
    atk_progress->setPosition(progress_border->getPositionX(), progress_border->getPositionY());
    atk_progress->setScaleX(progress_scalex);
    atk_progress->setScaleY(0.7f*sGlobal->scaleX);
    arms_detail_panel->addChildLocked(atk_progress);
    atk_progress->setPercentage(0);
    
    speed_progress_title = wyBitmapFontLabel::make(sGlobal->font_desc, "speed: ");
    speed_progress_title->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    speed_progress_title->setAlignment(wyBitmapFontLabel::LEFT);
    speed_progress_title->setPosition(atk_progress_title->getPositionX(), atk_progress_title->getPositionY()-DP(20)*sGlobal->scaleY);
    speed_progress_title->setAnchor(0, 0.5f);
    arms_detail_panel->addChildLocked(speed_progress_title);
    
    progress_border = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "progressbar_bottom.png");
    progress_border->setAnchor(0.0f, 0.5f);
    progress_border->setScaleX(progress_scalex);
    progress_border->setScaleY(0.7f*sGlobal->scaleX);
    progress_border->setPosition(atk_progress->getPositionX(), speed_progress_title->getPositionY()-DP(0)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(progress_border);
    
    speed_progress = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
    speed_progress->setStyle(HORIZONTAL_BAR_LR);
    speed_progress->setAnchor(0, 0.5f);
    speed_progress->setPosition(progress_border->getPositionX(), progress_border->getPositionY());
    speed_progress->setScaleX(progress_scalex);
    speed_progress->setScaleY(0.7f*sGlobal->scaleX);
    arms_detail_panel->addChildLocked(speed_progress);
    speed_progress->setPercentage(0);
    
    cost_progress_title = wyBitmapFontLabel::make(sGlobal->font_desc, "cost: ");
    cost_progress_title->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    cost_progress_title->setAlignment(wyBitmapFontLabel::LEFT);
    cost_progress_title->setPosition(speed_progress_title->getPositionX(), speed_progress_title->getPositionY()-DP(20)*sGlobal->scaleY);
    cost_progress_title->setAnchor(0, 0.5f);
    arms_detail_panel->addChildLocked(cost_progress_title);
    
    progress_border = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "progressbar_bottom.png");
    progress_border->setAnchor(0.0f, 0.5f);
    progress_border->setScaleX(progress_scalex);
    progress_border->setScaleY(0.7f*sGlobal->scaleX);
    progress_border->setPosition(atk_progress->getPositionX(), cost_progress_title->getPositionY()-DP(0)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(progress_border);
    
    cost_progress = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
    cost_progress->setStyle(HORIZONTAL_BAR_LR);
    cost_progress->setAnchor(0, 0.5f);
    cost_progress->setPosition(progress_border->getPositionX(), progress_border->getPositionY());
    cost_progress->setScaleX(progress_scalex);
    cost_progress->setScaleY(0.7f*sGlobal->scaleX);
    arms_detail_panel->addChildLocked(cost_progress);
    cost_progress->setPercentage(0);
    
    wySpriteEx *atk_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "attack_icon.png");
    atk_icon->setScaleX(sGlobal->scaleX*0.5f);
    atk_icon->setScaleY(sGlobal->scaleY*0.5f);
    atk_icon->setAnchor(0, 0.5f);
    atk_icon->setPosition(leftX - DP(4)*sGlobal->scaleX, arms_desc->getPositionY() - DP(128)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(atk_icon);
    
    arms_atk_prop = wyBitmapFontLabel::make(sGlobal->font_desc, "10 > 15");
    arms_atk_prop->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_atk_prop->setAlignment(wyBitmapFontLabel::LEFT);
    arms_atk_prop->setPosition(atk_icon->getPositionX()+DP(34)*sGlobal->scaleX, atk_icon->getPositionY() - DP(0)*sGlobal->scaleY);
    arms_atk_prop->setAnchor(0, 0.5f);
    arms_detail_panel->addChildLocked(arms_atk_prop);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "upgrade");
    title->setScale(SP(26)/sGlobal->descFontSize);
    arms_atk_bg = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onArmUpgradeBtnClick)), mask, title);
    arms_atk_bg->setScaleX(sGlobal->scaleX*0.6f);
    arms_atk_bg->setScaleY(sGlobal->scaleY*0.6f);
    arms_atk_bg->setPosition(rightX-DP(55)*sGlobal->scaleX, atk_icon->getPositionY()-DP(30)*sGlobal->scaleY);
    arms_atk_bg->setMultiTouchClickable(false);
    arms_atk_bg->setInterceptTouch(true);
    arms_atk_bg->setTouchPriority(101);
    arms_detail_panel->addChildLocked(arms_atk_bg);

    coin_icon1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    coin_icon1->setScaleX(sGlobal->scaleX*0.8f);
    coin_icon1->setScaleY(sGlobal->scaleY*0.8f);
    coin_icon1->setAnchor(1.0, 0.5f);
    coin_icon1->setPosition(arms_atk_bg->getPositionX()-DP(110)*sGlobal->scaleX, arms_atk_bg->getPositionY());
    arms_detail_panel->addChildLocked(coin_icon1);
    
    cash_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    cash_icon->setScaleX(sGlobal->scaleX*0.8f);
    cash_icon->setScaleY(sGlobal->scaleY*0.8f);
    cash_icon->setAnchor(1.0, 0.5f);
    cash_icon->setPosition(coin_icon1->getPositionX()+DP(0)*sGlobal->scaleX, coin_icon1->getPositionY()-DP(0)*sGlobal->scaleY);
    arms_detail_panel->addChildLocked(cash_icon);
    
    arms_atk_gold = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    arms_atk_gold->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_atk_gold->setAlignment(wyBitmapFontLabel::LEFT);
    arms_atk_gold->setPosition(coin_icon1->getPositionX() + DP(2)*sGlobal->scaleX, coin_icon1->getPositionY());
    arms_atk_gold->setAnchor(0.0, 0.5f);
    arms_detail_panel->addChildLocked(arms_atk_gold);
    
    arms_ammo_container = wyNode::make();
    arms_detail_panel->addChildLocked(arms_ammo_container);
    
    arms_ammo_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "bullet_icon.png");
    arms_ammo_icon->setScaleX(sGlobal->scaleX*0.4f);
    arms_ammo_icon->setScaleY(sGlobal->scaleY*0.4f);
    arms_ammo_icon->setAnchor(0, 0.5f);
    arms_ammo_icon->setPosition(leftX, coin_icon1->getPositionY() - DP(35)*sGlobal->scaleY);
    arms_ammo_container->addChildLocked(arms_ammo_icon);
    
    arms_ammo_prop = wyBitmapFontLabel::make(sGlobal->font_desc, "50 > 100");
    arms_ammo_prop->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_ammo_prop->setAlignment(wyBitmapFontLabel::LEFT);
    arms_ammo_prop->setPosition(arms_ammo_icon->getPositionX()+DP(30)*sGlobal->scaleX, arms_ammo_icon->getPositionY() - DP(0)*sGlobal->scaleY);
    arms_ammo_prop->setAnchor(0, 0.5f);
    arms_ammo_container->addChildLocked(arms_ammo_prop);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "buy");
    title->setScale(SP(26)/sGlobal->descFontSize);
    arms_ammo_bg = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onArmAmmoBtnClick)), mask, title);
    arms_ammo_bg->setScaleX(sGlobal->scaleX*0.6f);
    arms_ammo_bg->setScaleY(sGlobal->scaleY*0.6f);
    arms_ammo_bg->setPosition(arms_atk_bg->getPositionX()+DP(0)*sGlobal->scaleX, arms_ammo_icon->getPositionY()-DP(30)*sGlobal->scaleY);
    arms_ammo_bg->setMultiTouchClickable(false);
    arms_ammo_bg->setInterceptTouch(true);
    arms_ammo_bg->setTouchPriority(101);
    arms_ammo_container->addChildLocked(arms_ammo_bg);
    
    coin_icon2 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    coin_icon2->setScaleX(sGlobal->scaleX*0.8f);
    coin_icon2->setScaleY(sGlobal->scaleY*0.8f);
    coin_icon2->setAnchor(1.0f, 0.5f);
    coin_icon2->setPosition(arms_ammo_bg->getPositionX()-DP(110)*sGlobal->scaleX, arms_ammo_bg->getPositionY()-DP(0)*sGlobal->scaleY);
    arms_ammo_container->addChildLocked(coin_icon2);
    
    arms_ammo_gold = wyBitmapFontLabel::make(sGlobal->font_desc, "500");
    arms_ammo_gold->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_ammo_gold->setAlignment(wyBitmapFontLabel::LEFT);
    arms_ammo_gold->setPosition(coin_icon2->getPositionX() + DP(2)*sGlobal->scaleY, coin_icon2->getPositionY());
    arms_ammo_gold->setAnchor(0, 0.5f);
    arms_ammo_container->addChildLocked(arms_ammo_gold);
    
    arms_lvmax_container = wyNode::make();
    arms_lvmax_container->setVisible(false);
    arms_lvmax_container->setPosition(middleX, coin_icon1->getPositionY());
    arms_detail_panel->addChildLocked(arms_lvmax_container);
    
    arms_lvmax_title = wyBitmapFontLabel::make(sGlobal->font_desc, "level max");
    arms_lvmax_title->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    arms_lvmax_title->setAlignment(wyBitmapFontLabel::LEFT);
    arms_lvmax_title->setPosition(0, 0);
    arms_lvmax_container->addChildLocked(arms_lvmax_title);
    
    arms_lvmax_bg = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "upgrade_title_bg.png");
    arms_lvmax_bg->setScaleX(0.5f*sGlobal->scaleX);
    arms_lvmax_bg->setScaleY(0.5f*sGlobal->scaleY);
    arms_lvmax_bg->setPosition(0, 0);
    arms_lvmax_container->addChildLocked(arms_lvmax_bg, -1);
}

void UpgradeScene::createDetailPanelForSkills() {
    wySpriteEx *normal, *mask;
    wyBitmapFontLabel *title;
    
    skills_detail_panel = wyNode::make();
    addChildLocked(skills_detail_panel);
    
    skills_title = wyBitmapFontLabel::make(sGlobal->font_desc, "title");
    skills_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    skills_title->setAlignment(wyBitmapFontLabel::LEFT);
    skills_title->setPosition(middleX, wyDevice::winHeight - DP(75)*sGlobal->scaleY);
    skills_detail_panel->addChildLocked(skills_title);
    
    skills_title_bg = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "upgrade_title_bg.png");
    skills_title_bg->setScaleX(0.6f*sGlobal->scaleX);
    skills_title_bg->setScaleY(0.6f*sGlobal->scaleY);
    skills_title_bg->setPosition(arms_title->getPositionX(), arms_title->getPositionY());
    skills_detail_panel->addChildLocked(skills_title_bg, -1);
    
    skills_desc = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    skills_desc->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    skills_desc->setAlignment(wyBitmapFontLabel::LEFT);
    skills_desc->setLineWidth(panelWidth/skills_desc->getScale());
    skills_desc->setAnchor(0.0f, 1.0f);
    skills_desc->setPosition(leftX, skills_title->getPositionY() - DP(22)*sGlobal->scaleY);
    skills_detail_panel->addChildLocked(skills_desc);
    skills_desc->setColor(wyc3b(220, 220, 220));
    
    skill_prop_panel = wyNode::make();
    skills_detail_panel->addChildLocked(skill_prop_panel);
    skill_prop_panel->setPosition(leftX, skills_desc->getPositionY() - DP(80)*sGlobal->scaleY);
    
    skill_up_panel = wyNode::make();
    skills_detail_panel->addChildLocked(skill_up_panel);
    skill_up_panel->setPosition(0,0);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "upgrade");
    title->setScale(SP(26)/sGlobal->descFontSize);
    skills_upgrades_bg = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(UpgradeScene::onSkillUpgradeBtnClick)), mask, title);
    skills_upgrades_bg->setScaleX(sGlobal->scaleX*0.6f);
    skills_upgrades_bg->setScaleY(sGlobal->scaleY*0.6f);
    skills_upgrades_bg->setPosition(rightX-DP(55)*sGlobal->scaleX, skill_prop_panel->getPositionY()-DP(30)*sGlobal->scaleY);
    skills_upgrades_bg->setMultiTouchClickable(false);
    skills_upgrades_bg->setInterceptTouch(true);
    skills_upgrades_bg->setTouchPriority(101);
    skill_up_panel->addChildLocked(skills_upgrades_bg);
    
    skills_coin_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    skills_coin_icon->setScaleX(sGlobal->scaleX*0.8f);
    skills_coin_icon->setScaleY(sGlobal->scaleY*0.8f);
    skills_coin_icon->setAnchor(1.0, 0.5f);
    skills_coin_icon->setPosition(skills_upgrades_bg->getPositionX()-DP(110)*sGlobal->scaleX, skills_upgrades_bg->getPositionY());
    skill_up_panel->addChildLocked(skills_coin_icon);
    
    skills_cash_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    skills_cash_icon->setScaleX(sGlobal->scaleX*0.8f);
    skills_cash_icon->setScaleY(sGlobal->scaleY*0.8f);
    skills_cash_icon->setAnchor(1.0, 0.5f);
    skills_cash_icon->setPosition(skills_coin_icon->getPositionX()+DP(0)*sGlobal->scaleX, skills_coin_icon->getPositionY()-DP(0)*sGlobal->scaleY);
    skill_up_panel->addChildLocked(skills_cash_icon);
    
    skills_gold = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    skills_gold->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    skills_gold->setAlignment(wyBitmapFontLabel::LEFT);
    skills_gold->setPosition(skills_coin_icon->getPositionX() + DP(2)*sGlobal->scaleX, skills_coin_icon->getPositionY());
    skills_gold->setAnchor(0.0f, 0.5f);
    skill_up_panel->addChildLocked(skills_gold);
    
    skills_unlock_notice = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    skills_unlock_notice->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    skills_unlock_notice->setAlignment(wyBitmapFontLabel::LEFT);
    skills_unlock_notice->setAnchor(0.0f, 0.5f);
    skills_unlock_notice->setVisible(false);
    skill_up_panel->addChildLocked(skills_unlock_notice);
    
    skills_unlock_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "skill_0.png");
    skills_unlock_icon->setScaleX(sGlobal->scaleX*0.4f);
    skills_unlock_icon->setScaleY(sGlobal->scaleY*0.4f);
    skills_unlock_icon->setAnchor(0.0f, 0.5f);
    skills_unlock_icon->setVisible(false);
    skill_up_panel->addChildLocked(skills_unlock_icon);
    
    skills_lvmax_container = wyNode::make();
    skills_lvmax_container->setVisible(false);
    skills_lvmax_container->setPosition(middleX, skills_coin_icon->getPositionY());
    skills_detail_panel->addChildLocked(skills_lvmax_container);
    
    skills_lvmax_title = wyBitmapFontLabel::make(sGlobal->font_desc, "level max");
    skills_lvmax_title->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    skills_lvmax_title->setAlignment(wyBitmapFontLabel::LEFT);
    skills_lvmax_title->setPosition(0, 0);
    skills_lvmax_container->addChildLocked(skills_lvmax_title);
    
    skills_lvmax_bg = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "upgrade_title_bg.png");
    skills_lvmax_bg->setScaleX(0.5f*sGlobal->scaleX);
    skills_lvmax_bg->setScaleY(0.5f*sGlobal->scaleY);
    skills_lvmax_bg->setPosition(0, 0);
    skills_lvmax_container->addChildLocked(skills_lvmax_bg, -1);
}

void UpgradeScene::onSkillsBtnClick(wyTargetSelector* ts, int data) {
    wyNode *prevMask = skills_panel->getChildByTag(100+skills_selectedIndex);
    if (prevMask != NULL) {
        prevMask->stopAllActions();
        prevMask->setVisible(false);
    }
    
    wyNode *mask = skills_panel->getChildByTag(100+data);
    mask->setAlpha(255);
    mask->setVisible(true);
    mask->stopAllActions();
    mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.6f, 255, 0), wyFadeTo::make(0.6f, 0, 255), NULL)));

    if (skills_selectedIndex == data) {
        return;
    }
    
    wyNode *nextBtn = skills_panel->getChildByTag(data);
    
    wyIntervalAction *a = wySequence::make(wyMoveTo::make(0.15f, skills_cursor_container->getPositionX(), skills_cursor_container->getPositionY(), nextBtn->getPositionX(), nextBtn->getPositionY()), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(UpgradeScene::onSkillsCursorMoveEnd), nextBtn)), NULL);
    a->setTag(1);
    skills_cursor_container->stopAction(1);
    skills_cursor_container->runAction(a);
    
    skills_selectedIndex = data;
    
    skills_title->setText(SKILLS_NAME[data]);
    skills_desc->setText(SKILLS_DESC[data]);
    
    updateSkillInfo(skills_selectedIndex);
    
    if ((data == 0 || data == 1 || data == 2) && sGlobal->skillArray[data].getValue() > 0 && sGlobal->activeSkill != data) {
        sGlobal->activeSkill = data;
        MyPrefs::setInt(DB_ACTIVE_SKILL, sGlobal->activeSkill);
        
        needEquipSkill = true;

        skill_badge->setVisible(false);
        skill_badge_mask->setVisible(false);
    } else {
        needEquipSkill = false;
    }
    
    if (ts == NULL) {
        skill_badge->setVisible(true);
        skill_badge_mask->setVisible(false);
        skill_badge->setPosition(nextBtn->getPositionX()-DP(36)*sGlobal->scaleX, nextBtn->getPositionY()+DP(36)*sGlobal->scaleY);
        skill_badge_mask->setPosition(skill_badge->getPositionX(), skill_badge->getPositionY());
    }
    
    wyNode *gantan = skills_panel->getChildByTag(1000+data);
    if (gantan->isVisible()) {
        gantan->stopAllActions();
        float fs = 0.6f*sGlobal->scaleX;
        float ts = 1.2f*sGlobal->scaleX;
        gantan->setScale(fs);
        gantan->runAction(wyScaleTo::make(0.5f, fs, fs, ts, ts));
        gantan->runAction(wySequence::make(wyFadeOut::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(UpgradeScene::onGantanDisappeared), 1)), NULL));
        
        sGlobal->skillNoticeArray[data] = 1;
        sGlobal->arrayToStr(DB_SKILL_NOTICE_ARRAY, sGlobal->skillNoticeArray, ARR_LEN_SKILL_NUM);
    }
}

void UpgradeScene::onArmsBtnClick(wyTargetSelector* ts, int data) {
    wyNode *fromPanel = getArmsPanelByWeaponIndex(arms_selectedIndex);
    wyNode *prevMask = fromPanel->getChildByTag(100+arms_selectedIndex);
    if (prevMask != NULL) {
        prevMask->stopAllActions();
        prevMask->setVisible(false);
    }
    
    wyNode *targetPanel = getArmsPanelByWeaponIndex(data);
    wyNode *mask = targetPanel->getChildByTag(100+data);
    mask->setAlpha(255);
    mask->setVisible(true);
    mask->stopAllActions();
    mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.6f, 255, 0), wyFadeTo::make(0.6f, 0, 255), NULL)));
    
    if (arms_selectedIndex == data) {
        return;
    }
    
    wyNode *nextBtn = targetPanel->getChildByTag(data);
    
//    LOGE("pos:%f, %f", arms_cursor_container->getPositionX(), +fromPanel->getPositionX());
    arms_cursor_container->retain();
    arms_cursor_container->getParent()->removeChildLocked(arms_cursor_container, false);
    targetPanel->addChildLocked(arms_cursor_container, 1);
    arms_cursor_container->release();
    arms_cursor_container->setPosition(arms_cursor_container->getPositionX()+fromPanel->getPositionX()-targetPanel->getPositionX(), arms_cursor_container->getPositionY());
    
    wyIntervalAction *a = wySequence::make(wyMoveTo::make(0.15f, arms_cursor_container->getPositionX(), arms_cursor_container->getPositionY(), nextBtn->getPositionX(), nextBtn->getPositionY()), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(UpgradeScene::onArmsCursorMoveEnd), nextBtn)), NULL);
    a->setTag(1);
    arms_cursor_container->stopAction(1);
    arms_cursor_container->runAction(a);
    
    arms_selectedIndex = data;
    
    arms_title->setText(EQUIP_WEAPON_NAME[data]);
    arms_desc->setText(EQUIP_WEAPON_DESC[data]);
    arms_desc_sub->setText(EQUIP_WEAPON_DESC_SUB[data]);
    
    updateWeaponInfo(arms_selectedIndex);
    
    if (sGlobal->weaponUnlockArray[data].getValue() > 0 && sGlobal->activeWeapon != data) {
        sGlobal->activeWeapon = data;
        MyPrefs::setInt(DB_ACTIVE_WEAPON, sGlobal->activeWeapon);
        
        needEquip = true;
    } else {
        needEquip = false;
    }
    
    if (needEquip) {
        equip_badge->retain();
        equip_badge->getParent()->removeChildLocked(equip_badge, false);
        targetPanel->addChildLocked(equip_badge, 2);
        equip_badge->release();
        
        equip_badge_mask->retain();
        equip_badge_mask->getParent()->removeChildLocked(equip_badge_mask, false);
        targetPanel->addChildLocked(equip_badge_mask, 2);
        equip_badge_mask->release();
        

//        else {
            equip_badge->setVisible(false);
            equip_badge_mask->setVisible(false);
            
            equip_badge->setPosition(equip_badge->getPositionX()+fromPanel->getPositionX()-targetPanel->getPositionX(), equip_badge->getPositionY());
            equip_badge_mask->setPosition(equip_badge->getPositionX(), equip_badge->getPositionY());
//        }
    }
    if (ts == NULL) {
        equip_badge->setVisible(true);
        equip_badge_mask->setVisible(false);
        
        equip_badge->setPosition(nextBtn->getPositionX()-DP(38)*sGlobal->scaleX, nextBtn->getPositionY()+DP(38)*sGlobal->scaleX);
        equip_badge_mask->setPosition(equip_badge->getPositionX(), equip_badge->getPositionY());
    }
    
    wyNode *gantan = targetPanel->getChildByTag(1000+data);
    if (gantan->isVisible()) {
        gantan->stopAllActions();
        float fs = 0.6f*sGlobal->scaleX;
        float ts = 1.2f*sGlobal->scaleX;
        gantan->setScale(fs);
        gantan->runAction(wyScaleTo::make(0.5f, fs, fs, ts, ts));
        gantan->runAction(wySequence::make(wyFadeOut::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(UpgradeScene::onGantanDisappeared), 0)), NULL));
        
        sGlobal->weaponNoticeArray[data] = 1;
        sGlobal->arrayToStr(DB_WEAPON_NOTICE_ARRAY, sGlobal->weaponNoticeArray, ARR_LEN_WEAPON_NUM);
    }
}

void UpgradeScene::onGantanDisappeared(wyTargetSelector *ts, int data) {
    if (data == 0) {
        refreshAllArmsGantanhao();
    } else {
        refreshAllSkillsGantanhao();
    }
}

void UpgradeScene::onArmsCursorMoveEnd(wyTargetSelector *ts, void *data) {
    wyNode *targetNode = (wyNode *)data;
    equip_badge->stopAllActions();
    equip_badge_mask->stopAllActions();
    
    if (needEquip) {
        equip_badge->setVisible(true);
        equip_badge_mask->runAction(wySequence::make(wyShow::make(), wyFadeOut::make(0.3f), NULL));
        equip_badge->setPosition(targetNode->getPositionX()-DP(38)*sGlobal->scaleX, targetNode->getPositionY()+DP(38)*sGlobal->scaleY);
        equip_badge_mask->setPosition(equip_badge->getPositionX(), equip_badge->getPositionY());
    }
//    else {
//        equip_badge->setVisible(false);
//        equip_badge_mask->setVisible(false);
//    }
}

void UpgradeScene::onSkillsCursorMoveEnd(wyTargetSelector *ts, void *data) {
    wyNode *targetNode = (wyNode *)data;
    
    skill_badge->stopAllActions();
    skill_badge_mask->stopAllActions();
    
    if (needEquipSkill) {
        skill_badge->setVisible(true);
        skill_badge_mask->runAction(wySequence::make(wyShow::make(), wyFadeOut::make(0.3f), NULL));
        
        skill_badge->setPosition(targetNode->getPositionX()-DP(36)*sGlobal->scaleX, targetNode->getPositionY()+DP(36)*sGlobal->scaleY);
        skill_badge_mask->setPosition(skill_badge->getPositionX(), skill_badge->getPositionY());
    }
//    else {
//        skill_badge->setVisible(false);
//        skill_badge_mask->setVisible(false);
//    }
}


void UpgradeScene::onTabArmsClick(wyTargetSelector* ts) {
    if (currentTabIndex == TAB_ARMS) {
        return;
    }
    
    wyNode *prevMask = skills_panel->getChildByTag(100+skills_selectedIndex);
    if (prevMask != NULL) {
        prevMask->stopAllActions();
        prevMask->setVisible(false);
    }
    skills_panel->setVisible(false);
    
    arms_selectedIndex = -1;
    atk_progress->setPercentage(0);
    speed_progress->setPercentage(0);
    cost_progress->setPercentage(0);
    onArmsBtnClick(NULL, sGlobal->activeWeapon);
    arms_cursor_container->stopAllActions();
    arms_selectedIndex = sGlobal->activeWeapon;
    
    wyNode *targetPanel = getArmsPanelByWeaponIndex(arms_selectedIndex);
    
    arms_cursor_container->retain();
    arms_cursor_container->getParent()->removeChildLocked(arms_cursor_container, false);
    targetPanel->addChildLocked(arms_cursor_container, 1);
    arms_cursor_container->release();
    
    wyNode *skill_0 = targetPanel->getChildByTag(arms_selectedIndex);
    arms_cursor_container->setPosition(skill_0->getPositionX(), skill_0->getPositionY());
    
    arms_panel->setVisible(true);
    
    currentTabIndex = TAB_ARMS;
    
    setPressColor(tab_upgrades_bg);
    tab_upgrades_bg->m_Title->setColor(wyc3b(10, 10, 10));
    tab_upgrades_bg->setScaleX(sGlobal->scaleX*0.7f);
    tab_upgrades_bg->setScaleY(sGlobal->scaleY*0.7f);
    
    tab_arms_bg->setColor(wyc3b(255, 255, 255));
    tab_upgrades_bg->m_Title->setColor(wyc3b(255, 255, 255));
    tab_arms_bg->setScaleX(sGlobal->scaleX*0.8f);
    tab_arms_bg->setScaleY(sGlobal->scaleY*0.8f);
    
    skills_detail_panel->setVisible(false);
    arms_detail_panel->setVisible(true);
    
    freeContainer->setPosition(leftX + DP(2)*sGlobal->scaleX, DP(57)*sGlobal->scaleY);
    freeContainer->setRotation(0);
    
    if (!isTraining) {
        trainGiftWeapon();
    }
}

void UpgradeScene::onTabSkillsClick(wyTargetSelector* ts) {
    if (currentTabIndex == TAB_SKILLS) {
        return;
    }
    
    
    wyNode *prevMask = getArmsPanelByWeaponIndex(arms_selectedIndex)->getChildByTag(100+arms_selectedIndex);
    if (prevMask != NULL) {
        prevMask->stopAllActions();
        prevMask->setVisible(false);
    }
    arms_panel->setVisible(false);
    
    skills_selectedIndex = -1;
    onSkillsBtnClick(NULL, sGlobal->activeSkill);
    skills_cursor_container->stopAllActions();
    skills_selectedIndex = sGlobal->activeSkill;
    wyNode *skill_0 = skills_panel->getChildByTag(skills_selectedIndex);
    skills_cursor_container->setPosition(skill_0->getPositionX(), skill_0->getPositionY());
    
    skills_panel->setVisible(true);
    
    currentTabIndex = TAB_SKILLS;
    
    setPressColor(tab_arms_bg);
    tab_arms_bg->m_Title->setColor(wyc3b(180, 180, 180));
    tab_arms_bg->setScaleX(sGlobal->scaleX*0.7f);
    tab_arms_bg->setScaleY(sGlobal->scaleY*0.7f);
    
    tab_upgrades_bg->setColor(wyc3b(255, 255, 255));
    tab_arms_bg->m_Title->setColor(wyc3b(255, 255, 255));
    tab_upgrades_bg->setScaleX(sGlobal->scaleX*0.8f);
    tab_upgrades_bg->setScaleY(sGlobal->scaleY*0.8f);
    
    skills_detail_panel->setVisible(true);
    arms_detail_panel->setVisible(false);
    
    freeContainer->setPosition(leftX - DP(26)*sGlobal->scaleX, DP(57)*sGlobal->scaleY);
    freeContainer->setRotation(90);
}

void UpgradeScene::onBackBtnClick(wyTargetSelector* ts) {
#if ANDROID
    hideAd();
#endif
    
    if (sGlobal->isRealEndless) {
//        wyDirector* director = wyDirector::getInstance();
//        wyScene *scene = new LoadingScene(SCENE_MENU, 0);
//        scene->autoRelease();
//        director->replaceScene(scene);
        sGlobal->gotoCGScene();
    } else {
//        wyDirector* director = wyDirector::getInstance();
//        wyScene *scene = new LoadingScene(SCENE_WORLD_MAP, 0);
//        scene->autoRelease();
//        director->replaceScene(scene);
        sGlobal->gotoWorldMapScene(true);
    }
}

void UpgradeScene::onStartBtnClick(wyTargetSelector* ts) {
//    LOGE("gotoMapIndex:%d", gotoMapIndex);
    btn_start->setEnabled(false);
    
    wyDirector* director = wyDirector::getInstance();
    wyScene *scene = new LoadingScene(SCENE_MAIN, gotoMapIndex);
    scene->autoRelease();
    director->replaceScene(scene);
}

void UpgradeScene::onShopBtnClick(wyTargetSelector* ts) {
//    sGlobal->resetPrefs();
    ShopLayer *layer = new ShopLayer();
    layer->autoRelease();
    layer->listener = this;
    this->addChildLocked(layer, 101, 0);
    
#if ANDROID
    sGlobal->reportGameEvent(EVT_SHOP_UPGRADE);
#endif
}

void UpgradeScene::onFreeBtnClick(wyTargetSelector* ts) {
#if ANDROID
    showOfferWall(0);
    
    if (currentTabIndex == TAB_ARMS) {
        sGlobal->reportGameEvent(EVT_OFFER_ARMS);
    } else {
        sGlobal->reportGameEvent(EVT_OFFER_SKILLS);
    }
#endif
}

void UpgradeScene::onArmUpgradeBtnClick(wyTargetSelector* ts) {
    if (isTraining) {
        handPointer->setVisible(false);
        showNextTrain();
        hideNotice(NULL);
        
        arms_atk_bg->setTouchEnabled(false);
        arms_atk_bg->setTouchPriority(101);
        arms_atk_bg->setTouchEnabled(true);
        
        sGlobal->isUpgradeTrained = 1;
        MyPrefs::setInt(DB_IS_UPGRADE_TRAINED, 1);
    }
    
    if (sGlobal->weaponUnlockArray[arms_selectedIndex].getValue() == sGlobal->WEAPON_MAX_LEVEL) {
        return;
    }
    
    int index = arms_selectedIndex;
    
    bool needShowShop = false;
    int needShowShopType;
    bool needEquip = false;
    bool needAddAmmo = false;
    if (sGlobal->weaponUnlockArray[index].getValue() == 0 && sGlobal->isWeaponUlkNeedCash(index)) {
        if (sGlobal->zuanshi.getValue() >= sGlobal->weapon_zuanshi[index]) {
            sGlobal->zuanshi.offset(-sGlobal->weapon_zuanshi[index]);
            sGlobal->weaponUnlockArray[index].offset(1);
            sGlobal->arrayToStrEncy(DB_WEAPON_UNLOCK_ARRAY, sGlobal->weaponUnlockArray, ARR_LEN_WEAPON_NUM);
            
            sGlobal->initWeaponInfo(index);
            updateWeaponInfo(index);
            updateCoin();
            updateCash();
            
            needEquip = true;
            
            needAddAmmo = true;
            
            sGlobal->reportGameEvent(EVT_UPGRADE_GUN, sGlobal->weaponUnlockArray[index].getValue(), 0.0f, EQUIP_WEAPON_NAME[index]);
        } else {
            needShowShop = true;
            needShowShopType = 1;
        }
    } else {
        if (sGlobal->getGold() >= sGlobal->weapon_gold[index]) {
            if (sGlobal->weaponUnlockArray[index].getValue() == 0) {
                needEquip = true;
                needAddAmmo = true;
            }
            
            sGlobal->offsetGold(-sGlobal->weapon_gold[index]);
            sGlobal->weaponUnlockArray[index].offset(1);
            sGlobal->arrayToStrEncy(DB_WEAPON_UNLOCK_ARRAY, sGlobal->weaponUnlockArray, ARR_LEN_WEAPON_NUM);
            
            sGlobal->initWeaponInfo(index);
            updateWeaponInfo(index);
            updateCoin();
            updateCash();
            
            sGlobal->reportGameEvent(EVT_UPGRADE_GUN, sGlobal->weaponUnlockArray[index].getValue(), 0.0f, EQUIP_WEAPON_NAME[index]);
        } else {
            needShowShop = true;
            needShowShopType = 0;
        }
    }
    
    wyNode *targetPanel = getArmsPanelByWeaponIndex(arms_selectedIndex);
    wyNode *skill_0 = targetPanel->getChildByTag(arms_selectedIndex);
    if (needEquip) {
        sGlobal->activeWeapon = arms_selectedIndex;
        MyPrefs::setInt(DB_ACTIVE_WEAPON, sGlobal->activeWeapon);
        
        equip_badge->retain();
        equip_badge->getParent()->removeChildLocked(equip_badge, false);
        targetPanel->addChildLocked(equip_badge, 2);
        equip_badge->release();
        equip_badge->setPosition(skill_0->getPositionX()-DP(38)*sGlobal->scaleX, skill_0->getPositionY()+DP(38)*sGlobal->scaleY);
        
        equip_badge_mask->retain();
        equip_badge_mask->getParent()->removeChildLocked(equip_badge_mask, false);
        targetPanel->addChildLocked(equip_badge_mask, 2);
        equip_badge_mask->release();
        equip_badge_mask->setPosition(equip_badge->getPositionX(), equip_badge->getPositionY());
        
        equip_badge->setVisible(true);
        equip_badge_mask->setVisible(true);
        equip_badge_mask->runAction(wyFadeOut::make(0.3f));
        
        wyNode *starContainer = targetPanel->getChildByTag(3000);
        for (int i = 0; i < 5; i++) {
            wyNode *star = starContainer->getChildByTag(index*1000+i);
            star->setVisible(true);
//            star->setScale(0);
//            star->runAction(wyScaleTo::make(0.2f, 0, 0, 0.4f*sGlobal->scaleX, 0.4f*sGlobal->scaleX));
        }
    }
    
    if (needShowShop) {
        showGoldNotEnoughDialog(needShowShopType);
    } else {
        sGlobal->playEffect(se_upgrade);
        
        wyParticleSystem *train_skill_emitter = wyParticleLoader::load("particle_train_skill.plist");
        targetPanel->addChildLocked(train_skill_emitter, 999);
        
        float scale = sGlobal->scaleX;
        float width = skill_0->getWidth()*skill_0->getScaleX()*0.8f;
        float height = skill_0->getHeight()*skill_0->getScaleY();
        train_skill_emitter->setStartSizeVariance(12, 5);
        train_skill_emitter->setParticlePositionVariance(width/2/scale, 0, width/2/scale, 0);
        train_skill_emitter->setEmissionRate(60);
        train_skill_emitter->setDuration(1.0f);

        train_skill_emitter->setLifeVariance(0.6, 0.2);
        train_skill_emitter->setScale(scale);
        train_skill_emitter->setPosition(skill_0->getPositionX()-width/2+DP(0)*sGlobal->scaleY, skill_0->getPositionY()-height/2+DP(10)*sGlobal->scaleY);
        train_skill_emitter->setAutoRemoveOnFinish(true);
        train_skill_emitter->setPositionType(GROUPED);
        
        wyNode *mask = targetPanel->getChildByTag(100+arms_selectedIndex);
        mask->setAlpha(0);
        mask->setVisible(true);
        mask->stopAllActions();
        mask->runAction(wySequence::make(wyFadeIn::make(0.5f), wyFadeOut::make(0.5f), wyDelayTime::make(0.2f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(UpgradeScene::recoverMaskBlink), mask)), NULL));
        
        wyNode *starMask;
//        if (!needEquip) {
            wyNode *btn_skill = targetPanel->getChildByTagLocked(index);
            wyNode *starContainer = targetPanel->getChildByTag(3000);
            
            int prevLevel = sGlobal->weaponUnlockArray[index].getValue()-1;
            int idxx = prevLevel/2;
        
            if (idxx*2 == prevLevel) {
                wySpriteEx *star = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star_left.png");
                star->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*(idxx+0), btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
                starContainer->addChildLocked(star, 10);
                star->setScaleX(sGlobal->scaleX*0.4f);
                star->setScaleY(sGlobal->scaleY*0.4f);
                
                starMask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star_left.png");
                starMask->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*(idxx+0), btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
            } else {
                wySpriteEx *star = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star_right.png");
                star->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*(idxx+0), btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
                starContainer->addChildLocked(star, 10);
                star->setScaleX(sGlobal->scaleX*0.4f);
                star->setScaleY(sGlobal->scaleY*0.4f);
                
                starMask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "star_right.png");
                starMask->setPosition(btn_skill->getPositionX()-DP(30)*sGlobal->scaleX+DP(16)*sGlobal->scaleX*(idxx+0), btn_skill->getPositionY()-DP(50)*sGlobal->scaleY);
            }
            
            targetPanel->addChildLocked(starMask, 10);
            starMask->setScaleX(sGlobal->scaleX*0.4f);
            starMask->setScaleY(sGlobal->scaleY*0.4f);
            starMask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
            starMask->runAction(wySequence::make(wyFadeOut::make(1.0f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(UpgradeScene::removeStarMask), starMask)), NULL));
//        }
        
    }
    
    if (needAddAmmo && index != WEAPON_UMP && index != WEAPON_HYDRA) {
        sGlobal->weaponAmmoArray[index] += sGlobal->WEAPON_PARAM[index*8+WP_AMMO_AMOUNT];
        sGlobal->arrayToStr(DB_WEAPON_AMMO_ARRAY, sGlobal->weaponAmmoArray, ARR_LEN_WEAPON_NUM);
        char temp[20];
        sprintf(temp, "%d > %d", sGlobal->weaponAmmoArray[index], sGlobal->weaponAmmoArray[index]+(int)sGlobal->WEAPON_PARAM[index*8+WP_AMMO_AMOUNT]);
        arms_ammo_prop->setText(temp);
    }
}

void UpgradeScene::recoverMaskBlink(wyTargetSelector* ts, void* data) {
    wyNode *node = (wyNode *)data;
    node->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.6f, 255, 0), wyFadeTo::make(0.6f, 0, 255), NULL)));
}

void UpgradeScene::removeStarMask(wyTargetSelector* ts, void* data) {
    wyNode *node = (wyNode *)data;
    node->getParent()->removeChildLocked(node, true);
}

void UpgradeScene::onArmAmmoBtnClick(wyTargetSelector* ts) {
    int index = arms_selectedIndex;
    
    bool needShowShop = false;
        if (sGlobal->getGold() >= sGlobal->weapon_ammo_gold[index]) {
            sGlobal->offsetGold(-sGlobal->weapon_ammo_gold[index]);
            sGlobal->weaponAmmoArray[index] += sGlobal->WEAPON_PARAM[index*8+WP_AMMO_AMOUNT];
            sGlobal->arrayToStr(DB_WEAPON_AMMO_ARRAY, sGlobal->weaponAmmoArray, ARR_LEN_WEAPON_NUM);
            
            char temp[20];
            sprintf(temp, "%d > %d", sGlobal->weaponAmmoArray[index], sGlobal->weaponAmmoArray[index]+(int)sGlobal->WEAPON_PARAM[index*8+WP_AMMO_AMOUNT]);
            arms_ammo_prop->setText(temp);
            updateCoin();
            updateCash();
        } else {
            needShowShop = true;
        }
    
    if (needShowShop) {
        showGoldNotEnoughDialog(0);
    } else {
        //添加购买子弹特效
    }
}

void UpgradeScene::onSkillUpgradeBtnClick(wyTargetSelector* ts) {
    if (isTraining) {
//        handPointer->setVisible(false);
//        showNextTrain();
        touchLayer->getParent()->removeChildLocked(touchLayer, true);
        handPointer->getParent()->removeChildLocked(handPointer, true);
        hideNotice(NULL);
        
        skills_upgrades_bg->setTouchEnabled(false);
        skills_upgrades_bg->setTouchPriority(101);
        skills_upgrades_bg->setTouchEnabled(true);
        
        sGlobal->isToolTrained = 1;
        MyPrefs::setInt(DB_IS_TOOL_TRAINED, 1);
        
        isTraining = false;
    }
    
    if (sGlobal->skillArray[skills_selectedIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
        return;
    }
    
    int index = skills_selectedIndex;
    
    bool needShowShop = false;
    bool needEquip = false;
    bool needUpdateImg = false;
    if (index > SKILL_SHIELD) {
        if (sGlobal->zuanshi.getValue() >= sGlobal->skill_cash[index]) {
            if (sGlobal->skillArray[index].getValue() == 0) {
                needUpdateImg = true;
            }
            
            sGlobal->zuanshi.offset(-sGlobal->skill_cash[index]);
            sGlobal->skillArray[index].offset(1);
            sGlobal->arrayToStrEncy(DB_SKILL_ARRAY, sGlobal->skillArray, ARR_LEN_SKILL_NUM);
            
            sGlobal->initSkillEffect(index);
            updateSkillInfo(index);
            updateCoin();
            updateCash();
            
            sGlobal->reportGameEvent(EVT_UPGRADE_SKILL, sGlobal->skillArray[index].getValue(), 0.0f, SKILLS_NAME[index]);
        } else {
            needShowShop = true;
        }
    } else {
        if (sGlobal->getGold() >= sGlobal->skill_gold[index]) {
            if (sGlobal->skillArray[index].getValue() == 0) {
                needEquip = true;
                needUpdateImg = true;
            }
            
            sGlobal->offsetGold(-sGlobal->skill_gold[index]);
            sGlobal->skillArray[index].offset(1);
            sGlobal->arrayToStrEncy(DB_SKILL_ARRAY, sGlobal->skillArray, ARR_LEN_SKILL_NUM);
            
            sGlobal->initSkillEffect(index);
            updateSkillInfo(index);
            updateCoin();
            updateCash();
            
            sGlobal->reportGameEvent(EVT_UPGRADE_SKILL, sGlobal->skillArray[index].getValue(), 0.0f, SKILLS_NAME[index]);
        } else {
            needShowShop = true;
        }
    }
    wyNode *skill_0 = skills_panel->getChildByTag(skills_selectedIndex);
    
    if (needEquip) {
        sGlobal->activeSkill = skills_selectedIndex;
        MyPrefs::setInt(DB_ACTIVE_SKILL, sGlobal->activeSkill);
        
        skill_badge->setVisible(true);
        skill_badge_mask->setVisible(true);
        skill_badge_mask->runAction(wyFadeOut::make(0.3f));
        
        skill_badge->setPosition(skill_0->getPositionX()-DP(36)*sGlobal->scaleX, skill_0->getPositionY()+DP(36)*sGlobal->scaleY);
        skill_badge_mask->setPosition(skill_badge->getPositionX(), skill_badge->getPositionY());
    }
    
    if (needUpdateImg) {
        char temp[40];
        sprintf(temp, "skill_%d.png", skills_selectedIndex);
        
        wySpriteEx *skill_icon = (wySpriteEx*)skills_panel->getChildByTag(skills_selectedIndex+300);
        wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", temp);
        wyRect r = frame->getRect();
        skill_icon->setTextureRect(r);
        skill_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
    }
    
    if (needShowShop) {
        showGoldNotEnoughDialog(1);
    } else {
        sGlobal->playEffect(se_upgrade);

        wyParticleSystem *train_skill_emitter = wyParticleLoader::load("particle_train_skill.plist");
        skills_panel->addChildLocked(train_skill_emitter, 999);
        
        float scale = sGlobal->scaleX;
        float width = skill_0->getWidth()*skill_0->getScaleX()*0.8f;
        float height = skill_0->getHeight()*skill_0->getScaleY();
        train_skill_emitter->setStartSizeVariance(12, 5);
        train_skill_emitter->setParticlePositionVariance(width/2/scale, 0, width/2/scale, 0);
        train_skill_emitter->setEmissionRate(60);
        train_skill_emitter->setDuration(1.0f);
        
        train_skill_emitter->setLifeVariance(0.6, 0.2);
        train_skill_emitter->setScale(scale);
        train_skill_emitter->setPosition(skill_0->getPositionX()-width/2+DP(0)*sGlobal->scaleY, skill_0->getPositionY()-height/2+DP(8)*sGlobal->scaleY);
        train_skill_emitter->setAutoRemoveOnFinish(true);
        train_skill_emitter->setPositionType(GROUPED);
        
        wyNode *mask = skills_panel->getChildByTag(100+skills_selectedIndex);
        mask->setAlpha(0);
        mask->setVisible(true);
        mask->stopAllActions();
        mask->runAction(wySequence::make(wyFadeIn::make(0.5f), wyFadeOut::make(0.5f), wyDelayTime::make(0.2f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(UpgradeScene::recoverMaskBlink), mask)), NULL));
        
        int unlockIdx = -1;
        switch (index) {
            case SKILL_MISSLE:
                if (sGlobal->skillArray[index].getValue() == SKILL_ROBOT_PRV) {
                    lineLayer->updateLineColor(SKILL_ROBOT);
                    unlockIdx = SKILL_ROBOT;
                }
                break;
            case SKILL_ROBOT:
                if (sGlobal->skillArray[index].getValue() == SKILL_SHIELD_PRV) {
                    lineLayer->updateLineColor(SKILL_SHIELD);
                    unlockIdx = SKILL_SHIELD;
                }
                break;
            case SKILL_GREED:
                if (sGlobal->skillArray[index].getValue() == SKILL_ARMOR_PRV) {
                    lineLayer->updateLineColor(SKILL_ARMOR);
                    unlockIdx = SKILL_ARMOR;
                }
                if (sGlobal->skillArray[index].getValue() == SKILL_STAMINA_PRV) {
                    lineLayer->updateLineColor(SKILL_STAMINA);
                    unlockIdx = SKILL_STAMINA;
                }
                break;
            case SKILL_STAMINA:
                if (sGlobal->skillArray[index].getValue() == SKILL_EVASION_PRV) {
                    lineLayer->updateLineColor(SKILL_EVASION);
                    unlockIdx = SKILL_EVASION;
                }
                break;
            case SKILL_FRENZY:
                if (sGlobal->skillArray[index].getValue() == SKILL_CRITICAL_PRV) {
                    lineLayer->updateLineColor(SKILL_CRITICAL);
                    unlockIdx = SKILL_CRITICAL;
                }
                break;
            case SKILL_CRITICAL:
                if (sGlobal->skillArray[index].getValue() == SKILL_STUN_PRV) {
                    lineLayer->updateLineColor(SKILL_STUN);
                    unlockIdx = SKILL_STUN;
                }
                break;
            default:
                break;
        }
        if (unlockIdx != -1) {
            char temp[40];
            sprintf(temp, "skill_%d.png", unlockIdx);
            wySpriteEx *skill_icon = (wySpriteEx*)skills_panel->getChildByTag(unlockIdx+300);
            wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", temp);
            wyRect r = frame->getRect();
            skill_icon->setTextureRect(r);
            skill_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
            
            wySpriteEx *mask1 = (wySpriteEx*)skills_panel->getChildByTag(unlockIdx+100);
            mask1->setAlpha(0);
            mask1->setVisible(true);
            mask1->runAction(wySequence::make(wyFadeIn::make(0.5f), wyFadeOut::make(0.5f), NULL));
        }
    }
}

void UpgradeScene::updateWeaponInfo(int data) {
    char temp[20];
    
    wyNode *targetPanel = getArmsPanelByWeaponIndex(data);
    if (sGlobal->weaponUnlockArray[data].getValue() > 0) {
        bool isMAX = sGlobal->weaponUnlockArray[data].getValue() == sGlobal->WEAPON_MAX_LEVEL;
        if (isMAX) {
            sprintf(temp, "%d", (int)round(sGlobal->weapon_atk[data]));
            arms_atk_prop->setText(temp);
        } else {
            arms_atk_bg->m_Title->setText("upgrade");
            
            sprintf(temp, "%d > %d", (int)round(sGlobal->weapon_atk[data]), (int)round(sGlobal->weapon_atk[data]+sGlobal->WEAPON_PARAM[data*8+WP_ATK_INC]));
            arms_atk_prop->setText(temp);
            sprintf(temp, "%d", sGlobal->weapon_gold[data]);
            arms_atk_gold->setText(temp);
        }
        
        arms_atk_bg->setVisible(!isMAX);
        coin_icon1->setVisible(!isMAX);
        cash_icon->setVisible(!isMAX);
        arms_atk_gold->setVisible(!isMAX);
        arms_lvmax_container->setVisible(isMAX);
        arms_lvmax_title->setText("level max");
        arms_atk_gold->setAnchor(0, 0.5f);
        arms_atk_gold->setPosition(coin_icon1->getPositionX() + DP(2)*sGlobal->scaleX, coin_icon1->getPositionY());
        
        if (!isMAX) {
            cash_icon->setVisible(false);
            coin_icon1->setVisible(true);
        }
        
        arms_ammo_container->setVisible(true);
        
        if (data == WEAPON_UMP || data == WEAPON_HYDRA) {
            sprintf(temp, "unlimited");
            arms_ammo_prop->setText(temp);
        } else {
            sprintf(temp, "%d > %d", sGlobal->weaponAmmoArray[data], sGlobal->weaponAmmoArray[data]+(int)sGlobal->WEAPON_PARAM[data*8+WP_AMMO_AMOUNT]);
            arms_ammo_prop->setText(temp);
            sprintf(temp, "%d", sGlobal->weapon_ammo_gold[data]);
            arms_ammo_gold->setText(temp);
        }
        
        bool flag = !(data == WEAPON_UMP || data == WEAPON_HYDRA);
        coin_icon2->setVisible(flag);
        arms_ammo_gold->setVisible(flag);
        arms_ammo_bg->setVisible(flag);
        
        wyBitmapFontLabel *lv = (wyBitmapFontLabel*) targetPanel->getChildByTag(200+data);
        if (lv != NULL) {
            lv->setVisible(true);
            sprintf(temp, "%d/%d", sGlobal->weaponUnlockArray[data].getValue(), sGlobal->WEAPON_MAX_LEVEL);
            lv->setText(temp);
        }
    } else {
        if (data == WEAPON_GLB) {
            arms_ammo_container->setVisible(false);
            arms_atk_bg->setVisible(false);
            arms_lvmax_container->setVisible(true);
            arms_atk_gold->setVisible(true);
            
            sprintf(temp, "%d", (int)round(sGlobal->weapon_atk[data]));
            arms_atk_prop->setText(temp);
            
            cash_icon->setVisible(false);
            coin_icon1->setVisible(false);
            
            arms_atk_gold->setAnchor(0.5f, 0.5f);
            arms_atk_gold->setText("purchase anything to unlock");
            arms_atk_gold->setPosition(arms_lvmax_container->getPositionX(), arms_lvmax_container->getPositionY() - DP(30)*sGlobal->scaleX);
            
            arms_lvmax_title->setText("gift weapon");
        } else {
            arms_ammo_container->setVisible(false);
            arms_atk_bg->setVisible(true);
            arms_atk_bg->m_Title->setText("unlock");
            arms_lvmax_container->setVisible(false);
            arms_atk_gold->setVisible(true);
            arms_atk_gold->setAnchor(0, 0.5f);
            arms_atk_gold->setPosition(coin_icon1->getPositionX() + DP(2)*sGlobal->scaleX, coin_icon1->getPositionY());
            
            sprintf(temp, "%d", (int)round(sGlobal->weapon_atk[data]));
            arms_atk_prop->setText(temp);
            
            int index = data;
            if (sGlobal->isWeaponUlkNeedCash(index)) {
                sprintf(temp, "%d", sGlobal->weapon_zuanshi[data]);
                cash_icon->setVisible(true);
                coin_icon1->setVisible(false);
            } else {
                sprintf(temp, "%d", sGlobal->weapon_gold[data]);
                cash_icon->setVisible(false);
                coin_icon1->setVisible(true);
            }
            arms_atk_gold->setText(temp);
        }
    }
    
    atk_progress->runAction(wyProgressTo::make(0.2f, atk_progress->getPercentage(), sGlobal->weapon_atk[data]/80.0f*100));
    speed_progress->runAction(wyProgressTo::make(0.2f, speed_progress->getPercentage(), sGlobal->WEAPON_PARAM[data*8+WP_SPEED]/15.0f*100));
    cost_progress->runAction(wyProgressTo::make(0.2f, cost_progress->getPercentage(), sGlobal->WEAPON_PARAM[data*8+WP_AMMO_COST]/60.0f*100));
    
    if (arms_atk_bg->isVisible()) {
        arms_atk_bg->m_Mask->setVisible(true);
        arms_atk_bg->m_Mask->stopAllActions();
        arms_atk_bg->m_Mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.8, 255, 0), wyFadeTo::make(0.8, 0, 255), NULL)));
    }
}

void UpgradeScene::updateSkillInfo(int data) {
    char temp[20];
    
    if (sGlobal->skillArray[data].getValue() > 0) {
        skills_unlock_notice->setVisible(false);
        skills_unlock_icon->setVisible(false);
        
        bool isMAX = sGlobal->skillArray[data].getValue() == sGlobal->SKILL_MAX_LEVEL;
        if (isMAX) {
            skills_coin_icon->setVisible(false);
            skills_cash_icon->setVisible(false);
        } else {
            skills_upgrades_bg->m_Title->setText("upgrade");
            
            if (data > SKILL_SHIELD) {
                sprintf(temp, "%d", sGlobal->skill_cash[data]);
                skills_cash_icon->setVisible(true);
                skills_coin_icon->setVisible(false);
            } else {
                sprintf(temp, "%d", sGlobal->skill_gold[data]);
                skills_cash_icon->setVisible(false);
                skills_coin_icon->setVisible(true);
            }
            skills_gold->setText(temp);
        }
        skills_upgrades_bg->setVisible(!isMAX);
        skills_lvmax_container->setVisible(isMAX);
        skills_gold->setVisible(!isMAX);
        
        wyBitmapFontLabel *lv = (wyBitmapFontLabel*) skills_panel->getChildByTag(200+data);
        if (lv != NULL) {
            lv->setVisible(true);
            sprintf(temp, "%d/%d", sGlobal->skillArray[data].getValue(), sGlobal->SKILL_MAX_LEVEL);
            lv->setText(temp);
        }
    } else {
        skills_upgrades_bg->setVisible(true);
        skills_gold->setVisible(true);
        skills_lvmax_container->setVisible(false);

        bool canUnlock = true;
        float ox = 0, ox1 = 0;
        float oy = 0;
        switch (data) {
            case SKILL_ROBOT: {
                if (sGlobal->skillArray[SKILL_MISSLE].getValue() >= SKILL_ROBOT_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "missle lv.%d", SKILL_ROBOT_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_0.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    ox = -6;
                    ox1 = 2;
                }
                break;
            }
            case SKILL_SHIELD: {
                if (sGlobal->skillArray[SKILL_ROBOT].getValue() >= SKILL_SHIELD_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "robot lv.%d", SKILL_SHIELD_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_1.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    ox = 0;
                    oy = 2;
                    ox1 = -4;
                }
                break;
            }
            case SKILL_ARMOR: {
                if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_ARMOR_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "greed lv.%d", SKILL_ARMOR_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_3.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    ox = 2;
                    oy = 0;
                    ox1 = -6;
                }
                break;
            }
            case SKILL_STAMINA: {
                if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_STAMINA_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "greed lv.%d", SKILL_STAMINA_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_3.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    ox = 2;
                    oy = 0;
                    ox1 = -6;
                }
                break;
            }
            case SKILL_EVASION: {
                if (sGlobal->skillArray[SKILL_STAMINA].getValue() >= SKILL_EVASION_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "stamina lv.%d", SKILL_EVASION_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_5.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    ox = 2;
                    oy = -6;
                    ox1 = -4;
                }
                break;
            }
            case SKILL_CRITICAL: {
                if (sGlobal->skillArray[SKILL_FRENZY].getValue() >= SKILL_CRITICAL_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "frenzy lv.%d", SKILL_CRITICAL_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_7.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    
                    oy = -8;
                    ox1 = -4;
                }
                break;
            }
            case SKILL_STUN: {
                if (sGlobal->skillArray[SKILL_CRITICAL].getValue() >= SKILL_STUN_PRV) {
                    canUnlock = true;
                } else {
                    canUnlock = false;
                    sprintf(temp, "critical lv.%d", SKILL_STUN_PRV);
                    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("dialog_deco", "skill_8.png");
                    wyRect r = frame->getRect();
                    skills_unlock_icon->setTextureRect(r);
                    skills_unlock_icon->setContentSize(frame->getOriginalSize().width, frame->getOriginalSize().height);
                    oy = -6;
                    ox1= -2;
                }
                break;
            }
            default:
                break;
        }
        if (canUnlock) {
            skills_upgrades_bg->setVisible(true);
            skills_upgrades_bg->m_Title->setText("unlock");
            
            if (data > SKILL_SHIELD) {
                sprintf(temp, "%d", sGlobal->skill_cash[data]);
                skills_cash_icon->setVisible(true);
                skills_coin_icon->setVisible(false);
            } else {
                sprintf(temp, "%d", sGlobal->skill_gold[data]);
                skills_cash_icon->setVisible(false);
                skills_coin_icon->setVisible(true);
            }
            skills_gold->setText(temp);
            
            skills_unlock_notice->setVisible(false);
            skills_unlock_icon->setVisible(false);
        } else {
            skills_gold->setText("need");
            skills_unlock_notice->setText(temp);
            skills_unlock_icon->setPosition(skills_gold->getPositionX()+skills_gold->getWidth()*skills_gold->getScale()+DP(6+ox)*sGlobal->scaleX, skills_gold->getPositionY()+DP(oy)*sGlobal->scaleX);
            skills_unlock_notice->setPosition(skills_unlock_icon->getPositionX()+skills_unlock_icon->getWidth()*skills_unlock_icon->getScaleX()+DP(ox1)*sGlobal->scaleX, skills_gold->getPositionY());
            
            skills_unlock_notice->setVisible(true);
            skills_unlock_icon->setVisible(true);
            
            skills_coin_icon->setVisible(false);
            skills_cash_icon->setVisible(false);
            
            skills_upgrades_bg->setVisible(false);
        }
    }
    
    if (skills_upgrades_bg->isVisible()) {
        skills_upgrades_bg->m_Mask->setVisible(true);
        skills_upgrades_bg->m_Mask->stopAllActions();
        skills_upgrades_bg->m_Mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.8, 255, 0), wyFadeTo::make(0.8, 0, 255), NULL)));
    }
    
    generateSkillProp(data);
}

void UpgradeScene::generateSkillProp(int skillIndex) {
    skill_prop_panel->removeAllChildren(true);
    
    int fontSize = 12;
    
    bool needMove = false;
    float curData, nextData;
    char temp[50];
    switch (skillIndex) {
        case SKILL_MISSLE: {
            wySpriteEx *atk_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "attack_icon.png");
            atk_icon->setScaleX(sGlobal->scaleX*0.5f);
            atk_icon->setScaleY(sGlobal->scaleY*0.5f);
            atk_icon->setAnchor(0, 0.5f);
            atk_icon->setPosition(0, 0);
            skill_prop_panel->addChildLocked(atk_icon);
            
            curData = sGlobal->skillMulti[SKILL_MISSLE];
            nextData = sGlobal->skillMulti[SKILL_MISSLE];
            if (sGlobal->skillArray[skillIndex].getValue() <= 10) {
                nextData += sGlobal->ROCKET_ATK_INC_0;
            }
            else if (sGlobal->skillArray[skillIndex].getValue() > 10 && sGlobal->skillArray[skillIndex].getValue() <= 20) {
                nextData += sGlobal->ROCKET_ATK_INC_1;
            }
            else if (sGlobal->skillArray[skillIndex].getValue() > 20 && sGlobal->skillArray[skillIndex].getValue() <= 30) {
                nextData += sGlobal->ROCKET_ATK_INC_2;
            }
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                sprintf(temp, "%d", (int)nextData);
            } else {
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "%d", (int)curData);
                } else {
                    sprintf(temp, "%d > %d", (int)curData, (int)nextData);
                }
            }

            wyBitmapFontLabel *atk_title = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            atk_title->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            atk_title->setAlignment(wyBitmapFontLabel::LEFT);
            atk_title->setPosition(atk_icon->getPositionX()+DP(34)*sGlobal->scaleX, atk_icon->getPositionY() - DP(0)*sGlobal->scaleY);
            atk_title->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(atk_title);
            break;
        }
        case SKILL_ROBOT: {
            wySpriteEx *atk_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "attack_icon.png");
            atk_icon->setScaleX(sGlobal->scaleX*0.5f);
            atk_icon->setScaleY(sGlobal->scaleY*0.5f);
            atk_icon->setAnchor(0, 0.5f);
            atk_icon->setPosition(0, 0);
            skill_prop_panel->addChildLocked(atk_icon);
            
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                nextData = sGlobal->SKILLS_PARAMS[SKILL_ROBOT] + sGlobal->SKILL_ROBOT_BASE_ATK;
                sprintf(temp, "%d", (int)nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_ROBOT];
                nextData = sGlobal->skillMulti[SKILL_ROBOT] + sGlobal->SKILLS_PARAMS[SKILL_ROBOT];
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "%d", (int)curData);
                } else {
                    sprintf(temp, "%d > %d", (int)curData, (int)nextData);
                }
            }

            wyBitmapFontLabel *atk_title = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            atk_title->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            atk_title->setAlignment(wyBitmapFontLabel::LEFT);
            atk_title->setPosition(atk_icon->getPositionX()+DP(34)*sGlobal->scaleX, atk_icon->getPositionY() - DP(0)*sGlobal->scaleY);
            atk_title->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(atk_title);
            break;
        }
        case SKILL_SHIELD: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                nextData = sGlobal->SKILLS_PARAMS[SKILL_SHIELD] + sGlobal->SKILL_SHIELD_LAST_BASE;
                sprintf(temp, "duration: %.1f s", nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_SHIELD];
                nextData = sGlobal->skillMulti[SKILL_SHIELD] + sGlobal->SKILLS_PARAMS[SKILL_SHIELD];
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "duration: %.1f s", curData);
                } else {
                    sprintf(temp, "duration: %.1f > %.1f s", curData, nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, DP(0)*sGlobal->scaleY);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            
            int idx = SKILL_STUN + 1;
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                nextData = sGlobal->SKILLS_PARAMS[idx] + sGlobal->SKILL_SHIELD_CD_BASE;
                sprintf(temp, "cooldown: %.1f s", nextData);
            } else {
                curData = sGlobal->skillMulti[idx];
                nextData = sGlobal->skillMulti[idx] - sGlobal->SKILLS_PARAMS[idx];
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "cooldown: %.1f s", curData);
                } else {
                    sprintf(temp, "cooldown: %.1f > %.1f s", curData, nextData);
                }
            }

            wyBitmapFontLabel *title2 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title2->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title2->setAlignment(wyBitmapFontLabel::LEFT);
            title2->setPosition(0, -DP(16)*sGlobal->scaleY);
            title2->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title2);
            
            needMove = true;
            break;
        }
        case SKILL_GREED: {
            curData = sGlobal->skillMulti[SKILL_GREED]*100;
            nextData = (sGlobal->skillMulti[SKILL_GREED] + sGlobal->SKILLS_PARAMS[SKILL_GREED])*100;
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                sprintf(temp, "extra gold: %.1f%%", nextData);
            } else {
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "extra gold: %.1f%%", curData);
                } else {
                    sprintf(temp, "extra gold: %.1f%%   >   %.1f%%", curData, nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
        case SKILL_ARMOR: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                curData = sGlobal->HERO_BASE_DEF;
                nextData = sGlobal->SKILLS_PARAMS[SKILL_ARMOR] + sGlobal->HERO_BASE_DEF;
                sprintf(temp, "def: %d", (int)nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_ARMOR] + sGlobal->HERO_BASE_DEF;
                nextData = (sGlobal->skillMulti[SKILL_ARMOR] + sGlobal->SKILLS_PARAMS[SKILL_ARMOR]) + sGlobal->HERO_BASE_DEF;
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "def: %d", (int)curData);
                } else {
                    sprintf(temp, "def: %d > %d", (int)curData, (int)nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
        case SKILL_STAMINA: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                curData = sGlobal->HERO_BASE_HP;
                nextData = sGlobal->SKILLS_PARAMS[SKILL_STAMINA] + sGlobal->HERO_BASE_HP;
                sprintf(temp, "life: %d", (int)nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_STAMINA] + sGlobal->HERO_BASE_HP;
                nextData = (sGlobal->skillMulti[SKILL_STAMINA] + sGlobal->SKILLS_PARAMS[SKILL_STAMINA]) + sGlobal->HERO_BASE_HP;
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "life: %d", (int)curData);
                } else {
                    sprintf(temp, "life: %d > %d", (int)curData, (int)nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
        case SKILL_EVASION: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                curData = 0;
                nextData = sGlobal->SKILLS_PARAMS[SKILL_EVASION]*100;
                sprintf(temp, "chance of doge: %.1f%%", nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_EVASION]*100;
                nextData = (sGlobal->skillMulti[SKILL_EVASION] + sGlobal->SKILLS_PARAMS[SKILL_EVASION])*100;
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "chance of doge: %.1f%%", curData);
                } else {
                    sprintf(temp, "chance of doge: %.1f%% > %.1f%%", curData, nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
        case SKILL_FRENZY: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                curData = 0;
                nextData = sGlobal->SKILLS_PARAMS[SKILL_FRENZY]*100;
                sprintf(temp, "speed increase: %.1f%%", nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_FRENZY]*100;
                nextData = (sGlobal->skillMulti[SKILL_FRENZY] + sGlobal->SKILLS_PARAMS[SKILL_FRENZY])*100;
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "speed increase: %.1f%%", curData);
                } else {
                    sprintf(temp, "speed increase: %.1f%% > %.1f%%", curData, nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
        case SKILL_CRITICAL: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                curData = 0;
                nextData = sGlobal->SKILLS_PARAMS[SKILL_CRITICAL]*100;
                sprintf(temp, "chance of critical: %.1f%%", nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_CRITICAL]*100;
                nextData = (sGlobal->skillMulti[SKILL_CRITICAL] + sGlobal->SKILLS_PARAMS[SKILL_CRITICAL])*100;
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "chance of critical: %.1f%%", curData);
                } else {
                    sprintf(temp, "chance of critical: %.1f%% > %.1f%%", curData, nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
        case SKILL_STUN: {
            if (sGlobal->skillArray[skillIndex].getValue() == 0) {
                curData = 0;
                nextData = sGlobal->SKILLS_PARAMS[SKILL_STUN]*100;
                sprintf(temp, "chance of stun: %.1f%%", nextData);
            } else {
                curData = sGlobal->skillMulti[SKILL_STUN]*100;
                nextData = (sGlobal->skillMulti[SKILL_STUN] + sGlobal->SKILLS_PARAMS[SKILL_STUN])*100;
                if (sGlobal->skillArray[skillIndex].getValue() == sGlobal->SKILL_MAX_LEVEL) {
                    sprintf(temp, "chance of stun: %.1f%%", curData);
                } else {
                    sprintf(temp, "chance of stun: %.1f%% > %.1f%%", curData, nextData);
                }
            }

            wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
            title1->setScale(SP(fontSize)*sGlobal->scaleX/sGlobal->descFontSize);
            title1->setAlignment(wyBitmapFontLabel::LEFT);
            title1->setPosition(0, 0);
            title1->setAnchor(0, 0.5f);
            skill_prop_panel->addChildLocked(title1);
            break;
        }
            
        default:
            break;
    }
    
    if (needMove) {
        skill_up_panel->setPosition(0, -DP(20)*sGlobal->scaleY);
    } else {
        skill_up_panel->setPosition(0, 0);
    }
}

void UpgradeScene::updateCoin() {
    char temp[30];
    addComma(sGlobal->getGold(), temp);
    coin_num->setText(temp);
    
    MyPrefs::setInt(DB_GOLD, sGlobal->getGold());
    
    refreshAllArmsGantanhao();
    refreshAllSkillsGantanhao();
}

void UpgradeScene::updateCash() {
    char temp[30];
	addComma(sGlobal->zuanshi.getValue(), temp);
    cash_num->setText(temp);
    
    MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
    
    refreshAllArmsGantanhao();
    refreshAllSkillsGantanhao();
}

void UpgradeScene::showGoldNotEnoughDialog(int type) {
    ConfirmDialogLayer *layer = new ConfirmDialogLayer(0, 0, type);
    layer->autoRelease();
    layer->listener = this;
    addChildLocked(layer, 101, 0);
}

void UpgradeScene::initNotice() {
    notice_bg = wyColorLayer::make();
    notice_bg->setColor(wyc4b(0, 0, 0, 200));
    notice_bg->setContentSize(wyDevice::winWidth, DP(50)/0.75f*sGlobal->scaleY);
    notice_bg->setPosition(0, wyDevice::winHeight/1.2f-DP(12)*sGlobal->scaleY);
    addChildLocked(notice_bg, 200);
    notice_bg->setVisible(false);
    
    notice_txt = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    notice_txt->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    notice_txt->setAlignment(wyBitmapFontLabel::CENTER);
    notice_txt->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/1.2f+DP(22)*sGlobal->scaleY);
    addChildLocked(notice_txt, 200);
    notice_txt->setVisible(false);
    introTimer = NULL;
}

void UpgradeScene::showNotice(const char *noticeStr, int duration) {
    notice_bg->setVisible(true);
    notice_txt->setVisible(true);
    notice_txt->setText(noticeStr);
    
    if (introTimer != NULL) {
        unscheduleLocked(introTimer);
        introTimer = NULL;
    }
    if (duration != -1) {
        introTimer = wyTimer::make(wyTargetSelector::make(this, SEL(UpgradeScene::hideNotice)), duration+1.0f);
        introTimer->setOneShot(true);
        scheduleLocked(introTimer);
    }
}

void UpgradeScene::hideNotice(wyTargetSelector *ts) {
    notice_txt->setVisible(false);
    notice_bg->setVisible(false);
}

void UpgradeScene::trainUpgradeWeapon() {
    isTraining = true;
    
    sGlobal->offsetGold(sGlobal->weapon_gold[0]);
    
    arms_selectedIndex = -1;
    atk_progress->setPercentage(0);
    speed_progress->setPercentage(0);
    cost_progress->setPercentage(0);
    onArmsBtnClick(NULL, WEAPON_UMP);
    
    showNotice(sGlobal->getIntroStr(STR_INTRO_UPGRADE_WEAPON));
    
    handPointer = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "handpointer.png");
    handPointer->setScaleX(sGlobal->scaleX*0.8f);
    handPointer->setScaleY(sGlobal->scaleY*0.8f);
//    handPointer->setRotation(180);
    handPointer->setAnchor(0.5f, 0.0f);
    arms_detail_panel->addChildLocked(handPointer, 1000);
    
    handPointer->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.4f, 0, DP(30)*sGlobal->scaleY), wyMoveBy::make(0.4f, 0, -DP(30)*sGlobal->scaleY), NULL)));
    
    handPointer->setPosition(arms_atk_bg->getPositionX(), arms_atk_bg->getPositionY());
    
    touchLayer = wyLayer::make();
//    touchLayer->setColor(wyc4b(255, 0, 0, 0));
    touchLayer->setTouchEnabled(true);
    touchLayer->setInterceptTouch(true);
    touchLayer->setTouchPriority(MAX_INT-1);
    addChildLocked(touchLayer);
    
    arms_atk_bg->setTouchEnabled(false);
    arms_atk_bg->setTouchPriority(MAX_INT);
    arms_atk_bg->setTouchEnabled(true);
}

void UpgradeScene::trainSkill() {
    isTraining = true;
    
    sGlobal->offsetGold(sGlobal->skill_gold[0]);
    
    currentTabIndex = -1;
    onTabSkillsClick(NULL);
    
    skills_selectedIndex = -1;
    onSkillsBtnClick(NULL, SKILL_MISSLE);
    
    showNotice(sGlobal->getIntroStr(STR_INTRO_UPGRADE_SKILL));
    
    handPointer = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "handpointer.png");
    handPointer->setScaleX(sGlobal->scaleX*0.8f);
    handPointer->setScaleY(sGlobal->scaleY*0.8f);
    //    handPointer->setRotation(180);
    handPointer->setAnchor(0.5f, 0.0f);
    skills_detail_panel->addChildLocked(handPointer, 1000);
    
    handPointer->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.4f, 0, DP(30)*sGlobal->scaleY), wyMoveBy::make(0.4f, 0, -DP(30)*sGlobal->scaleY), NULL)));
    
    handPointer->setPosition(skills_upgrades_bg->getPositionX(), skills_upgrades_bg->getPositionY());
    
    touchLayer = wyLayer::make();
    touchLayer->setTouchEnabled(true);
    touchLayer->setInterceptTouch(true);
    touchLayer->setTouchPriority(MAX_INT-1);
    addChildLocked(touchLayer);
    
    skills_upgrades_bg->setTouchEnabled(false);
    skills_upgrades_bg->setTouchPriority(MAX_INT);
    skills_upgrades_bg->setTouchEnabled(true);
}

void UpgradeScene::showNextTrain() {
    wyTimer *timer = wyTimer::make(wyTargetSelector::make(this, SEL(UpgradeScene::showNextTrainFunc)), 1.5f);
    timer->setOneShot(true);
    scheduleLocked(timer);
}

void UpgradeScene::showNextTrainFunc(wyTargetSelector *ts) {
    handPointer->stopAllActions();
    handPointer->retain();
    handPointer->getParent()->removeChildLocked(handPointer, false);
    btn_start->getParent()->addChildLocked(handPointer, 1000);
    handPointer->release();
    handPointer->setPosition(btn_start->getPositionX(), btn_start->getPositionY());
    handPointer->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.4f, 0, DP(30)*sGlobal->scaleY), wyMoveBy::make(0.4f, 0, -DP(30)*sGlobal->scaleY), NULL)));
    handPointer->setVisible(true);
    
    btn_start->setTouchEnabled(false);
    btn_start->setTouchPriority(MAX_INT);
    btn_start->setTouchEnabled(true);
    
    showNotice(sGlobal->getIntroStr(STR_INTRO_GOTONEXT));
}

void UpgradeScene::refreshAllArmsGantanhao() {
    
    float fx = 0.6f*sGlobal->scaleX;
    float tx = 0.8*sGlobal->scaleX;
    int count = 0;
    for (int i = 0; i <= 15; i++) {
        int weaponId = sGlobal->WEAPON_SEQ2ID[i];
        wyNode *targetPanel = getArmsPanelByWeaponIndex(weaponId);
        wySpriteEx *gantan = (wySpriteEx*)targetPanel->getChildByTag(weaponId+1000);
//        LOGE("gantan:%d", i);
        if (sGlobal->canWeaponUpgrade(weaponId)) {
            count++;
            if (!gantan->isVisible()) {
                gantan->setVisible(true);
                gantan->setScale(fx);
                gantan->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
            }
        } else {
            gantan->stopAllActions();
            gantan->setVisible(false);
        }
    }
    
    fx = 0.7f*sGlobal->scaleX;
    tx = 0.8*sGlobal->scaleX;
    if (count>0) {
        if (!arms_gantanhao->isVisible()) {
            arms_gantanhao->setVisible(true);
            arms_gantanhao->setScale(fx);
//            arms_gantanhao->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
        }
        char temp[20];
        sprintf(temp, "%d", count);
        arms_gantanhao_number->setText(temp);
    } else {
        arms_gantanhao->stopAllActions();
        arms_gantanhao->setVisible(false);
    }
}

void UpgradeScene::refreshAllSkillsGantanhao() {
    int count = 0;
    float fx = 0.6f*sGlobal->scaleX;
    float tx = 0.8*sGlobal->scaleX;
    for (int i = 0; i < 10; i++) {
        wySpriteEx *gantan = (wySpriteEx*)skills_panel->getChildByTag(i+1000);
        if (sGlobal->canSkillUpgrade(i)) {
            count++;
            if (!gantan->isVisible()) {
                gantan->setVisible(true);
                gantan->setScale(fx);
                gantan->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
            }
        } else {
            gantan->stopAllActions();
            gantan->setVisible(false);
        }
    }

    fx = 0.7f*sGlobal->scaleX;
    tx = 0.8*sGlobal->scaleX;
    if (count > 0) {
        if (!skills_gantanhao->isVisible()) {
            skills_gantanhao->setVisible(true);
            skills_gantanhao->setScale(fx);
//            skills_gantanhao->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(0.6f, fx,fx,tx,tx), wyScaleTo::make(0.6f, tx,tx,fx,fx), NULL)));
        }
        
        char temp[20];
        sprintf(temp, "%d", count);
        skills_gantanhao_number->setText(temp);
    } else {
        skills_gantanhao->stopAllActions();
        skills_gantanhao->setVisible(false);
    }
}

void UpgradeScene::onShopEnd(int result) {
    if (result == 1) {
        updateCoin();
        updateCash();
        if (sGlobal->isBigPackSoldout) {
            shop_sale->setVisible(false);
        }
        
        trainGiftWeapon();
    }
}

void UpgradeScene::onCoinChanged() {
    updateCoin();
    updateCash();
}

void UpgradeScene::trainGiftWeapon() {
    if (sGlobal->isAdsRemoved==1 && sGlobal->weaponUnlockArray[WEAPON_GLB].getValue()==0) {
        onTabArmsClick(NULL);
        if (currentPageNo != 1) {
            pageControl->gotoPage(1);
        }
        onArmsBtnClick(NULL, WEAPON_GLB);
        onArmUpgradeBtnClick(NULL);
        showNotice("congratulations! gift weapon has unlocked!", 5.0f);
    }
}











