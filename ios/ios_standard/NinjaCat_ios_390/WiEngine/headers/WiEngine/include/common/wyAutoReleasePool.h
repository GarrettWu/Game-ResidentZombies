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
#ifndef __wyAutoReleasePool_h__
#define __wyAutoReleasePool_h__

#include "wyObject.h"

class wyDirector;

/**
 * @class wyAutoReleasePool
 *
 * \if English
 * pool of objects which need to be auto released. Maintained for
 * every thread so that objects got released when thread exits
 * \else
 * 自动释放对象池. 每个创建的线程都需要一个这样的池用来保存需要释放的对象, 在
 * 线程结束时释放它们
 * \endif
 */
class WIENGINE_API wyAutoReleasePool : public wyObject {
	friend class wyObject;
	friend class wyDirector;

private:
	/// thread id of this autorelease pool
	ssize_t m_thread;

	/// objects need to be released
	vector<wyObject*>* m_objects;

protected:
	wyAutoReleasePool();

	/// add an object to autorelease pool of current thread
	static void addToPool(wyObject* obj);

	/// flush all pending object, release them now
	static void flush();

public:
	virtual ~wyAutoReleasePool();
	static wyAutoReleasePool* make();

	/**
	 * \if English
	 * Drain autorelease pool of current thread
	 * \else
	 * 抽干当前线程的自动释放池
	 * \endif
	 */
	static void drainCurrent();

	/**
	 * \if English
	 * Drain pool to release all objects in it
	 * \else
	 * 抽干自动释放池, 从而里面的所有对象都得到释放
	 * \endif
	 */
	void drain();
};

#endif // __wyAutoReleasePool_h__
