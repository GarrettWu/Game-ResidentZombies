//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ShopLayer.h"
#include "Global.h"
#include "DialogBgLayer.h"
#if IOS
#include "ObjCCalls.h"
#endif
#include "LevelSelectLayer.h"
#include "FreeStuffLayer.h"

ShopLayer::ShopLayer() : BaseMenuLayer() {
    sGlobal->shopLayer = this;
    
    sGlobal->isShopOpening = true;
    
    m_Title->setText("shop");
    btn_back->setButtonClickSelector(wyTargetSelector::make(this, SEL(ShopLayer::onCloseButtonClick)));
    btn_back->setTouchPriority(2003);
    
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(container);

    wySpriteEx *normal;
    
    if (sGlobal->isDebug) {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_common_button.png");
        wyButton *btn_firstpurchase = wyButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(ShopLayer::onBtn1Click)));
        container->addChildLocked(btn_firstpurchase);
        btn_firstpurchase->setPosition(0, wyDevice::winHeight/2);
        btn_firstpurchase->setScaleX(sGlobal->scaleX*0.6f);
        btn_firstpurchase->setScaleY(sGlobal->scaleY*0.5f);
        btn_firstpurchase->setClickScale(1.0f);
        btn_firstpurchase->setTouchPriority(10000);
        btn_firstpurchase->setInterceptTouch(true);
        btn_firstpurchase->setAnchor(0.5, 1.0);
    }
    
    for (int i = 0; i < 6; i++) {
        createItemButton(i);
    }
    
    normal = wySpriteEx::make(MyTextureManager::makePNG("start_scene_tex.png"), wyr(617, 810, 403, 214));
    wySpriteEx *mask = wySpriteEx::make(MyTextureManager::makePNG("start_scene_tex.png"), wyr(617, 810, 403, 214));
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *big_item_btn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(ShopLayer::onShopButtonClick), SHOP_BIGPACK_ID), mask, NULL);
    container->addChildLocked(big_item_btn, 102);
    big_item_btn->setPosition(-DP(150)*sGlobal->scaleX, DP(-62)*sGlobal->scaleY);
    big_item_btn->setScaleX(sGlobal->scaleX*0.56f);
    big_item_btn->setScaleY(sGlobal->scaleY*0.56f);
    big_item_btn->setTouchPriority(3000);
    big_item_btn->setInterceptTouch(true);
    big_item_btn->setMultiTouchClickable(false);
    
    soldout = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_sold_out.png");
    container->addChildLocked(soldout, 200);
    soldout->setPosition(big_item_btn->getPositionX()+DP(90)*sGlobal->scaleX, big_item_btn->getPositionY()-DP(40)*sGlobal->scaleY);
    soldout->setScaleX(sGlobal->scaleX*0.4f);
    soldout->setScaleY(sGlobal->scaleY*0.4f);
    if (sGlobal->isBigPackSoldout) {
        soldout->setVisible(true);
    } else {
        soldout->setVisible(false);
    }
    
    float ofy = -40;
    
    BlinkStar *star1 = new BlinkStar();
    star1->autoRelease();
    container->addChildLocked(star1, 102);
    star1->posx1 = big_item_btn->getPositionX()-DP(82)*sGlobal->scaleX;
    star1->posy1 = big_item_btn->getPositionY()+DP(50+ofy)*sGlobal->scaleY;
    star1->posx2 = big_item_btn->getPositionX()+DP(35)*sGlobal->scaleX,
    star1->posy2 = big_item_btn->getPositionY()+DP(52+ofy)*sGlobal->scaleY;
    
    BlinkStar *star2 = new BlinkStar();
    star2->autoRelease();
    container->addChildLocked(star2, 102);
    star2->posx1 = big_item_btn->getPositionX()+DP(58)*sGlobal->scaleX;
    star2->posy1 = big_item_btn->getPositionY()+DP(76+ofy)*sGlobal->scaleY;
    star2->posx2 = big_item_btn->getPositionX()-DP(48)*sGlobal->scaleX,
    star2->posy2 = big_item_btn->getPositionY()+DP(90+ofy)*sGlobal->scaleY;
    
    normal = wySpriteEx::make(MyTextureManager::makePNG("start_scene_tex.png"), wyr(617, 498, 403, 130));
    mask = wySpriteEx::make(MyTextureManager::makePNG("start_scene_tex.png"), wyr(617, 498, 403, 130));
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_dbcoin = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(ShopLayer::onShopButtonClick), SHOP_DBCOIN_ID), mask, NULL);
    container->addChildLocked(btn_dbcoin, 102);
    btn_dbcoin->setPosition(-DP(150)*sGlobal->scaleX, DP(40)*sGlobal->scaleY);
    btn_dbcoin->setScaleX(sGlobal->scaleX*0.56f);
    btn_dbcoin->setScaleY(sGlobal->scaleY*0.56f);
    btn_dbcoin->setTouchPriority(3000);
    btn_dbcoin->setInterceptTouch(true);
    btn_dbcoin->setMultiTouchClickable(false);
    
    soldout_dbcoin = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_sold_out.png");
    container->addChildLocked(soldout_dbcoin, 200);
    soldout_dbcoin->setPosition(btn_dbcoin->getPositionX()+DP(90)*sGlobal->scaleX, btn_dbcoin->getPositionY()-DP(15)*sGlobal->scaleY);
    soldout_dbcoin->setScaleX(sGlobal->scaleX*0.4f);
    soldout_dbcoin->setScaleY(sGlobal->scaleY*0.4f);
    if (sGlobal->isDoubleCoin) {
        soldout_dbcoin->setVisible(true);
    } else {
        soldout_dbcoin->setVisible(false);
    }
    
    BlinkStar *star22 = new BlinkStar();
    star22->autoRelease();
    container->addChildLocked(star22, 102);
    star22->posx1 = btn_dbcoin->getPositionX()-DP(24)*sGlobal->scaleX;
    star22->posy1 = btn_dbcoin->getPositionY()+DP(20)*sGlobal->scaleY;
    star22->posx2 = btn_dbcoin->getPositionX()-DP(60)*sGlobal->scaleX,
    star22->posy2 = btn_dbcoin->getPositionY()+DP(35)*sGlobal->scaleY;
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "first_purchase_title.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "first_purchase_title.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_fp = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(ShopLayer::onFPButtonClick), SHOP_BIGPACK_ID), mask, NULL);
    container->addChildLocked(btn_fp, 102);
    btn_fp->setPosition(big_item_btn->getPositionX(), DP(130)*sGlobal->scaleY);
    btn_fp->setScaleX(sGlobal->scaleX*0.52f);
    btn_fp->setScaleY(sGlobal->scaleY*0.52f);
    btn_fp->setTouchPriority(3000);
    btn_fp->setInterceptTouch(true);
    btn_fp->setMultiTouchClickable(false);
    
    fp_light = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), 58);
    fp_light->setForceTickMode(true);
	fp_light->setUnitInterval(0.08f);
    fp_light->setScaleX(3.0f*btn_fp->getScaleX());
    fp_light->setScaleY(3.2f*btn_fp->getScaleY());
    container->addChildLocked(fp_light, 102);
    fp_light->setLoopCount(0);
    fp_light->setPosition(btn_fp->getPositionX(), btn_fp->getPositionY()-DP(2)*sGlobal->scaleY);
    fp_light->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    fp_light->setAlpha(210);
    
    
    wyNode *n1 = container->getChildByTag(100);
    wyNode *n2 = container->getChildByTag(101);
    wyNode *n3 = container->getChildByTag(102);
    wyNode *n4 = container->getChildByTag(103);
    wyNode *n5 = container->getChildByTag(104);
    wyNode *n6 = container->getChildByTag(105);
    
    BlinkStar *star4 = new BlinkStar();
    star4->autoRelease();
    container->addChildLocked(star4, 102);
    star4->posx1 = n1->getPositionX()-DP(40)*sGlobal->scaleX;
    star4->posy1 = n1->getPositionY()+DP(42)*sGlobal->scaleY;
    star4->posx2 = n2->getPositionX()-DP(30)*sGlobal->scaleX,
    star4->posy2 = n2->getPositionY()+DP(35)*sGlobal->scaleY;
    
    BlinkStar *star5 = new BlinkStar();
    star5->autoRelease();
    container->addChildLocked(star5, 102);
    star5->posx1 = n3->getPositionX()-DP(50)*sGlobal->scaleX;
    star5->posy1 = n3->getPositionY()+DP(25)*sGlobal->scaleY;
    star5->posx2 = n4->getPositionX()-DP(38)*sGlobal->scaleX,
    star5->posy2 = n4->getPositionY()+DP(40)*sGlobal->scaleY;
    
    BlinkStar *star6 = new BlinkStar();
    star6->autoRelease();
    container->addChildLocked(star6, 102);
    star6->posx1 = n5->getPositionX()-DP(40)*sGlobal->scaleX;
    star6->posy1 = n5->getPositionY()+DP(30)*sGlobal->scaleY;
    star6->posx2 = n6->getPositionX()-DP(15)*sGlobal->scaleX,
    star6->posy2 = n6->getPositionY()+DP(30)*sGlobal->scaleY;
    
    
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(ShopLayer::updateTimer));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);
    
    tickTimer = 0;
    nextTickTime = randRange(1, 2);
    
    listener = NULL;
    isPurchaseSuccess = 0;
    
    this->setTouchEnabled(true);
    this->setTouchPriority(1003);
    this->setInterceptTouch(true);
    
    sGlobal->needReloadTex = false;
}

ShopLayer::~ShopLayer() {
    sGlobal->shopLayer = NULL;
}

void ShopLayer::onEnter() {
    wyLayer::onEnter();
    
//    wyIntervalAction* a2 = wyScaleTo::make(0.6f, 0.8f, 0.8f, 1.0f, 1.0f);
//    wyIntervalAction *a21 = wyEaseElasticOut::make(0.6f, a2);
//    container->runAction(a21);
}

void ShopLayer::updateTimer(wyTargetSelector *ts) {
    fp_light->tick(ts->getDelta());
    tickTimer += ts->getDelta();
    if (tickTimer > nextTickTime) {
        tickTimer = 0;
        fp_light->playAnimation(58);
        nextTickTime = randRange(2, 4);
    }
}

void ShopLayer::onBtn1Click(wyTargetSelector* ts) {
    sGlobal->playEffect(se_button);
    
    sGlobal->resetPrefs();
}

bool ShopLayer::touchesBegan(wyMotionEvent& event) {
//    this->getParent()->removeChildLocked(this, true);
    return true;
}

bool ShopLayer::touchesMoved(wyMotionEvent& event) {
    //    this->getParent()->removeChildLocked(this, true);
    return true;
}

bool ShopLayer::touchesEnded(wyMotionEvent& event) {
//    this->getParent()->removeChildLocked(this, true);
    return true;
}

void ShopLayer::setPurchaseSuccess(int itemId) {
    if (itemId == SHOP_BIGPACK_ID) {
        soldout->setVisible(true);
        soldout->setAlpha(0);
        soldout->runAction(wyFadeIn::make(1.0f));
    }
    
    if (itemId == SHOP_DBCOIN_ID) {
        soldout_dbcoin->setVisible(true);
        soldout_dbcoin->setAlpha(0);
        soldout_dbcoin->runAction(wyFadeIn::make(1.0f));
    }
    
    updateCoin();
    updateCash();
    isPurchaseSuccess = 1;
    
    LOGE("setPurchaseSuccess...");
}

void ShopLayer::onCloseButtonClick(wyTargetSelector* ts) {
    sGlobal->isShopOpening = false;
    btn_back->setEnabled(false);
#if ANDROID
    if (sGlobal->needReloadTex) {
        showUIProgress();
        sGlobal->loadAllImagesAsync(this);
    } else {
        if (listener != NULL) {
            listener->onShopEnd(isPurchaseSuccess);
        }
        
        this->getParent()->removeChildLocked(this, true);
    }
#endif
    
#if IOS
    if (listener != NULL) {
        listener->onShopEnd(isPurchaseSuccess);
    }
    
    this->getParent()->removeChildLocked(this, true);
#endif
}

void ShopLayer::onLoadTexEnd() {
    LOGE("onLoadTexEnd...");
    if (listener != NULL) {
        listener->onShopEnd(isPurchaseSuccess);
    }
    
    this->getParent()->removeChildLocked(this, true);
}

void ShopLayer::onFPButtonClick(wyTargetSelector* ts) {
    PopupDialog *popupDialog = new PopupDialog();
    popupDialog->autoRelease();
    popupDialog->setContent("buy anything to get the super weapon, glb!");
    popupDialog->showOnlyOkBtn();
    addChildLocked(popupDialog, 101, 0);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "first_purchase_title.png");
    normal->setPosition(DP(10)*sGlobal->scaleX, DP(36)*sGlobal->scaleY);
    normal->setScaleX(sGlobal->scaleX*0.52f);
    normal->setScaleY(sGlobal->scaleY*0.52f);
    popupDialog->container->addChildLocked(normal);
    
    popupDialog->okBtn->setPosition(popupDialog->okBtn->getPositionX(), popupDialog->okBtn->getPositionY()-DP(6)*sGlobal->scaleY);
}

void ShopLayer::onShopButtonClick(wyTargetSelector* ts, int data) {
    sGlobal->needReloadTex = true;
    
#if ANDROID
    sGlobal->reportGameEvent(EVT_PRE_BUY, 0, SHOP_GOODS_PRICE[data]);
#endif
    
    if (data == SHOP_BIGPACK_ID) {
        if (sGlobal->isBigPackSoldout) {
            PopupDialog *popupDialog = new PopupDialog();
            popupDialog->autoRelease();
            popupDialog->setContent(OTHER_STR[STR_SHOP_SOLDOUT]);
            popupDialog->showOnlyOkBtn();
            addChildLocked(popupDialog, 101, 0);
            return;
        }
    }
    
    if (data == SHOP_DBCOIN_ID) {
        if (sGlobal->isDoubleCoin) {
            PopupDialog *popupDialog = new PopupDialog();
            popupDialog->autoRelease();
            popupDialog->setContent("you have already get this.");
            popupDialog->showOnlyOkBtn();
            addChildLocked(popupDialog, 101, 0);
            return;
        }
    }
#if ANDROID
    sGlobal->purchaseGameItemStart(data);
#endif
    
#if IOS
    if (data == 3) {
        sGlobal->addGem(10);
    }else {
        sGlobal->addGold(2000);
    }
    sGlobal->isAdsRemoved = 1;
    MyPrefs::setInt(DB_IS_ADS_REMOVED, 1);
    
    isPurchaseSuccess = 1;
    
#endif
}

void ShopLayer::createItemButton(int index) {
    float ofx = DP(148)*sGlobal->scaleX * (index/3);
    float ofy = -DP(97)*sGlobal->scaleY * (index%3);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_common_button.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_common_button.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_item_0 = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(ShopLayer::onShopButtonClick), index), mask, NULL);
    container->addChildLocked(btn_item_0, 99, 100+index);
    btn_item_0->setPosition(DP(60)*sGlobal->scaleX+ofx, DP(105)*sGlobal->scaleY+ofy);
    btn_item_0->setScaleX(sGlobal->scaleX*0.58f);
    btn_item_0->setScaleY(sGlobal->scaleY*0.58f);
    btn_item_0->setTouchPriority(3000);
    btn_item_0->setInterceptTouch(true);
    btn_item_0->setMultiTouchClickable(false);
    
    wySpriteEx *item_0_icon;
    if (index < 3) {
        item_0_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_coin.png");
        item_0_icon->setPosition(btn_item_0->getPositionX()-DP(45)*sGlobal->scaleX, btn_item_0->getPositionY()+DP(24)*sGlobal->scaleY);
    } else {
        item_0_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_cash.png");
        item_0_icon->setPosition(btn_item_0->getPositionX()-DP(40)*sGlobal->scaleX, btn_item_0->getPositionY()+DP(24)*sGlobal->scaleY);
    }
    container->addChildLocked(item_0_icon, 101);
    item_0_icon->setScaleX(sGlobal->scaleX*0.58f);
    item_0_icon->setScaleY(sGlobal->scaleY*0.58f);
    
    char temp[20];
    sprintf(temp, "shop_%d.png", index);
    wySpriteEx *item_0_number = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", temp);
    container->addChildLocked(item_0_number, 101);
    item_0_number->setPosition(btn_item_0->getPositionX()+DP(22)*sGlobal->scaleX, btn_item_0->getPositionY()+DP(16)*sGlobal->scaleY);
    item_0_number->setScaleX(sGlobal->scaleX*0.58f);
    item_0_number->setScaleY(sGlobal->scaleY*0.58f);
    
//    addComma(SHOP_GOODS_NUM[index], temp);
//    wyBitmapFontLabel *item_0_title = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
//    item_0_title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
//    item_0_title->setAlignment(wyBitmapFontLabel::LEFT);
//    item_0_title->setPosition(btn_item_0->getPositionX()+DP(22)*sGlobal->scaleX, btn_item_0->getPositionY()+DP(16)*sGlobal->scaleY);
//    container->addChildLocked(item_0_title, 101);
    
    sprintf(temp, "$%.2f", SHOP_GOODS_PRICE[index]);
    wyBitmapFontLabel *item_0_price = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    item_0_price->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    item_0_price->setAlignment(wyBitmapFontLabel::LEFT);
    item_0_price->setPosition(btn_item_0->getPositionX()+DP(22)*sGlobal->scaleX, btn_item_0->getPositionY()-DP(16)*sGlobal->scaleY);
    container->addChildLocked(item_0_price, 101);
}

