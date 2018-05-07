/*
 * Copyright (c) 2010 WiYun Inc.
 * Author: luma(stubma@gmail.com)
 *
 * For all entities this program is free software; you can redistribute
 * it and/or modify it under the terms of the 'WiEngine' license with
 * the additional provision that 'WiEngine' must be credited in a manner
 * that can be be observed by end users, for example, in the credits or during
 * start up. (please find WiEngine logo in sdk's logo folder)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __wyAFCConstants_h__
#define __wyAFCConstants_h__

/**
 * @typedef wyAFCClipType
 *
 * \if English
 * Type of clip
 * \else
 * 分片的类型
 * \endif
 */
typedef enum {
	/**
	 * \if English
	 * clip is a image
	 * \else
	 * 分片是一个图片
	 * \endif
	 */
	AFC_CLIP_IMAGE,

	/**
	 * \if English
	 * clip is a ellipse
	 * \else
	 * 分片是一个椭圆
	 * \endif
	 */
	AFC_CLIP_ELLIPSE,

	/**
	 * \if English
	 * clip is a line
	 * \else
	 * 分片是一条线
	 * \endif
	 */
	AFC_CLIP_LINE,

	/**
	 * \if English
	 * clip is a rectangle
	 * \else
	 * 分片是一个矩形
	 * \endif
	 */
	AFC_CLIP_RECT,

	/**
	 * \if English
	 * clip is a round corner rectangle
	 * \else
	 * 分片是一个圆角矩形
	 * \endif
	 */
	AFC_CLIP_ROUNDRECT,

	/**
	 * \if English
	 * clip is a collision rect. Collision rectangle only has size property, it
	 * can't have a fill color, etc.
	 * \else
	 * 分片是一个碰撞矩形, 碰撞矩形和矩形的区别是: 碰撞矩形只是一个大小, 没有填充色等信息
	 * \endif
	 */
	AFC_CLIP_COLLISION_RECT,

	/**
	 * \if English
	 * clip is a reference point
	 * \else
	 * 分片是一个点
	 * \endif
	 */
	AFC_CLIP_POINT,

	/**
	 * \if English
	 * clip is a triangle
	 * \else
	 * 分片是一个三角形
	 * \endif
	 */
	AFC_CLIP_TRIANGLE
} wyAFCClipType;

#endif // __wyAFCConstants_h__
