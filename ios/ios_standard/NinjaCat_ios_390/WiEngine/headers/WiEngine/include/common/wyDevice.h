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
#ifndef __wyDevice_h__
#define __wyDevice_h__

#include "wyGlobal.h"

/**
 * \if English
 * low resolution, 120 dpi
 * \else
 * 低分辨率, 相当于120dpi
 * \endif
 */
#define DENSITY_LDPI 0.75f

/**
 * \if English
 * medium resolution, 160 dpi
 * \else
 * 中分辨率, 相当于160dpi
 * \endif
 */
#define DENSITY_MDPI 1.f

/**
 * \if English
 * high resolution, 240 dpi
 * \else
 * 高分辨率，相当于240dpi
 * \endif
 */
#define DENSITY_HDPI 1.5f

/**
 * @typedef wyScaleMode
 *
 * \if English
 * Screen adaption mode constant
 * \else
 * 屏幕适配模式常量
 * \endif
 */
typedef enum {
	/**
	 * \if English
	 * Scale resource based on resource's density and screen's density. This is the
	 * default mode.
	 * \else
	 * 按照资源的density和屏幕的density自动进行缩放，这是缺省的适配模式. 这个模式的
	 * 优点是可以自动找到合适的资源，可以为不同的屏幕指定不同的资源，从而保证游戏在不同
	 * 屏幕上的画面品质。缺点是需要开发者自己处理坐标的转换，以及编程时要注意使用DP值和逻辑
	 * 的严谨
	 * \endif
	 */
	SCALE_MODE_BY_DENSITY,

	/**
	 * \if English
	 * \par
	 * Set a base size for game and engine will follow this size anywhere. A
	 * small base size will drop down image quality in large screen, and a large base
	 * size may consume more memory. So you should choose a best base size to fit as
	 * many screens as possible, meanwhile gaining acceptable image quality.
	 *
	 * \par
	 * Once base size mode is used, WiEngine will not auto scale images
	 * \else
	 * \par
	 * 设定一个基础大小，然后所有的大小都以这个基础大小为参考, 所有的坐标也以这个为参考，比如
	 * 触摸的位置也会以这个基础大小为标准转换。如果屏幕的大小和基础大小不符合，则会在xy方向拉伸
	 * 图像直至填充满. 基础大小适配方式的优点是简单，可以自动适配所有屏幕尺寸, 但是你要选择一个
	 * 合适的基础大小以便能适配尽可能多的设备, 并且保持可以接受的画面质量.
	 *
	 * \par
	 * 一旦选择了基础大小适配模式，WiEngine将不再自动缩放贴图
	 * \endif
	 */
	SCALE_MODE_BASE_SIZE_FIT_XY
} wyScaleMode;

/**
 * @class wyDevice
 *
 * \if English
 * Abstraction interface for a mobile equipment, it provides device level
 * API for upper layer
 * \else
 * 对一个移动设备的抽象接口, 为上层提供设备级的接口, 不同的平台需要继承以实现特定设备的接口
 * \endif
 */
class WIENGINE_API wyDevice {
public:
	/**
	 * \if English
	 * Screen adaption mode
	 * \else
	 * 屏幕适配模式
	 * \endif
	 */
	static wyScaleMode scaleMode;

	/**
	 * \if English
	 * Density of current screen
	 * \else
	 * 当前设备的屏幕分辨率
	 * \endif
	 */
	static float density;

	/**
	 * \if English
	 * Font ensity of current screen
	 * \else
	 * 当前设备屏幕的字体分辨率
	 * \endif
	 */
	static float scaledDensity;

	/**
	 * \if English
	 * Default input density of resources and it is only used when resource
	 * density can't be determined.
	 * \else
	 * 图片的缺省分辨率，这个参数会影响到所有图片的解析。
	 * 如果这个参数和density相同，那么图片不会被缩放。
	 * 如果density是1.5，而这个值是1，那么图片将会被放大到1.5倍
	 * \endif
	 */
	static float defaultInDensity;

	/**
	 * \if English
	 * System api level. For android, it is the number defined in BUILD.VERSION.SDK_INT.
	 * For iOS, the system version string "x.y" is converted in formula x * 100 + y
	 * \else
	 * 对于Android, 这是系统api版本，3表示1.5, 4表示1.6，等等
	 * 对于iOS系统, 它并没有一个表示系统版本的整数值, 但是有字符串值, 因此这里对iOS做一个
	 * 映射, iOS系统版本字符串是一个"x.y"的格式, 通过公式x * 100 + y转换为整数, 即403对
	 * 应与iOS版本4.3
	 * \endif
	 */
	static int apiLevel;

	/**
	 * \if English
	 * Max texture size supported by this device
	 * \else
	 * OpenGL支持的最大贴图尺寸
	 * \endif
	 */
	static int maxTextureSize;

	/**
	 * \if English
	 * base width, only used in base size mode
	 * \else
	 * 基础宽度，仅用于基础大小适配模式
	 * \endif
	 */
	static int baseWidth;

	/**
	 * \if English
	 * base height, only used in base size mode
	 * \else
	 * 基础高度，仅用于基础大小适配模式
	 * \endif
	 */
	static int baseHeight;

	/**
	 * \if English
	 * the scale rate of base width and screen width
	 * \else
	 * 基础宽度和屏幕宽度的比例
	 * \endif
	 */
	static float baseScaleX;

	/**
	 * \if English
	 * the scale rate of base height and screen height
	 * \else
	 * 基础高度和屏幕宽度的比例
	 * \endif
	 */
	static float baseScaleY;

	/**
	 * \if English
	 * \par
	 * Current window width in pixels
	 *
	 * \par
	 * If current mode is density mode, then it is width of surface view. However,
	 * surface view is not known until it is created so this field will be initialized
	 * as screen width.
	 *
	 * \par
	 * If current mode is base size mode, then it is base width user set
	 * \else
	 * \par
	 * 得到当前窗口宽度
	 *
	 * \par
	 * 如果当前适配模式是密度适配，则这个宽度是surface view的宽度，但是要注意，
	 * surface view的大小要在surface被创建时才能知道，因此这个大小在一开始会被初始
	 * 化成屏幕大小。如果最终的surface大小和屏幕大小不匹配，则在得到surface正确大小
	 * 之前创建的场景都可能出现坐标的偏差。因此在最新的demo中，已经把创建第一个场景挪
	 * 到了surface创建之后进行
	 *
	 * \par
	 * 如果当前模式是基础大小适配，则这个宽度是用户设置的基础宽度
	 * \endif
	 */
	static int winWidth;

	/**
	 * \if English
	 * \par
	 * Current window height in pixels
	 *
	 * \par
	 * If current mode is density mode, then it is height of surface view. However,
	 * surface view is not known until it is created so this field will be initialized
	 * as screen height.
	 *
	 * \par
	 * If current mode is base size mode, then it is base height user set
	 * \else
	 * \par
	 * 得到当前窗口高度
	 *
	 * \par
	 * 如果当前适配模式是密度适配，则这个高度是surface view的高度，但是要注意，
	 * surface view的大小要在surface被创建时才能知道，因此这个大小在一开始会被初始
	 * 化成屏幕大小。如果最终的surface大小和屏幕大小不匹配，则在得到surface正确大小
	 * 之前创建的场景都可能出现坐标的偏差。因此在最新的demo中，已经把创建第一个场景挪
	 * 到了surface创建之后进行
	 *
	 * \par
	 * 如果当前模式是基础大小适配，则这个高度是用户设置的基础高度
	 * \endif
	 */
	static int winHeight;

	/**
	 * \if English
	 * The real height of surface view
	 * \else
	 * 真实surface宽度
	 * \endif
	 */
	static int realWidth;

	/**
	 * \if English
	 * The real height of surface view
	 * \else
	 * 真实surface高度
	 * \endif
	 */
	static int realHeight;
	
public:
	/**
	 * \if English
	 * Get ISO 639-1 language code of current system language
	 *
	 * @return ISO 639-1 language code, caller should not release returned string
	 * \else
	 * 得到系统当前语言的ISO 639-1 2字符语言码
	 *
	 * @return ISO 639-1语言码, 调用者不需要释放返回的字符串
	 * \endif
	 */
	static const char* getLanguage();

	/**
	 * \if English
	 * Get ISO 3166-1 country code of current system country
	 *
	 * @return ISO 3166-1 country code, caller should not release returned string
	 * \else
	 * 得到系统当前的ISO 3166-1 2字符国家码
	 *
	 * @return ISO 3166-1国家码, 调用者不需要释放返回的字符串
	 * \endif
	 */
	static const char* getCountry();

	/**
	 * \if English
	 * Get an unique identifier which can represent this device
	 *
	 * \note
	 * in Android platform, you need add READ_PHONE_STATE and ACCESS_WIFI_STATE permission
	 * to make this method works
	 *
	 * @return unique identifier, caller should not release it. Returns NULL if can't get
	 * \else
	 * 返回一个能代表该设备的唯一标识符
	 *
	 * \note
	 * 在Android平台上, 你需要添加READ_PHONE_STATE和ACCESS_WIFI_STATE权限才能获得UUID
	 *
	 * @return 能代表该设备的唯一标识符, 调用者不需要释放返回的字符串. 如果得不到, 返回NULL
	 * \endif
	 */
	static const char* getDeviceId();

	/**
	 * \if English
	 * Check whether this device has external storage available. If external
	 * storage is ready, then path likes "/sdcard/abc.png" should works.
	 *
	 * \note
	 * For iOS and Mac OS X, this always returns true
	 *
	 * @return true means external storage is available, false means not
	 * \else
	 * 检查设备的外部存储是否可用. 可用意味着类似于/sdcard/abc.png这样的资源路径应该是
	 * 可以访问的
	 *
	 * \note
	 * 对于iOS和Mac OS X, 这个方法总是返回true
	 *
	 * @return true表示外部存储当前可用
	 * \endif
	 */
	static bool hasExternalStorage();
};

#endif // __wyDevice_h__
