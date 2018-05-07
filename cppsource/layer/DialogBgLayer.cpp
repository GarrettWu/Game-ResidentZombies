//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "DialogBgLayer.h"
#include "Global.h"

DialogBgLayer::DialogBgLayer() {
//    MyGradientColorLayer *layer1 = MyGradientColorLayer::make(wyc4b(45, 45, 45, 255), wyc4b(45, 45, 45, 255), 90);//27, 27, 27, 255
//    layer1->setContentSize(DP(520)*sGlobal->scaleX, DP(48)*sGlobal->scaleY);
//    layer1->setPosition(-layer1->getContentSize().width/2, DP(250)*sGlobal->scaleY/2);
//    addChildLocked(layer1);
//    
//    wyColorLayer *layer = wyColorLayer::make();
//    layer->setColor(wyc4b(45, 45, 45, 255));
//    layer->setContentSize(DP(520)*sGlobal->scaleX, DP(250)*sGlobal->scaleY);
//    layer->setPosition(-layer->getContentSize().width/2, -layer->getContentSize().height/2);
//    addChildLocked(layer);
//    
//    MyGradientColorLayer *layer2 = MyGradientColorLayer::make(wyc4b(45, 45, 45, 255), wyc4b(45, 45, 45, 255), 90);//20, 20, 20, 255
//    layer2->setContentSize(DP(520)*sGlobal->scaleX, DP(50)*sGlobal->scaleY);
//    layer2->setPosition(-layer2->getContentSize().width/2, -layer->getContentSize().height/2-layer2->getContentSize().height);
//    addChildLocked(layer2);
    
//    MyGradientColorLayer *layer1 = MyGradientColorLayer::make(wyc4b(45, 45, 45, 255), wyc4b(45, 45, 45, 255), 90);//27, 27, 27, 255
//    layer1->setContentSize(DP(520)*sGlobal->scaleX, DP(48)*sGlobal->scaleY);
//    layer1->setPosition(-layer1->getContentSize().width/2, DP(250)*sGlobal->scaleY/2);
//    addChildLocked(layer1);
    
    wyColorLayer *layer = wyColorLayer::make();
    layer->setColor(wyc4b(45, 45, 45, 255));
    layer->setContentSize(DP(518)*sGlobal->scaleX, DP(348)*sGlobal->scaleY);
    layer->setPosition(-layer->getContentSize().width/2, -layer->getContentSize().height/2);
    addChildLocked(layer);
//    layer->setVisible(false);
    
//    MyGradientColorLayer *layer2 = MyGradientColorLayer::make(wyc4b(45, 45, 45, 255), wyc4b(45, 45, 45, 255), 90);//20, 20, 20, 255
//    layer2->setContentSize(DP(520)*sGlobal->scaleX, DP(50)*sGlobal->scaleY);
//    layer2->setPosition(-layer2->getContentSize().width/2, -layer->getContentSize().height/2-layer2->getContentSize().height);
//    addChildLocked(layer2);
    
    wySpriteEx *deco1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco1.png");
    deco1->setScaleX(sGlobal->scaleX*1.0f);
    deco1->setScaleY(sGlobal->scaleY*1.0f);
    deco1->setPosition(DP(140)*sGlobal->scaleX, -DP(70)*sGlobal->scaleY);
    addChildLocked(deco1);
    
    wySpriteEx *deco2 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco2.png");
    deco2->setScaleX(sGlobal->scaleX*1.0f);
    deco2->setScaleY(sGlobal->scaleY*1.0f);
    deco2->setPosition(-DP(160)*sGlobal->scaleX, -DP(145)*sGlobal->scaleY);
    addChildLocked(deco2);
    
    wySpriteEx *deco3 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco3.png");
    deco3->setScaleX(sGlobal->scaleX*1.0f);
    deco3->setScaleY(sGlobal->scaleY*1.0f);
    deco3->setPosition(-DP(50)*sGlobal->scaleX, -DP(140)*sGlobal->scaleY);
    addChildLocked(deco3);
    
    wySpriteEx *deco4 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco4.png");
    deco4->setScaleX(sGlobal->scaleX*1.0f);
    deco4->setScaleY(sGlobal->scaleY*1.0f);
    deco4->setPosition(-DP(240)*sGlobal->scaleX, -DP(135)*sGlobal->scaleY);
    addChildLocked(deco4);
    
    wySpriteEx *deco5 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco5.png");
    deco5->setScaleX(sGlobal->scaleX*1.0f);
    deco5->setScaleY(sGlobal->scaleY*1.0f);
    deco5->setPosition(-DP(240)*sGlobal->scaleX, DP(50)*sGlobal->scaleY);
    addChildLocked(deco5);
    
    wySpriteEx *deco6 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco6.png");
    deco6->setScaleX(sGlobal->scaleX*1.0f);
    deco6->setScaleY(sGlobal->scaleY*1.0f);
    deco6->setPosition(-DP(215)*sGlobal->scaleX, DP(137)*sGlobal->scaleY);
    addChildLocked(deco6);
    
    wySpriteEx *deco7 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco7.png");
    deco7->setScaleX(sGlobal->scaleX*1.0f);
    deco7->setScaleY(sGlobal->scaleY*1.0f);
    deco7->setPosition(-DP(120)*sGlobal->scaleX, DP(142)*sGlobal->scaleY);
    addChildLocked(deco7);
    
    wySpriteEx *deco8 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco8.png");
    deco8->setScaleX(sGlobal->scaleX*1.0f);
    deco8->setScaleY(sGlobal->scaleY*1.0f);
    deco8->setPosition(DP(164)*sGlobal->scaleX, DP(152)*sGlobal->scaleY);
    addChildLocked(deco8);
    
    wySpriteEx *deco9 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco9.png");
    deco9->setScaleX(sGlobal->scaleX*1.0f);
    deco9->setScaleY(sGlobal->scaleY*1.0f);
    deco9->setPosition(DP(220)*sGlobal->scaleX, -DP(140)*sGlobal->scaleY);
    addChildLocked(deco9);
    
    wySpriteEx *deco10 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "deco10.png");
    deco10->setScaleX(sGlobal->scaleX*1.0f);
    deco10->setScaleY(sGlobal->scaleY*1.0f);
    deco10->setPosition(DP(230)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
    addChildLocked(deco10);
    
    wySpriteEx *bottom = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "border_bottom.png");
    bottom->setScaleX(sGlobal->scaleX*0.9f);
    bottom->setScaleY(sGlobal->scaleY*0.9f);
    bottom->setAnchor(0, 0);
    bottom->setPosition(layer->getPositionX()+DP(10)*sGlobal->scaleX, -layer->getContentSize().height/2-DP(10)*sGlobal->scaleY);
    addChildLocked(bottom);
    
    wySpriteEx *left = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "border_left.png");
    left->setScaleX(sGlobal->scaleX*0.9f);
    left->setScaleY(sGlobal->scaleY*0.9f);
    left->setAnchor(1.0, 0);
    left->setPosition(bottom->getPositionX(), bottom->getPositionY());
    addChildLocked(left);
    
    wySpriteEx *top = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "border_top.png");
    top->setScaleX(sGlobal->scaleX*0.9f);
    top->setScaleY(sGlobal->scaleY*0.9f);
    top->setAnchor(0, 1.0);
    top->setPosition(bottom->getPositionX(), left->getPositionY()+left->getHeight()*left->getScaleY());
    addChildLocked(top);
    
    wySpriteEx *right = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "border_right.png");
    right->setScaleX(sGlobal->scaleX*0.9f);
    right->setScaleY(sGlobal->scaleY*0.9f);
    right->setAnchor(0, 0);
    right->setPosition(bottom->getPositionX()+bottom->getWidth()*bottom->getScaleX(), bottom->getPositionY());
    addChildLocked(right);
    
    this->setScale(1.04f);
    this->setPosition(layer->getContentSize().width*0.04f/2+DP(0)*sGlobal->scaleY, layer->getContentSize().height*0.04/2+DP(4)*sGlobal->scaleY);
//    -layer1->getContentSize().width/2, DP(250)*sGlobal->scaleY/2
    this->setContentSize(layer->getContentSize().width, layer->getContentSize().height);
}

DialogBgLayer::~DialogBgLayer() {

}
