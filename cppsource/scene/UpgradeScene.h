//
//  UpgradeScene.h
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef _UpgradeScene_h
#define _UpgradeScene_h

#include "WiEngine.h"
#include "LevelSelectLayer.h"
#include "Global.h"
#include "CoinChangeListener.h"
#include "ArmsPageControl.h"

const static int TAB_ARMS = 0;
const static int TAB_SKILLS = 1;

class LineLayer;

class UpgradeScene: public wyScene, public ShopListener, public CoinChangeListener{
public:
    UpgradeScene(int mapIndex = 0);
    virtual ~UpgradeScene();
    
    int gotoMapIndex;
    LineLayer *lineLayer;
    
    int currentTabIndex;
    
    wySpriteEx *m_bg;
    ControlButton *btn_back, *btn_start, *btn_shop;
    wySpriteEx *shop_sale;
    
    //创建skill面板
    wyNode *skills_panel, *skills_cursor_container;
    wySpriteEx *skills_cursor_center, *skill_badge, *skill_badge_mask;
    wyNode *skills_equip_container;
    int skills_selectedIndex;
    void onSkillsBtnClick(wyTargetSelector* ts, int data);
    void createSkillsPanel();
    
    //创建arms面板
    wyNode *arms_panel, *arms_cursor_container;
    wySpriteEx *arms_cursor_center, *equip_badge, *equip_badge_mask;
    int arms_selectedIndex;
    void onArmsBtnClick(wyTargetSelector* ts, int data);
    void createArmsPanel();
    
    //创建tab按钮
    ControlButton *tab_arms_bg, *tab_upgrades_bg;
    void createTabs();
    void onTabArmsClick(wyTargetSelector* ts);
    void onTabSkillsClick(wyTargetSelector* ts);
    
    //创建技能详细信息
    wyNode *skills_detail_panel;
    wyBitmapFontLabel *skills_title, *skills_desc, *skills_gold, *skills_unlock_notice;//, *skills_upgrades_title
    ControlButton *skills_upgrades_bg;
    wySpriteEx *skills_coin_icon, *skills_cash_icon, *skills_unlock_icon;
    wyNode *skill_prop_panel, *skill_up_panel;
    wyNode *skills_lvmax_container;
    wySpriteEx *skills_lvmax_bg;
    wyBitmapFontLabel *skills_lvmax_title;
    wySpriteEx *skills_title_bg;
    void createDetailPanelForSkills();
    void updateSkillInfo(int skillIndex);
    void onSkillUpgradeBtnClick(wyTargetSelector* ts);
    void generateSkillProp(int skillIndex);
    
    //创建武器详细信息
    wyNode *arms_detail_panel, *arms_ammo_container;
    wyBitmapFontLabel *arms_title, *arms_desc, *arms_desc_sub, *arms_atk_prop, *arms_atk_gold, *arms_ammo_prop, *arms_ammo_gold;//, *arms_atk_title, *arms_ammo_title
    wySpriteEx *arms_title_bg;
    ControlButton *arms_atk_bg, *arms_ammo_bg;
    wyBitmapFontLabel *atk_progress_title, *speed_progress_title, *cost_progress_title;
    wyProgressTimer *atk_progress, *speed_progress, *cost_progress;
    wySpriteEx *coin_icon1, *coin_icon2, *cash_icon;
    wyNode *arms_lvmax_container, *arms_ammo_unlimit_container;
    wySpriteEx *arms_lvmax_bg, *arms_ammo_unlimit_bg;
    wyBitmapFontLabel *arms_lvmax_title, *arms_ammo_unlimit_title;
    void createDetailPanelForArms();
    void onArmUpgradeBtnClick(wyTargetSelector* ts);
    void onArmAmmoBtnClick(wyTargetSelector* ts);
    void updateWeaponInfo(int weaponIndex);
    
    wyBitmapFontLabel *coin_num, *cash_num;
    void createMoneyPanel();
    void updateCoin();
    void updateCash();
    
    wyNode *freeContainer;
    wySpriteEx *free_coin_icon, *free_cash_icon, *free_star, *free_light, *free_word;
    ControlButton *free_button_bg;
    void createFreePanel();
    
    bool keyUp(wyKeyEvent& event);
    void onEnter();
    void onEnterTransitionDidFinish();
    
    void createFuncButtons();
    void createShopButton();
    
    
    void showGoldNotEnoughDialog(int type);
    
    
    void onBackBtnClick(wyTargetSelector* ts);
    void onStartBtnClick(wyTargetSelector* ts);
    void onShopBtnClick(wyTargetSelector* ts);
    void onFreeBtnClick(wyTargetSelector* ts);
    
    virtual void draw();
    
    wyBitmapFontLabel *levelName, *levelMode;
    
    wySpriteEx *v_line, *h_line;
    int middleX;
    int panelWidth;
    int leftX;
    int rightX;
    
    wyBitmapFontLabel *notice_txt;
    wyColorLayer *notice_bg;
    wyTimer *introTimer;
    void showNotice(const char *noticeStr, int duration = -1);
    void hideNotice(wyTargetSelector *ts);
    void initNotice();
    
    wySpriteEx *handPointer;
    void trainUpgradeWeapon();
    void trainSkill();
    bool isTraining;
    wyNode *touchLayer;
    
    void showNextTrain();
    void showNextTrainFunc(wyTargetSelector *ts);
    
    wySpriteEx *arms_ammo_icon;
    float starTimer, nextStarTime;
    float lightTimer, nextlightTime;
    void updateTimer(wyTargetSelector *ts);
    
    wyNode *arms_gantanhao, *skills_gantanhao;
    wyBitmapFontLabel *arms_gantanhao_number, *skills_gantanhao_number;
    void refreshAllArmsGantanhao();
    void refreshAllSkillsGantanhao();
    
    void onShopEnd(int result);
    void onCoinChanged();
    
    wyBitmapFontLabel *tip1, *tip2, *curTip, *nextTip;
    float tipTimer;
    void updateTip(wyTargetSelector *ts);
    int curTipIndex, nextTipIndex, tipcount;
    void changeNextTip(wyTargetSelector* ts);
    
    ArmsPageControl *pageControl;
    wyColorLayer *arms_panel1, *arms_panel2, *arms_panel3;
    void onArmsNextPageClicked(wyTargetSelector *ts);
    void onArmsPrevPageClicked(wyTargetSelector *ts);
    void createArmsSubPanel(int page);
    
    float arms_spWidth, arms_spHeight, arms_offsetX, arms_offsetY, arms_pageWidth, arms_pageHeight;
    wyNode* getArmsPanelByWeaponIndex(int index);
    wyNode* getArmsPanelByPageIndex(int index);
    bool needEquip;
    void onArmsCursorMoveEnd(wyTargetSelector *ts, void *data);
    void onSkillsCursorMoveEnd(wyTargetSelector *ts, void *data);
    ControlButton *btn_arms_next, *btn_arms_prev;
    int currentPageNo;
    bool needEquipSkill;
    
    void removeStarMask(wyTargetSelector* ts, void* data);
    
    void onGantanDisappeared(wyTargetSelector *ts, int data);
    
    void recoverMaskBlink(wyTargetSelector* ts, void* data);
    void trainGiftWeapon();
};

class LineLayer : public wyLayer {
public:
    UpgradeScene *parent;
    float lineWidth, halfLineWidth;
    wySpriteEx *arrow1, *arrow2, *arrow3, *arrow4, *arrow5, *arrow6, *arrow7;
    wySpriteEx *line1, *line2, *line31, *line32, *line33, *line34, *line35, *line7, *line8, *line9;
    
    LineLayer(UpgradeScene *scene) {
        parent = scene;
        lineWidth = DP(4)*sGlobal->scaleX;
        halfLineWidth = lineWidth/2;
        
        ControlButton *node0 = (ControlButton*)parent->skills_panel->getChildByTag(0);
        ControlButton *node1 = (ControlButton*)parent->skills_panel->getChildByTag(1);
        ControlButton *node2 = (ControlButton*)parent->skills_panel->getChildByTag(2);
        ControlButton *node3 = (ControlButton*)parent->skills_panel->getChildByTag(3);
        ControlButton *node4 = (ControlButton*)parent->skills_panel->getChildByTag(4);
        ControlButton *node5 = (ControlButton*)parent->skills_panel->getChildByTag(5);
        ControlButton *node6 = (ControlButton*)parent->skills_panel->getChildByTag(6);
        ControlButton *node7 = (ControlButton*)parent->skills_panel->getChildByTag(7);
        ControlButton *node8 = (ControlButton*)parent->skills_panel->getChildByTag(8);
        ControlButton *node9 = (ControlButton*)parent->skills_panel->getChildByTag(9);
        
        arrow1 = createArrow(node1);
        arrow2 = createArrow(node2);
        arrow3 = createArrow(node4);
        arrow4 = createArrow(node5);
        arrow5 = createArrow(node6);
        arrow6 = createArrow(node8);
        arrow7 = createArrow(node9);

        float sx = DP(40)*sGlobal->scaleX;
        line1 = createLine(node1, node0->getPositionX(), node0->getPositionY(), node1->getPositionX()-sx, node1->getPositionY());
        line2 = createLine(node2, node1->getPositionX(), node1->getPositionY(), node2->getPositionX()-sx, node2->getPositionY());
        
        float x1 = (node4->getPositionX() - node3->getPositionX()) / 2 + node3->getPositionX();
        float y1 = node3->getPositionY();
        line31 = createLine(node5, node3->getPositionX(), node3->getPositionY(), x1+halfLineWidth, node3->getPositionY());
        line32 = createLine(node4, x1, y1+halfLineWidth, x1, node4->getPositionY()+halfLineWidth, true);
        line33 = createLine(node4, x1, node4->getPositionY(), node4->getPositionX()-sx, node4->getPositionY());
        line34 = createLine(node5, x1, node5->getPositionY()-halfLineWidth, x1, y1-halfLineWidth, true);
        line35 = createLine(node5, x1, node5->getPositionY(), node5->getPositionX()-sx, node5->getPositionY());
        
        line7 = createLine(node6, node5->getPositionX(), node5->getPositionY(), node6->getPositionX()-sx, node6->getPositionY());
        line8 = createLine(node8, node7->getPositionX(), node7->getPositionY(), node8->getPositionX()-sx, node8->getPositionY());
        line9 = createLine(node9, node8->getPositionX(), node8->getPositionY(), node9->getPositionX()-sx, node9->getPositionY());
    }
    ~LineLayer() {
        
    }
    
    wySpriteEx* createArrow(ControlButton *node) {
        wyColor3B grayColor = wyc3b(72,72,72);
        float ofx = -DP(24)*sGlobal->scaleX;
        float ofy = -DP(0)*sGlobal->scaleX;
        
        wySpriteEx* arrow = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow_upgrade.png");
        arrow->setScaleX(sGlobal->scaleX*0.4f);
        arrow->setScaleY(sGlobal->scaleY*0.3f);
        arrow->setAnchor(1.0f, 0.5f);
        arrow->setPosition(node->getPositionX()+ofx, node->getPositionY()+ofy);
        addChildLocked(arrow);

        if (!sGlobal->isSkillUnlockable(node->getTag())) {
            arrow->setColor(wyc3b(72,72,72));
        } else {
            arrow->setColor(wyc3b(147,62,39));
        }
        return arrow;
    }
    
    wySpriteEx* createLine(ControlButton *node, float fromX, float fromY, float toX, float toY, bool isVertical = false) {
        wySpriteEx* line = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "upgrade_line.png");

        int lineWidth = toX - fromX;
        int lineHeight = toY - fromY;
        if (!isVertical) {
            line->setScaleX(lineWidth/line->getContentSize().width);
            line->setAnchor(0, 0.5f);
        } else {
            line->setScaleX(DP(4)*sGlobal->scaleX);
            line->setAnchor(0.5f, 0);
        }
        if (isVertical) {
            line->setScaleY(lineHeight/line->getContentSize().height);
            line->setAnchor(0.5f, 0);
        } else {
            line->setScaleY(DP(4)*sGlobal->scaleX);
            line->setAnchor(0, 0.5f);
        }

        line->setPosition(fromX, fromY);
        addChildLocked(line);
        
        if (!sGlobal->isSkillUnlockable(node->getTag())) {
            line->setColor(wyc3b(72,72,72));
        } else {
            line->setColor(wyc3b(147,62,39));
        }
        return line;
    }
    
    void updateLineColor(int skillId) {
        wyColor3B color = wyc3b(147,62,39);

        switch (skillId) {
            case SKILL_ROBOT:
                arrow1->setColor(color);
                line1->setColor(color);

                runHighlightArrow(arrow1);
                runHighlightLine(line1);
                break;
            case SKILL_SHIELD:
                arrow2->setColor(color);
                line2->setColor(color);

                runHighlightArrow(arrow2);
                runHighlightLine(line2);
                break;
            case SKILL_ARMOR:
                arrow3->setColor(color);

                line31->setColor(color);
                line32->setColor(color);
                line33->setColor(color);

                runHighlightArrow(arrow3);
                runHighlightLine(line31);
                runHighlightLine(line32);
                runHighlightLine(line33);
                
                break;
            case SKILL_STAMINA:
                arrow4->setColor(color);
                
                line31->setColor(color);
                line34->setColor(color);
                line35->setColor(color);

                runHighlightArrow(arrow4);
                runHighlightLine(line31);
                runHighlightLine(line34);
                runHighlightLine(line35);
                break;
            case SKILL_EVASION:
                arrow5->setColor(color);
                line7->setColor(color);

                runHighlightArrow(arrow5);
                runHighlightLine(line7);
                break;
            case SKILL_CRITICAL:
                arrow6->setColor(color);
                line8->setColor(color);
                
                runHighlightArrow(arrow6);
                runHighlightLine(line8);
                break;
            case SKILL_STUN:
                arrow7->setColor(color);
                line9->setColor(color);

                runHighlightArrow(arrow7);
                runHighlightLine(line9);
                break;
            default:
                break;
        }
    }
    
    static void onLineAnimEnd(wyAction* action, void* data) {
        wyNode *node = (wyNode *)data;
        node->getParent()->removeChildLocked(node, true);
    }
    
    void runHighlightLine(wyNode *node) {
        wySpriteEx* line = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "upgrade_line.png");
        line->setScaleX(node->getScaleX());
        line->setScaleY(node->getScaleY());
        line->setPosition(node->getPositionX(), node->getPositionY());
        line->setColor(wyc3b(147,62,39));
        line->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        line->setAnchor(node->getAnchorX(), node->getAnchorY());
        addChildLocked(line);
        wyIntervalAction *a = wySequence::make(wyFadeIn::make(0.3f), wyFadeOut::make(0.3f), wyFadeIn::make(0.3f), wyFadeOut::make(0.3f), NULL);
        wyActionCallback callback = {
            NULL,
            onLineAnimEnd,
            NULL
        };
        a->setCallback(&callback, line);
        line->runAction(a);
    }
    
    void runHighlightArrow(wyNode *node) {
        wySpriteEx* line = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow_upgrade.png");
        line->setScaleX(node->getScaleX());
        line->setScaleY(node->getScaleY());
        line->setPosition(node->getPositionX(), node->getPositionY());
        line->setColor(wyc3b(147,62,39));
        line->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        line->setAnchor(node->getAnchorX(), node->getAnchorY());
        addChildLocked(line);
        wyIntervalAction *a = wySequence::make(wyFadeIn::make(0.3f), wyFadeOut::make(0.3f), wyFadeIn::make(0.3f), wyFadeOut::make(0.3f), NULL);
        wyActionCallback callback = {
            NULL,
            onLineAnimEnd,
            NULL
        };
        a->setCallback(&callback, line);
        line->runAction(a);
        
    }
    
};
#endif
