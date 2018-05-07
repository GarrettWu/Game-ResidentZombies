/*
 * Copyright (c) 2010 WiYun Inc.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __wyLog_h__
#define __wyLog_h__

#if ANDROID
	#include <android/log.h>
#elif IOS || MACOSX
	#import <Foundation/Foundation.h>
#endif

#define ISLOGON 1

#if ANDROID
	#undef LOG_TAG
	#undef LOGD
	#undef LOGW
	#undef LOGE
	#define LOG_TAG "libwiengine"
	#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
	#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#if ISLOGON
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
    #define LOGE(...) ;
#endif

#elif IOS || MACOSX
	#define LOGD(fmt, ...) NSLog(@fmt, ##__VA_ARGS__)
	#define LOGW(fmt, ...) NSLog(@fmt, ##__VA_ARGS__) 
//	#define LOGE(fmt, ...) NSLog(@fmt, ##__VA_ARGS__)
#if ISLOGON
#define LOGE(fmt, ...) NSLog(@fmt, ##__VA_ARGS__)
#else
#define LOGE(fmt, ...) ;
#endif

#elif WINDOWS
	#if LIBWIENGINE_EXPORTS
		#define WYLOGEXPORT __declspec(dllexport)
	#else
		#define WYLOGEXPORT __declspec(dllimport)
	#endif

	// this header may be included by c project, so extern "C"
	// may not be compiled in that project, need macro
	#ifdef __cplusplus
	extern "C" {
	#endif
	WYLOGEXPORT void __win_log_print(const char format[], ...);
	#ifdef __cplusplus
	}
	#endif

	#define LOGD(fmt, ...) __win_log_print(fmt, __VA_ARGS__)
	#define LOGW(fmt, ...) __win_log_print(fmt, __VA_ARGS__)
	#define LOGE(fmt, ...) __win_log_print(fmt, __VA_ARGS__)
#endif

#endif // __wyLog_h__
