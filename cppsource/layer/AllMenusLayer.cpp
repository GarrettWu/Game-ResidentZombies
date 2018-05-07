//
//  AllMenusLayer.cpp
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "AllMenusLayer.h"
#include "Global.h"
#include "DailyRewardLayer.h"

AllMenusLayer::AllMenusLayer() : BaseMenuLayer() {
    m_Title->setText("menus");
    btn_back->setButtonClickSelector(wyTargetSelector::make(this, SEL(AllMenusLayer::onBackBtnClick)));
    
    createShopButton();
    
    setKeyEnabled(true);
    setKeyPriority(1);
    
    float margin = DP(30)*sGlobal->scaleX;
    float yy = DP(180)*sGlobal->scaleY;
    for (int i = 0; i < 8; i++) {
        wySpriteEx *sp;
        wySpriteEx *mask;
        wySEL ts;
        const char *title;
        switch (i) {
            case 0:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_shop.png");//wySpriteEx::make(MyTextureManager::makePNG("menu_shop.png"));
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_shop.png");
                title = "shop";
                break;
            case 1:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_free_stuff.png");//wySpriteEx::make(MyTextureManager::makePNG("menu_free_stuff.png"));
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_free_stuff.png");//wySpriteEx::make(MyTextureManager::makePNG("menu_free_stuff.png"));
                title = "free stuff";
                break;
            case 2:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_missions.png");
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_missions.png");
                title = "missions";
                break;
            case 3:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_statistics.png");
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_statistics.png");
                title = "statistics";
                break;
            case 4:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_leaderboards.png");
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_leaderboards.png");
                title = "leaderboards";
                break;
            case 5:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_settings.png");
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_settings.png");
                title = "settings";
                break;
//            case 6:
//                sp = wySpriteEx::make(MyTextureManager::makePNG("menu_moregames.png"));
//                mask = wySpriteEx::make(MyTextureManager::makePNG("menu_moregames.png"));
//                title = "more games";
//                break;
            case 6:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_tutorial.png");
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "menu_tutorial.png");
                title = "tutorial";
                break;
            case 7:
                sp = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "menu_challenges.png");
                mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "menu_challenges.png");
                title = "daily reward";
                break;
            default:
                break;
        }
        mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        ControlButton *button2 = ControlButton::make(sp, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(AllMenusLayer::onMenusBtnClicked), i), mask, NULL);
        button2->setAnchor(0, 0);
        button2->setInterceptTouch(true);
        button2->setTouchPriority(301);
        button2->setScaleX(sGlobal->scaleX*0.7f);
        button2->setScaleY(sGlobal->scaleY*0.7f);
        button2->setTouchPriority(1002);
        addChildLocked(button2);
        
        int j = i / 5;
        button2->setPosition((wyDevice::winWidth-margin*2)/10*(i%5*2+1)-sp->getWidth()*sp->getScaleX()/2 + margin, wyDevice::winHeight - yy - DP(140)*j*sGlobal->scaleY);
        
        wyBitmapFontLabel *btn_title = wyBitmapFontLabel::make(sGlobal->font_desc, title);
        btn_title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
        btn_title->setAlignment(wyBitmapFontLabel::LEFT);
        btn_title->setPosition(button2->getPositionX()+sp->getWidth()*sp->getScaleX()/2, button2->getPositionY()+DP(6)*sGlobal->scaleY);
        btn_title->setAnchor(0.5f, 1.0f);
        addChildLocked(btn_title);
    }
}

AllMenusLayer::~AllMenusLayer() {

}

bool AllMenusLayer::keyUp(wyKeyEvent& event) {
    switch(event.keyCode) {
        case KEYCODE_BACK:
            return true;
        default:
            break;
    }
    
    return wyLayer::keyUp(event);
}

void AllMenusLayer::onEnter() {
    wyLayer::onEnter();

#if ANDROID
    showAd(1);
#endif
}

void AllMenusLayer::onExit() {
    wyLayer::onExit();
    
#if ANDROID
    hideAd();
#endif
}

void AllMenusLayer::createFuncButtons() {
    
}

void AllMenusLayer::createShopButton() {

}

void AllMenusLayer::onBackBtnClick(wyTargetSelector* ts) {
    this->getParent()->removeChildLocked(this, true);
}

void AllMenusLayer::onMenusBtnClicked(wyTargetSelector* ts, int menuId) {
    if (menuId == MENU_SHOP) {
        ShopLayer *slayer = new ShopLayer();
        slayer->listener = this;
        
        slayer->autoRelease();
        addChildLocked(slayer, 9999);
        
        if (sGlobal->isDebug) {
            sGlobal->gold.offset(100000000);
            sGlobal->zuanshi.offset(1000000);
            
            MyPrefs::setInt(DB_GOLD, sGlobal->gold.getValue());
            MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
        }
    }
    else if (menuId == MENU_FREE_STUFF) {
#if ANDROID
        showOfferWall(0);
#endif
    }
    else {
        sGlobal->showMenu(this, menuId);
    }
    
    if (menuId == MENU_FREE_STUFF) {
#if ANDROID
        sGlobal->reportGameEvent(EVT_OFFER_MENU);
#endif
    }
    
    if (menuId == MENU_SHOP) {
#if ANDROID
        sGlobal->reportGameEvent(EVT_SHOP_MENU);
#endif
    }
    
    if (menuId == MENU_DAILY_REWARD) {
        DailyRewardLayer *layer = new DailyRewardLayer(true, true);
        layer->autoRelease();
        addChildLocked(layer, 101, 0);
    }
}

void AllMenusLayer::onShopEnd(int result) {
    if (result == 1) {
        updateCoin();
        updateCash();
    }
}



