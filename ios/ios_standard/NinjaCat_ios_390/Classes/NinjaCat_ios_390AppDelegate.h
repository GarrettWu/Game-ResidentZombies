//
//  NinjaCat_ios_390AppDelegate.h
//  NinjaCat_ios_390
//
//  Created by  on 12-3-6.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GLViewController;

@interface NinjaCat_ios_390AppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet GLViewController *viewController;

+(id) instance;

- (void) paid:(NSString*)pid money:(int)money;
@end
