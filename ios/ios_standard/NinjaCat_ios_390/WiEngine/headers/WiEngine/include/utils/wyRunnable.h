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
#ifndef __wyRunnable_h__
#define __wyRunnable_h__

#include "wyObject.h"

/**
 * @class wyRunnable
 *
 * \if English
 * it encapsulates some logic and can be delivered to other class to process it.
 * This class is a virtual base class and subclass must implmement run methods
 * \else
 * 类似于java中的Runnable类, \link wyRunnable wyRunnable\endlink 封装了一段逻辑, 这段
 * 逻辑可以交给其它任何类执行, 只要调用run方法即可. 该类是虚类, 子类必须实现run方法.
 * \endif
 */
class WIENGINE_API wyRunnable : public wyObject {
public:
	wyRunnable();
	virtual ~wyRunnable();

	/**
	 * \if English
	 * execute
	 * \else
	 * 执行runnable
	 * \endif
	 */
	virtual void run() = 0;
};

#endif // __wyRunnable_h__
