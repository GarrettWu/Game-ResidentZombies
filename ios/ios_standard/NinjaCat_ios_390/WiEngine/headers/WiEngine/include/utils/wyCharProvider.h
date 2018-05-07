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
#ifndef __wyCharProvider_h__
#define __wyCharProvider_h__

#include "wyGlobal.h"

/**
 * @class wyCharProvider
 *
 * \if English
 * An interface defines method to query character infomation
 * \else
 * 定义获取字符信息的接口
 * \endif
 */
class WIENGINE_API wyCharProvider {
public:
	/**
	 * \if English
	 * Measure string width for specified character range. No need consider about
	 * new line character.
	 *
	 * @param chars string in utf-8 encoding
	 * @param length byte length to be measured
	 * @param spaceWidth space character width in pixels
	 * @param tabSize space count which one tab equals to
	 * @return width of sub string in specified range
	 * \else
	 * 计算指定范围的字符构成的字符串长度, 不需要考虑换行符的情况
	 *
	 * @param chars utf-8编码的字符串
	 * @param length 需要被计算的字节长度
	 * @param spaceWidth 如果不存在空格字符, 则为空格填充的像素宽度
	 * @param tabSize 一个tab相当于多少个空格
	 * @return 范围内的子串长度
	 * \endif
	 */
	virtual float measureText(const char* chars, int length, float spaceWidth, float tabSize) = 0;
};

#endif // __wyCharProvider_h__
