//
//  CafPaid.m
//  ApuV2
//
//  Created by BooSo on 12-1-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "CafPaid.h"
#import "CafDeviceInfo.h"
#import "CafSetting.h"
#import "GTMBase64.h"
#import "SBJsonWriter.h"
#import "SBJsonParser.h"
#import "ASINetworkQueue.h"

#define VAILDATING_RECEIPTS_URL @"https://buy.itunes.apple.com/verifyReceipt"

/**
 * 付费的唯一对象
 */
static CafPaid *shared_;

/**
 * 等待提示框
 */
static UIAlertView *alert;

@implementation ECPurchaseHTTPRequest:ASIHTTPRequest
@synthesize productIdentifier;
@synthesize restore;
@end

@implementation CafPaid

@synthesize restorePayIds = restorePayIds_;

/**
 * 得到唯一的对象
 */
+ (CafPaid*) shared{
    if (shared_ == nil) {
        shared_ = [[[CafPaid alloc] init] autorelease];
        [shared_ retain];
    }
    
    return shared_;
}

/**
 * 初始化
 */
- (id) init{
    if((self=[super init])) {
        // 添加InApp
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(alertPurchaseSuccess) name:@"kInAppPurchaseManagerTransactionSucceededNotification" object:nil];
        
        // 检查是否是安装IAP Free
        [self checkVerifyReceipt];
    }
    
    return self;
}

/**
 * 检查是否是内购插件
 */
- (void) checkVerifyReceipt{
    
    // 是否收集本机号码
    if ([[CafSetting getString:@"APU_CHECK_IAP_FREE"] intValue] != 1) {
        return;
    }
    
	networkQueue_ = [ASINetworkQueue queue];
	[networkQueue_ retain];
    
	NSURL *verifyURL = [NSURL URLWithString:VAILDATING_RECEIPTS_URL];
	ECPurchaseHTTPRequest *request = [[ECPurchaseHTTPRequest alloc] initWithURL:verifyURL];
	[request setProductIdentifier:@""];
	[request setRequestMethod: @"POST"];
	[request setDelegate:self];
	[request setDidFinishSelector:@selector(checkDidFinishVerify:)];
	[request addRequestHeader: @"Content-Type" value: @"application/json"];
    [request setRestore:NO];

	NSDictionary* data = [NSDictionary dictionaryWithObjectsAndKeys:@"", @"receipt-data", nil];
	SBJsonWriter *writer = [SBJsonWriter new];
	[request appendPostData: [[writer stringWithObject:data] dataUsingEncoding: NSUTF8StringEncoding]];
	[writer release];
	[networkQueue_ addOperation: request];
	[networkQueue_ go];
}

/**
 * 检查是否是内购插件检测回复
 */
-(void)checkDidFinishVerify:(ECPurchaseHTTPRequest *)request{
    NSString *response = [request responseString];
	SBJsonParser *parser = [SBJsonParser new];
	NSDictionary* jsonData = [parser objectWithString: response];
	[parser release];
	NSString *status = [jsonData objectForKey: @"status"];
	if ([status intValue] == 0) {
        [CafSetting setString:@"APU_IAP_HAVE_IAP_FREE" value:@"YES"];
	}
	else {
        [CafSetting setString:@"APU_IAP_HAVE_IAP_FREE" value:@"NO"];
	}
}

/**
 * 购买一个商品
 */
- (void) paid:(NSString*) pid money:(int)usd target:(id)target selector:(SEL)selector
{
    // 纪录回调方法
    target_ = target;
    selector_ = selector;
    payid_ = pid;
    money_ = usd;
    
    // 判断是否是越狱，并且还安装了Iap Free，则不允许购买商品
    if ([CafDeviceInfo isJailbroken] == YES && [[CafSetting getString:@"APU_IAP_HAVE_IAP_FREE"] isEqualToString:@"YES"]) {
        [self callBack:pid success:PAID_NO restore:NO];
        return;
    }
    
    // 弹出等待对话框
    [CafPaid showWaitAlert];
    
    // 发送到IAP服务器上
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:pid];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

/**
 * 设置restore回调
 */
- (void)setRestoreTarget:(id)target selector:(SEL)selector
{
    restoreTarget_ = target;
    restoreSelector_ = selector;
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    NSMutableArray *array = [[[NSMutableArray alloc] init] autorelease];
    for (int i = 0; i < [transactions count]; i++) {
        SKPaymentTransaction *transaction = (SKPaymentTransaction *)[transactions objectAtIndex:i];
        [array addObject:transaction];
    }
    
    NSMutableArray *hasPayRestoreIds_ = [[[NSMutableArray alloc] init] autorelease];
    for (int i = 0; i < [array count]; i++) {
        // 得到处理
        SKPaymentTransaction *transaction = (SKPaymentTransaction *)[array objectAtIndex:i];
        
        // 没有完成的处理不操作
        if(transaction.transactionState == SKPaymentTransactionStatePurchasing){
            continue;
        }
        
        // 去掉原有的提示框
        [CafPaid hideWaitAlert];
        
        if (transaction.transactionState == SKPaymentTransactionStateRestored) {
            [self restoreTransaction:transaction];
            continue;
        }
        
        // 内计费不成功
        if (transaction.transactionState == SKPaymentTransactionStateFailed){
            [self callBack:transaction.payment.productIdentifier success:PAID_NO restore:NO];
            
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
        }
        // 内计费成功，且非越狱用户，不需要验证消费订单
        else if([CafDeviceInfo isJailbroken] == NO){
            [self callBack:transaction.payment.productIdentifier success:PAID_YES restore:NO];
            
            BOOL isNeedRemove = YES;
            for (NSString *str in restorePayIds_) {
                if ([str isEqualToString:transaction.payment.productIdentifier]) {
                    isNeedRemove = NO;
                    [hasPayRestoreIds_ addObject:transaction.payment.productIdentifier];
                    
                    break;
                }
            }
            
            // 如果需要移出则从队列中移出
            if (isNeedRemove == YES) {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            }
        }
        // 内计费成功，且是越狱用户，必须验证消费订单
        else {
            [self verifyReceipt:transaction];
            
            BOOL isNeedRemove = YES;
            for (NSString *str in restorePayIds_) {
                if ([str isEqualToString:transaction.payment.productIdentifier]) {
                    isNeedRemove = NO;
                    [hasPayRestoreIds_ addObject:transaction.payment.productIdentifier];
                    
                    break;
                }
            }
            
            // 如果需要移出则从队列中移出
            if (isNeedRemove == YES) {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            }
        }
    }
    
    if ([hasPayRestoreIds_ count] > 0) {
        if (restoreTarget_ != nil && [restoreTarget_ respondsToSelector:restoreSelector_]) {
            [restoreTarget_ performSelector:restoreSelector_ withObject:hasPayRestoreIds_];
        }
    }
    
//    NSLog(@"paymentQueue");
}

/**
 * 付费的服务器确认
 */
- (void) verifyReceipt:(SKPaymentTransaction *)transaction{
    
	networkQueue_ = [ASINetworkQueue queue];
	[networkQueue_ retain];
    
	NSURL *verifyURL = [NSURL URLWithString:VAILDATING_RECEIPTS_URL];
	ECPurchaseHTTPRequest *request = [[ECPurchaseHTTPRequest alloc] initWithURL:verifyURL];
	[request setProductIdentifier:transaction.payment.productIdentifier];
	[request setRequestMethod: @"POST"];
	[request setDelegate:self];
	[request setDidFinishSelector:@selector(didFinishVerify:)];
	[request addRequestHeader: @"Content-Type" value: @"application/json"];
    [request setRestore:YES];
    if(transaction.transactionState == SKPaymentTransactionStatePurchased){
        [request setRestore:NO];
    }
	
	NSString *recepit = [GTMBase64 stringByEncodingData:transaction.transactionReceipt];
	NSDictionary* data = [NSDictionary dictionaryWithObjectsAndKeys:recepit, @"receipt-data", nil];
	SBJsonWriter *writer = [SBJsonWriter new];
	[request appendPostData: [[writer stringWithObject:data] dataUsingEncoding: NSUTF8StringEncoding]];
	[writer release];
	[networkQueue_ addOperation: request];
	[networkQueue_ go];
}

-(void)didFinishVerify:(ECPurchaseHTTPRequest *)request
{
	NSString *response = [request responseString];
	SBJsonParser *parser = [SBJsonParser new];
	NSDictionary* jsonData = [parser objectWithString: response];
	[parser release];
	NSString *status = [jsonData objectForKey: @"status"];
	if ([status intValue] == 0) {
		NSDictionary *receipt = [jsonData objectForKey: @"receipt"];
		NSString *productIdentifier = [receipt objectForKey: @"product_id"];
		[self callBack:productIdentifier success:PAID_YES restore:[request restore]];
	}
	else {
        NSDictionary *receipt = [jsonData objectForKey: @"receipt"];
		NSString *productIdentifier = [receipt objectForKey: @"product_id"];
        [self callBack:productIdentifier success:PAID_NO restore:[request restore]];
	}
}

/**
 * 回复消息
 */
- (void) callBack:(NSString *) pid success:(NSString*)isOk restore:(BOOL) restore{
    NSLog(@"success:%@", isOk);
    
    // 将数据发到具体的处理函数中
    if (target_ != nil) {
        if ([target_ respondsToSelector:selector_]) {
            [target_ performSelector:selector_ withObject:pid withObject:isOk];
        }
    }
}

/**
 * 显示网络链接中提示框
 */
+(void) showWaitAlert{
    if (alert == nil) {
        // 得到屏幕的尺寸
        alert = [[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable (@"alert_connect_title",@"Localizable", nil) message:nil delegate:nil cancelButtonTitle:nil otherButtonTitles:nil, nil];
        UIActivityIndicatorView *activeView = [[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge] autorelease];
        [activeView startAnimating];
        [alert addSubview:activeView];
        
        [alert show];
        activeView.center = CGPointMake(alert.bounds.size.width * 0.5f, alert.bounds.size.height - 40.0f);  
    }
}

/**
 * 关闭网络链接中提示框
 */
+(void) hideWaitAlert{
    if (alert != nil) {
        [alert dismissWithClickedButtonIndex:0 animated:NO];
        [alert release];
        alert = nil;
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{}
- (void)recordTransaction:(SKPaymentTransaction *)transaction{}
- (void)provideContent:(NSString *)productId{}
- (void)finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful{}
- (void)completeTransaction:(SKPaymentTransaction *)transaction{}
//- (void)restoreTransaction:(SKPaymentTransaction *)transaction{}
- (void)failedTransaction:(SKPaymentTransaction *)transaction{}

- (void)setCallBackWithTarget:(id)target selector:(SEL)selector
{
    target_ = target;
    selector_ = selector;
    if ([target respondsToSelector:selector]) {
        NSLog(@"oksama...");
    }
}

-(void) restorePurchases {
    [CafPaid showWaitAlert];
    
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentQueue *)queue{
    NSLog(@"restore processing...");
//    NSLog(@"恢复产品:%@", transaction.);
    NSMutableArray *array = [[[NSMutableArray alloc] init] autorelease];
    for (int i = 0; i < [queue.transactions count]; i++) {
        SKPaymentTransaction *transaction = (SKPaymentTransaction *)[queue.transactions objectAtIndex:i];
        [array addObject:transaction];
    }
    
    NSMutableArray *hasPayRestoreIds_ = [[[NSMutableArray alloc] init] autorelease];
    for (int i = 0; i < [array count]; i++) {
        // 得到处理
        SKPaymentTransaction *transaction = (SKPaymentTransaction *)[array objectAtIndex:i];
        
        NSLog(@"restoring:%@", transaction.payment.productIdentifier);
        [hasPayRestoreIds_ addObject:transaction.payment.productIdentifier];
    }
    
    if ([hasPayRestoreIds_ count] > 0) {
        if (restoreTarget_ != nil && [restoreTarget_ respondsToSelector:restoreSelector_]) {
            [restoreTarget_ performSelector:restoreSelector_ withObject:hasPayRestoreIds_];
        }
    }
    
    [CafPaid hideWaitAlert];
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"Restore purchase...");
    
}

-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error{
    NSLog(@"restore failed...");
    
    [CafPaid hideWaitAlert];
}

@end
