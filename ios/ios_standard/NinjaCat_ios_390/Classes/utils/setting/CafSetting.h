//
//  CafSetting.h
//  APU2
//
//  Created by 宁 许 on 12-3-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CafSetting : NSObject

/**
 * 得到设置的文字
 */
+ (NSString*) getString:(NSString*)key;

/**
 * 得到设置的文字
 */
+ (void) setString:(NSString*)key value:(NSString*)value;

@end
