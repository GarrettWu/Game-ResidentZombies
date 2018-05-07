#pragma once

#include "WiEngine.h"

class TouchLayer : public wyLayer {
public:
    TouchLayer() : wyLayer() {
        this->setTouchEnabled(true);
        this->setTouchPriority(MAX_INT);
        this->setInterceptTouch(true);
        
        this->setContentSize(wyDevice::winWidth, wyDevice::winHeight);
//        this->setColor(wyc4b(255, 0, 0, 100));
    }
    
    static TouchLayer* make() {
        TouchLayer *layer = new TouchLayer();
        layer->autoRelease();
        return layer;
    }
    
    bool touchesBegan(wyMotionEvent& event) {
        return true;
    }
    
    bool touchesMoved(wyMotionEvent& event) {
        return true;
    }
    
    bool touchesEnded(wyMotionEvent& event) {
        return true;
    }
};

class ControlButton : public wyButton {
public:
    wyNode *m_Mask;
    wyBitmapFontLabel *m_Title;
    wyTargetSelector *m_UpSelector;
    TouchLayer *touchLayer;
    bool needDelay;
    int userData;
    
    ControlButton(wyNode* normal, wyNode* selected, wyNode* disabled, wyNode* focused, wyTargetSelector* downSelector, wyTargetSelector* upSelector, wyNode *mask, wyBitmapFontLabel *title);
    
    ~ControlButton();
    
    static ControlButton* make(wyNode* normal, wyNode* selected, wyNode* disabled, wyNode* focused, wyTargetSelector* downSelector, wyTargetSelector* upSelector, wyNode *mask, wyBitmapFontLabel *title) {
        ControlButton *button = new ControlButton(normal, selected, disabled, focused, downSelector, upSelector, mask, title);
        button->autoRelease();
        return button;
    }
    
//    void onControlButtonMoveOut(wyTargetSelector* ts, int data) {
////        m_Mask->setVisible(false);
//        setSelected(false);
//    }
//    
//    void onControlButtonDown(wyTargetSelector* ts, int data) {
////        m_Mask->setVisible(true);
////        m_Mask->setAlpha(100);
//        setSelected(true);
//    }
    
    void setMask(wyNode *mask);
    
    void onControlButtonClick(wyTargetSelector* ts, int data);
    
    void setSelected(bool selected);
    
    void draw();
    
    void setButtonClickSelector(wyTargetSelector* ts);

};