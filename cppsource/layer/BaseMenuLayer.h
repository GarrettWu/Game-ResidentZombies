//
//  AllMenusLayer.h
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef _BaseMenusLayer_h
#define _BaseMenusLayer_h

#include "WiEngine.h"
#include "ControlButton.h"
#include "CoinChangeListener.h"

class BaseMenuLayer : public  wyLayer, public CoinChangeListener {
public:
    void createMoneyPanel();
    wyBitmapFontLabel *m_Title;
    wyBitmapFontLabel *coin_num, *cash_num;
    wySpriteEx *h_line, *v_line;
    ControlButton *btn_back;
    
    void updateCoin();
    void updateCash();
    
    BaseMenuLayer();
    ~BaseMenuLayer();
    
    void onCoinChanged();
};

#endif
