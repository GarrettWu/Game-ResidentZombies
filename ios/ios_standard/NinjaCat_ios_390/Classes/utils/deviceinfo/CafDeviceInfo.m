//
//  CafDeviceInfo.m
//  Apu_windows_base
//
//  Created by ning xu on 11-10-25.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "CafDeviceInfo.h"
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import "CafSetting.h"
//extern NSString *CTSettingCopyMyPhoneNumber();

@implementation CafDeviceInfo

/* 
 * 得到设备的udid，这个是一个字符型的值，udid是唯一标识机器的唯一id
 */
+(NSString*) getUDID{
    return [CafDeviceInfo macaddress];
//    UIDevice *myDevice = [UIDevice currentDevice];
//    return [[myDevice uniqueIdentifier] retain];
}

/* 
 * 得到设备的udid，这个是一个字符型的值，udid是唯一标识机器的唯一id
 */
+(NSString*) getUDID2{
//    return [CafDeviceInfo macaddress];
    UIDevice *myDevice = [UIDevice currentDevice];
    return [[myDevice uniqueIdentifier] retain];
}

/*
 * 得到设备的系统版本，这个是一个字符串的值，例如：4.3.5
 */
+(NSString*) getVersion{
    UIDevice *myDevice = [UIDevice currentDevice];
    return [[myDevice systemVersion] retain];
}

/**
 * 得到屏幕横竖，0为竖屏 1为横屏
 */
+(int) getScreenMode{
    return [NSLocalizedStringFromTable(@"APU_SCREEN", @"Localizable", nil) intValue];
}

/*
 * 得到屏幕的宽，如果是iPhone3GS则等于320或者480, 如果是iPhone4则等于640或者960，如果是iPad则等于768或者1024
 */
+(int) getScreenWidth{
    CGSize size = [[[UIScreen mainScreen] currentMode] size];
    if ([CafDeviceInfo getScreenMode] == 0) {
        return size.width;
    }
    else {
        return size.height;
    }
    
}

/*
 * 得到屏幕的高，如果是iPhone3GS则等于320或者480, 如果是iPhone4则等于640或者960，如果是iPad则等于768或者1024
 */
+(int) getScreenHeight{
    CGSize size = [[[UIScreen mainScreen] currentMode] size];
    if ([CafDeviceInfo getScreenMode] == 0) {
        return size.height;
    }
    else {
        return size.width;
    }
    
}

/*
 * 得到平台是什么，iPhone3GS的值等于0，iPhone4的值等于1，iPad2的值等于2，未知设备的值等于-1
 */
+(int) getModel{
    switch ([CafDeviceInfo getScreenWidth]) {
        case 320:
        case 480:
            return 0;
        case 640:
        case 960:
            return 1;
        case 768:
        case 1024:
            return 2;
        default:
            return -1;
    }
}

/**
 * 得到平台
 */
+ (NSString*) getModelStr{
    UIDevice *myDevice = [UIDevice currentDevice];
    return [[myDevice model] retain];
}

/**
 * 得到国家代码
 */
+ (NSString*) getCountryCode{
    return [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
}

/**
 *
 */
+ (NSString *)getLanguageCode{
    return [[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode];
}

/**
 * 得到appVersion，这个Apucid是游戏开始运行的时候从，Localizable.strings文件的APP_VERSION字段中取得的
 */
+ (NSString*) appVersion{
    return NSLocalizedStringFromTable (@"APP_VERSION",@"Localizable", nil);
}

/**
 * 得到本机地址
 */
+(NSString *)myPhone{
    return @"";//CTSettingCopyMyPhoneNumber();        
}

/**
 * 得到MAC地址
 */
+ (NSString *) macaddress
{
	int                    mib[6];
	size_t                len;
	char                *buf;
	unsigned char        *ptr;
	struct if_msghdr    *ifm;
	struct sockaddr_dl    *sdl;
	
	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;
	
	if ((mib[5] = if_nametoindex("en0")) == 0) {
		printf("Error: if_nametoindex error/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		printf("Error: sysctl, take 1/n");
		return NULL;
	}
	
	if ((buf = (char *)malloc(len)) == NULL) {
		printf("Could not allocate memory. error!/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		printf("Error: sysctl, take 2");
		return NULL;
	}
	
	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	// NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	NSString *outstring = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	free(buf);
	return [outstring uppercaseString];
	
}

/**
 * 判断是否越狱
 */
+ (BOOL)isJailbroken{
    if ([[NSFileManager defaultManager] fileExistsAtPath:@"/Applications/Cydia.app"]) {  
        return YES;
    }  
    return NO;
}

/**
 * 得到Url区分越狱和非越狱
 */
+ (void) openUrl:(NSString*)url{
    NSArray *b = [url componentsSeparatedByString:@"|"];
    
    for (int i = 0; i < [b count]; i++) {
        if ([CafDeviceInfo isJailbroken] == NO) {
            break;
        }
        NSString *sub = [b objectAtIndex:i];
        NSRange foundObj=[sub rangeOfString:@"itms-services://" options:NSCaseInsensitiveSearch];
        if(foundObj.length>0) { 
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:sub]];
            return;
        }
    }
    
    for (int i = 0; i < [b count]; i++) {
        NSString *sub = [b objectAtIndex:i];
        NSRange foundObj=[sub rangeOfString:@"http://" options:NSCaseInsensitiveSearch];
        if(foundObj.length>0 && foundObj.location==0) { 
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:sub]];
            return;
        }
    }
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

/**
 * 得到一些设定数值
 */
+ (NSString*) getLocalizable:(NSString*)key{
    NSString* value = [CafSetting getString:key];
    if (value != nil && [value isEqualToString:@""] == NO) {
        return value;
    }
    
    return NSLocalizedStringFromTable(key, @"Localizable", nil);
}


@end
