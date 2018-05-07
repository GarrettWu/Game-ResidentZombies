//
//  GLViewController.h
//  NinjaCat_ios_390
//
//  Created by  on 12-3-6.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WYEAGLView.h"

@interface GLViewController : UIViewController <WYEAGLViewDelegate> {
//    WYEAGLView *view;
//    ApuAd *apuAd;
    bool isAdShowing;
}

+(GLViewController *) instance;

- (void)showAd;

- (void)hideAd;

@end
