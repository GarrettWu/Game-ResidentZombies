//
//  LogoScene.h
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef _LogoScene_h
#define _LogoScene_h

#include "WiEngine.h"
#include "ShopLayer.h"

class LogoScene: public wyScene, LoadTexListener {
public:
    LogoScene();
    virtual ~LogoScene();
    
    void loadingCallBack(wyTargetSelector* ts);
    void loadingCallBack1(wyTargetSelector* ts);
    
    wySprite *sprite1, *sprite2;
    
    void onEnter();
    void onEnterTransitionDidFinish();
    
    void onLoadTexEnd();
};
#endif
