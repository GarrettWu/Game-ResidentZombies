#pragma once

#include "WiEngine.h"
#include "Global.h"

class ConfirmDialogLayer : public wyLayer {
public:
    wyNode *container;
    int parentType, prevId;
    ShopListener *listener;
    
    ~ConfirmDialogLayer() {
        
    }
    
    void onEnter();
    
    ConfirmDialogLayer(int parentType, int prevId, int type);
    
    void onBtn1Click(wyTargetSelector* ts);
    
    void onBtn2Click(wyTargetSelector* ts);
    
    bool touchesBegan(wyMotionEvent& event);
    
    wyButton *okBtn, *cancelBtn;
};


class NanDuSelectLayer : public wyLayer {
public:
    wyNode *container;
    int parentType, prevId;
    
    ~NanDuSelectLayer() {
        
    }
    
    void onEnter();
    
    NanDuSelectLayer();
    
    void onNanduBtnClick(wyTargetSelector* ts, int data);
    
    void onCloseButtonClick(wyTargetSelector* ts);
    
    bool touchesBegan(wyMotionEvent& event);
    bool touchesMoved(wyMotionEvent& event);
    bool touchesEnded(wyMotionEvent& e);
    
    wyButton *okBtn, *cancelBtn;
    bool isEnterAnimEnd;
    
    void onDifDisabled(wyTargetSelector* ts, void *data);
    
    void onUnlockEmitterEnd(wyTargetSelector* ts, void *data);
    
//    TouchLayer *touchLayer;
    void showUnlockEffect(ControlButton *btn);
};

class PopupDialog : public wyLayer {
public:
    wyNode *container;
    wyBitmapFontLabel *content;
    ControlButton *okBtn, *cancelBtn;
    wyTargetSelector* okSelector, *cancelSelector;
    bool isEnterAnimEnd;
    DialogBgLayer *bgLayer;
    
    PopupDialog();
    
    ~PopupDialog() {
        
    }
    
    void onEnter();

    bool touchesBegan(wyMotionEvent& event);
    
    void setContent(const char *text);
    void setOkButtonSelector(wyTargetSelector* sel);
    void setCancelButtonSelector(wyTargetSelector* sel);
    
    void defaultOnOkButtonClick(wyTargetSelector* ts);
    void defaultOnCancelButtonClick(wyTargetSelector* ts);
    
    void showOnlyOkBtn();
};




