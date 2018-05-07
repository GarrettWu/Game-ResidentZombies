//
//  CafSetting.m
//  APU2
//
//  Created by 宁 许 on 12-3-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "CafSetting.h"

@implementation CafSetting


/**
 * 得到设置的文字
 */
+ (NSString*) getString:(NSString*)key{
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    return [prefs stringForKey:key];
}

/**
 * 得到设置的文字
 */
+ (void) setString:(NSString*)key value:(NSString*)value{
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    [prefs setValue:value forKey:key];
}

@end
