//
//  CafPaid.h
//  ApuV2
//
//  Created by BooSo on 12-1-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "ASIHTTPRequest.h"

@class ASINetworkQueue;

#define PAID_YES @"YES"
#define PAID_NO @"NO"

@interface ECPurchaseHTTPRequest:ASIHTTPRequest{
	NSString *_productIdentifier;
    BOOL restore;
}
@property(nonatomic,retain) NSString *productIdentifier;
@property BOOL restore;
@end


@interface CafPaid : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>{
    
    // 回调的类
    id target_;
    
    // 回调的方法
    SEL selector_;
    
    // 回调的类
    id restoreTarget_;
    
    // 回调的方法
    SEL restoreSelector_;
    
    // 付费ID
    NSString *payid_;
    
    // 金额
    int money_;
    
    // ASI队列
    ASINetworkQueue *networkQueue_;
    
    // restore的商品id集合
    NSMutableArray *restorePayIds_;
}

@property (nonatomic, retain) NSMutableArray *restorePayIds;

/**
 * 得到唯一的对象
 */
+ (CafPaid*) shared;

/**
 * 显示网络链接中提示框
 */
+(void) showWaitAlert;

/**
 * 关闭网络链接中提示框
 */
+(void) hideWaitAlert;

/**
 * 检查是否是内购插件
 */
- (void) checkVerifyReceipt;

/**
 * 检查是否是内购插件检测回复
 */
-(void)checkDidFinishVerify:(ECPurchaseHTTPRequest *)request;

/**
 * 购买一个商品
 */
- (void) paid:(NSString*) pid money:(int)usd target:(id)target selector:(SEL)selector;

/**
 * 设置restore回调
 */
- (void)setRestoreTarget:(id)target selector:(SEL)selector;

/**
 * 付费的服务器确认
 */
- (void) verifyReceipt:(SKPaymentTransaction *)transaction;

/**
 * 付费检测回复
 */
-(void)didFinishVerify:(ECPurchaseHTTPRequest *)request;

/**
 * 恢复消息
 */
- (void) callBack:(NSString *) pid success:(NSString*) isOk restore:(BOOL) restore;

- (void)setCallBackWithTarget:(id)target selector:(SEL)selector;

-(void) restorePurchases;
@end
