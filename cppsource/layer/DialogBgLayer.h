//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"

class DialogBgLayer : public wyColorLayer {
public:
    DialogBgLayer();
    virtual ~DialogBgLayer();
    
    static DialogBgLayer* make() {
        DialogBgLayer *layer = new DialogBgLayer();
        layer->autoRelease();
        return layer;
    }
};



