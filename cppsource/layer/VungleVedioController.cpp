//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "VungleVedioController.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "UpgradeScene.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#import "ObjCCalls.h"
#endif
#include "DialogBgLayer.h"
#include "ShopLayer.h"

int VungleVedioController::playTimes = 0;
int VungleVedioController::period = 0;
int VungleVedioController::completeTimes = 0;

VungleVedioController::VungleVedioController() {
    
}

void VungleVedioController::onEnter() {
    wyLayer::onEnter();

}





