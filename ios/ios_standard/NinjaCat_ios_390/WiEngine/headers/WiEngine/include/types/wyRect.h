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
#ifndef __wyRect_h__
#define __wyRect_h__

#include <stdio.h>
#include "wyTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

WIENGINE_API extern const wyRect wyrZero;

static inline wyRect wyr(float x, float y, float w, float h) {
	wyRect r = { x, y, w, h };
	return r;
}

/**
 * \if English
 * Parse a rect from a string. The format of string must be {{x,y},{w,h}}, for example: "{{3.2,3.4},{10,11}}"
 *
 * @param s string
 * @return parsed rect
 * \else
 * 把一个字符串转换为wyRect结构, 字符串的格式必须是{{x,y},{w,h}}, 例如"{{3.2,3.4},{10,11}}"
 *
 * @param s 字符串形式
 * @return \link wyRect wyRect\endlink 结构
 * \endif
 */
WIENGINE_API wyRect wyrFromString(const char* s);

/**
 * \if English
 * Is two rectangle same?
 *
 * @param r1 rectangle one
 * @param r2 rectangle two
 * @return true if they are same
 * \else
 * 判断两个矩形是否相等
 *
 * @param r1 矩形1
 * @param r2 矩形2
 * @return true表示相等
 * \endif
 */
static inline bool wyrEquals(wyRect& r1, wyRect& r2) {
	return r1.x == r2.x &&
			r1.y == r2.y &&
			r1.width == r2.width &&
			r1.height == r2.height;
}

/**
 * \if English
 * Is a point inside a rectangle?
 *
 * @param r1 rectangle
 * @param p point
 * @return true if p is located in rectangle
 * \else
 * 判断某个点是否在矩形内
 *
 * @param r1 矩形
 * @param p 点
 * @return true表示点在矩形内
 * \endif
 */
static inline bool wyrContains(wyRect& r1, wyPoint p) {
	return p.x >= r1.x &&
		p.x < r1.x + r1.width &&
		p.y >= r1.y &&
		p.y < r1.y + r1.height;
}

/**
 * \if English
 * Check two rectangle is intersected or not
 *
 * @param r1 rectangle one
 * @param r2 rectangle two
 * @return true if they are intersected
 * \else
 * 检查两个矩形是否相交
 *
 * @param r1 矩形1
 * @param r2 矩形2
 * @return 如果两个矩形相交则返回true
 * \endif
 */
static inline bool wyrIsIntersect(wyRect& r1, wyRect& r2) {
	return !(r1.x > r2.x + r2.width ||
		r1.x + r1.width < r2.x ||
		r1.y > r2.y + r2.height ||
		r1.y + r1.height < r2.y);
}

/**
 * \if English
 * Intersect two rectangles
 *
 * @param r1 rectangle one, its (x, y) must be left bottom corner
 * @param r2 rectangle two, its (x, y) must be left bottom corner
 * @return intersected rectangle
 * \else
 * 取两个矩形的相交矩形
 *
 * @param r1 矩形1, 它的(x, y)位置必须是左下角
 * @param r2 矩形2, 它的(x, y)位置必须是左下角
 * @return 两个矩形的相交矩形, (x, y)位置是左下角
 * \endif
 */
static inline wyRect wyrIntersect(wyRect r1, wyRect r2) {
	float left = MAX(r1.x, r2.x);
	float right = MIN(r1.x + r1.width, r2.x + r2.width);
	float bottom = MAX(r1.y, r2.y);
	float top = MIN(r1.y + r1.height, r2.y + r2.height);
	float width = right - left;
	float height = top - bottom;
	if(width <= 0 || height <= 0)
		return wyrZero;
	else
		return wyr(left, bottom, width, height);
}

/**
 * \if English
 * Combine two rectangles
 *
 * @param r1 rectangle one, its (x, y) must be left bottom corner
 * @param r2 rectangle two, its (x, y) must be left bottom corner
 * @return combined rectangle
 * \else
 * 合并两个矩形, 得到包含两个矩形的最小矩形
 *
 * @param r1 矩形1, 它的(x, y)位置必须是左下角
 * @param r2 矩形2, 它的(x, y)位置必须是左下角
 * @return 包含两个矩形的最小矩形, (x, y)位置是左下角
 * \endif
 */
static inline wyRect wyrCombine(wyRect r1, wyRect r2) {
	float left = MIN(r1.x, r2.x);
	float right = MAX(r1.x + r1.width, r2.x + r2.width);
	float bottom = MIN(r1.y, r2.y);
	float top = MAX(r1.y + r1.height, r2.y + r2.height);
	return wyr(left, bottom, right - left, top - bottom);
}

#ifdef __cplusplus
}
#endif

#endif // __wyRect_h__
