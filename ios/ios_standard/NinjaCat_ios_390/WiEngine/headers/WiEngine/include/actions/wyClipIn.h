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
#ifndef __wyClipIn_h__
#define __wyClipIn_h__

#include "wyIntervalAction.h"

/**
 * @class wyClipIn
 *
 * \if English
 * Dynamic change clip rect of node to get an enter animation
 * \else
 * 动态的改变节点的裁剪区域从而得到一个逐渐显示的效果, 类似于愤怒小鸟中的菜单出现效果
 * \endif
 */
class WIENGINE_API wyClipIn : public wyIntervalAction {
public:
	/**
	 * \if English
	 * Direction to which clip rect moves
	 * \else
	 * 裁剪区域渐变的方向
	 * \endif
	 */
	enum Direction {
		/**
		 * \if English
		 * clip grows from left to right side
		 * \else
		 * 从左边开始显示到右边
		 * \endif
		 */
		LEFT_TO_RIGHT,

		/**
		 * \if English
		 * clip grows from right to left side
		 * \else
		 * 从右边开始显示到左边
		 * \endif
		 */
		RIGHT_TO_LEFT,

		/**
		 * \if English
		 * clip grows from top to bottom side
		 * \else
		 * 从上边开始显示到下边
		 * \endif
		 */
		TOP_TO_BOTTOM,

		/**
		 * \if English
		 * clip grows from bottom to top side
		 * \else
		 * 从下边开始显示到上边
		 * \endif
		 */
		BOTTOM_TO_TOP
	};

private:
	/// clip grow direction
	Direction m_direction;

protected:
	/**
	 * Constructor
	 *
	 * @param duration action duration in seconds
	 * @param direction clip grow direction
	 */
	wyClipIn(float duration, Direction direction);

public:
	virtual ~wyClipIn();

	/**
	 * \if English
	 * Create a clip in action
	 *
	 * @param duration action duration in seconds
	 * @param direction clip grow direction, default is \c BOTTOM_TO_TOP
	 * @return \link wyClipIn wyClipIn\endlink
	 * \else
	 * 创建一个clip in动作
	 *
	 * @param duration 动作支持时间, 单位秒
	 * @param direction 裁剪区域变化的方向, 缺省是从下到上
	 * @return \link wyClipIn wyClipIn\endlink
	 * \endif
	 */
	static wyClipIn* make(float duration, Direction direction = BOTTOM_TO_TOP);

	/// @see wyAction::copy
	virtual wyAction* copy();

	/// @see wyAction::reverse
	virtual wyAction* reverse();

	/// @see wyAction::update
	virtual void update(float t);

	/// @see wyAction::start
	virtual void start(wyNode* target);

	/// @see wyAction::stop
	virtual void stop();
};

#endif // __wyClipIn_h__
