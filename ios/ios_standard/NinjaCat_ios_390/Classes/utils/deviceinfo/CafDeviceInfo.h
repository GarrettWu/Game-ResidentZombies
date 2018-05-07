//
//  CafDeviceInfo.h
//  Apu_windows_base
//
//  Created by ning xu on 11-10-25.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CafDeviceInfo : NSObject

/* 
 * 得到设备的udid，这个是一个字符型的值，udid是唯一标识机器的唯一id
 */
+(NSString*) getUDID;

/* 
 * 得到设备的udid，这个是一个字符型的值，udid是唯一标识机器的唯一id
 */
+(NSString*) getUDID2;

/*
 * 得到设备的系统版本，这个是一个字符串的值，例如：4.3.5
 */
+(NSString*) getVersion;

/**
 * 得到屏幕横竖，0为竖屏 1为横屏
 */
+(int) getScreenMode;

/*
 * 得到屏幕的宽，如果是iPhone3GS则等于320或者480, 如果是iPhone4则等于640或者960，如果是iPad则等于768或者1024
 */
+(int) getScreenWidth;

/*
 * 得到屏幕的高，如果是iPhone3GS则等于320或者480, 如果是iPhone4则等于640或者960，如果是iPad则等于768或者1024
 */
+(int) getScreenHeight;

/*
 * 得到平台是什么，iPhone3GS的值等于0，iPhone4的值等于1，iPad2的值等于2，未知设备的值等于-1
 */
+(int) getModel;

/**
 * 得到平台
 */
+(NSString*) getModelStr;

/**
 * 得到国家代码
 */
+ (NSString*) getCountryCode;

/**
 * 得到语言代码
 */
+ (NSString *)getLanguageCode;

/**
 * 得到appVersion，这个Apucid是游戏开始运行的时候从，Localizable.strings文件的APP_VERSION字段中取得的
 */
+ (NSString*) appVersion;

/**
 * 得到本机电话号码
 */
+(NSString *)myPhone;

/**
 * 得到MAC地址
 */
+ (NSString *) macaddress;

/**
 * 判断是否越狱
 */
+ (BOOL)isJailbroken;

/**
 * 得到Url区分越狱和非越狱
 */
+ (void) openUrl:(NSString*)url;

/**
 * 得到一些设定数值
 */
+ (NSString*) getLocalizable:(NSString*)key; 

@end
