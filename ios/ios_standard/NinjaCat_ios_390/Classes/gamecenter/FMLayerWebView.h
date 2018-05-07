//

//  FMLayerWebView.h

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//
#ifndef WebViewDemo_FMLayerWebView_h
#define WebViewDemo_FMLayerWebView_h

#include "WiEngine.h"

class FMLayerWebView : public wyLayer{
    
public:
    FMLayerWebView();
    ~FMLayerWebView();
    
    void webViewDidFinishLoad();
    
    void onBackbuttonClick();
    
    void onEnter();
private:
    int mWebViewLoadCounter;
    
};



#endif