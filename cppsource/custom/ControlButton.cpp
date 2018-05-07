//
//  CDynamicSprite.mm
//  Bubbsy
//
//  Created by steve green on 18/05/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "ControlButton.h"
#include "Global.h"

ControlButton::ControlButton(wyNode* normal, wyNode* selected, wyNode* disabled, wyNode* focused, wyTargetSelector* downSelector, wyTargetSelector* upSelector, wyNode *mask, wyBitmapFontLabel *title) : wyButton(normal, selected, disabled, focused, NULL, wyTargetSelector::make(this, SEL(ControlButton::onControlButtonClick))) {
    
    m_Mask = mask;
    m_Title = title;
    
    if (m_Mask != NULL) {
        m_Mask->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
        m_Mask->setVisible(false);
        addChildLocked(m_Mask);
    }
    
    if (m_Title != NULL) {
        m_Title->setAlignment(wyBitmapFontLabel::LEFT);
        m_Title->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
        addChildLocked(m_Title);
    }
    
    if (upSelector != NULL) {
        m_UpSelector = upSelector;
        m_UpSelector->retain();
    } else {
        m_UpSelector = NULL;
    }
    
    needDelay = true;
    this->setContentSize(normal->getWidth(), normal->getHeight());
    //        this->setMoveOutSelector(wyTargetSelector::make(this, SEL(ControlButton::onControlButtonMoveOut)));
    
    userData = -1;
}

ControlButton::~ControlButton() {
    wyObjectRelease(m_UpSelector);
}

static void onMaskBlinkEnd(wyAction* action, void* data) {
    ControlButton *button = (ControlButton *)data;
    
    button->stopAllActions();
    button->touchLayer->getParent()->removeChildLocked(button->touchLayer, true);
    if (button->m_UpSelector != NULL) {
        button->m_UpSelector->invoke();
    }
}

void ControlButton::onControlButtonClick(wyTargetSelector* ts, int data) {
    m_Mask->setVisible(true);
    m_Mask->stopAllActions();
    
    sGlobal->playEffect(se_button);
    if (needDelay) {
        wyAction *a = wyFadeOut::make(0.2f);//wySequence::make(wyFadeTo::make(0.03f, 100, 255), wyFadeOut::make(0.08f), NULL);//wyFadeTo::make(0.3f, 100, 0);//
        wyActionCallback callback = {
            NULL,
            onMaskBlinkEnd,
            NULL
        };
        a->setCallback(&callback, this);
        m_Mask->runAction(a);
        
        touchLayer = TouchLayer::make();
        wyDirector* director = wyDirector::getInstance();
        director->getRunningScene()->addChildLocked(touchLayer);
    } else {
        wyAction *a = wyFadeOut::make(0.2f);
        m_Mask->runAction(a);
        
        if (m_UpSelector != NULL) {
            m_UpSelector->invoke();
        }
    }
}

void ControlButton::setSelected(bool selected) {
    if (m_enabled && selected != m_selected) {
        wyNode::setSelected(selected);
        
        if(selected) {
            m_Mask->stopAllActions();
            m_Mask->setVisible(true);
            m_Mask->setAlpha(100);
        } else {
            m_Mask->setVisible(false);
        }
    }
}

void ControlButton::draw() {
    wyNode* node = m_normalState;
    
    if(node) {
        glTranslatef((getWidth() - node->getWidth()) / 2, (getHeight() - node->getHeight()) / 2, 0);
        
        node->draw();
    }
}

void ControlButton::setButtonClickSelector(wyTargetSelector* ts) {
    if (m_UpSelector != NULL) {
        m_UpSelector->release();
    }
    if (ts != NULL) {
        m_UpSelector = ts;
        ts->retain();
    }
}

void ControlButton::setMask(wyNode *mask) {
    if (m_Mask != NULL) {
        m_Mask->getParent()->removeChildLocked(m_Mask, true);
    }
    
    m_Mask = mask;
    m_Mask->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    m_Mask->setVisible(false);
    addChildLocked(m_Mask);
}
