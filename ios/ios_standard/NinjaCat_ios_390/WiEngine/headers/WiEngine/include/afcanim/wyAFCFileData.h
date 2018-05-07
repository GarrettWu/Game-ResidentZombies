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
#ifndef __wyAFCFileData_h__
#define __wyAFCFileData_h__

#include "wyObject.h"
#include "wyAFCClip.h"

/**
 * @class wyAFCFileData
 *
 * \if English
 * Base class for all file data object in AFC architecture, it contains
 * common fields for AFC file data
 * \else
 * 所有AFC动画文件数据的基类, 包含了所有AFC动画文件都应该具有的属性
 * \endif
 */
class WIENGINE_API wyAFCFileData : public wyObject {
public:
	/**
	 * \if English
	 * unique id for this file data
	 * \else
	 * data的一个唯一标识符，内部使用，用来缓冲
	 * \endif
	 */
	unsigned int m_dataId;

	/**
	 * \if English
	 * scale of file data
	 * \else
	 * 动画文件的缩放比例
	 * \endif
	 */
	float m_resScale;

protected:
	wyAFCFileData();

public:
	virtual ~wyAFCFileData();
};

#endif // __wyAFCFileData_h__
