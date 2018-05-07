//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"
#include "BaseMenuLayer.h"

class ShopListener {
public:
    //result: 0:success; 1:failed
    virtual void onShopEnd(int result) {
        
    }
};

class LoadTexListener {
public:
    virtual void onLoadTexEnd() {
        
    }
};

class ShopLayer : public BaseMenuLayer, LoadTexListener {
public:
    wyNode *container;
    int parentType, prevId;
    ShopListener *listener;
    
    ~ShopLayer();
    
    void onEnter();
    
    ShopLayer();
    
    void onBtn1Click(wyTargetSelector* ts);
    
    void onCloseButtonClick(wyTargetSelector* ts);
    void onFPButtonClick(wyTargetSelector* ts);
    
    void onShopButtonClick(wyTargetSelector* ts, int data);

    bool touchesBegan(wyMotionEvent& event);
    bool touchesMoved(wyMotionEvent& event);
    bool touchesEnded(wyMotionEvent& e);

    void createItemButton(int index);
    
    int isPurchaseSuccess;
    void setPurchaseSuccess(int itemId);
    wySpriteEx *soldout, *soldout_dbcoin;
    
//    bool needReloadTex;
    void onLoadTexEnd();
    
    wySPXSprite *fp_light;
    float tickTimer;
    float nextTickTime;
    void updateTimer(wyTargetSelector *ts);
};
