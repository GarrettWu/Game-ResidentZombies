//
//  AllMenusLayer.h
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef _AllMenusLayer_h
#define _AllMenusLayer_h

#include "WiEngine.h"
//#include "ControlButton.h"
#include "ShopLayer.h"
#include "BaseMenuLayer.h"

class AllMenusLayer: public BaseMenuLayer, public ShopListener {
public:
    AllMenusLayer();
    virtual ~AllMenusLayer();
    
    bool keyUp(wyKeyEvent& event);
    void onEnter();
    void onExit();
    
    void createFuncButtons();
    void createShopButton();
    
    void onBackBtnClick(wyTargetSelector* ts);
    void onStartBtnClick(wyTargetSelector* ts);
    
    void onMenusBtnClicked(wyTargetSelector* ts, int menuId);
    
    void onShopEnd(int result);
};

#endif
