//

//  FMLayerWebView.mm

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//
#include "FMLayerWebView.h"
#include "FMUIWebViewBridge.h"

static FMUIWebViewBridge *g_FMUIWebViewBridge=nil;

FMLayerWebView::FMLayerWebView(){
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    [g_FMUIWebViewBridge setLayerWebView : this URLString:"http://www.boo.so/qiuxin/moregame.html"];
}

FMLayerWebView::~FMLayerWebView(){
    [g_FMUIWebViewBridge release];
}

void FMLayerWebView::webViewDidFinishLoad(){
    
}

static void onMoveOutEnd(wyAction* action, void* data) {
    wyNode *layer = (wyNode *)data;
    layer->getParent()->removeChild(layer, true);
}

void FMLayerWebView::onBackbuttonClick(){
//    wyIntervalAction *move = wyMoveBy::make(2.5f, 0, -wyDevice::winHeight);
//    wyActionCallback callback = {
//        NULL,
//        onMoveOutEnd,
//        NULL,
//    };
//    move->setCallback(&callback, this);
//    this->runAction(move);
    this->getParent()->removeChild(this, true);
}

void FMLayerWebView::onEnter() {
    wyLayer::onEnter();
}