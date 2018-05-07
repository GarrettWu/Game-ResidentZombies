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
#ifndef __wyCrossFadeTransition_h__
#define __wyCrossFadeTransition_h__

#include "wyTransitionScene.h"
#include "wyRenderTexture.h"

/**
 * @class wyCrossFadeTransition
 *
 * 旧场景从不透明变成透明，新场景从透明变成不透明
 */
class WIENGINE_API wyCrossFadeTransition : public wyTransitionScene {
protected:
	/// 新场景贴图\link wyRenderTexture wyRenderTexture对象指针\endlink
	wyRenderTexture* m_inTexture;

	/// 旧场景贴图\link wyRenderTexture wyRenderTexture对象指针\endlink
	wyRenderTexture* m_outTexture;

	/// @see wyTransitionScene::postFinish
	virtual bool shouldInSceneOnTop();

	/// @see wyTransitionScene::postFinish
	virtual void postFinish();

	/// @see wyTransitionScene::initScenes
	virtual void initScenes();

	/// @see wyTransitionScene::getInActionTarget
	virtual wyNode* getInActionTarget();

	/// @see wyTransitionScene::getOutActionTarget
	virtual wyNode* getOutActionTarget();

	/// @see wyTransitionScene::getInAction
	virtual wyIntervalAction* getInAction();

	/// @see wyTransitionScene::getOutAction
	virtual wyIntervalAction* getOutAction();

public:
	/**
	 * 静态构造函数
	 *
	 * @param duration 场景切换的时间
	 * @param inScene 准备载入的\link wyScene wyScene对象指针\endlink. 如果不为NULL, 则
	 * 		载出场景自动设置为当前场景. 如果为NULL, 则载入场景和载出场景都不设置. 需要在之后再
	 * 		自行设置. 一般如果是想在popScene的时候, 需要把这个参数设置为NULL, 这样\link wyDirector wyDirector\endlink
	 * 		会自动设置载入和载出场景.
	 */
	static wyCrossFadeTransition* make(float duration, wyScene* inScene);

	/**
	 * 构造函数
	 *
	 * @param duration 场景切换的时间
	 * @param inScene 准备载入的\link wyScene wyScene对象指针\endlink. 如果不为NULL, 则
	 * 		载出场景自动设置为当前场景. 如果为NULL, 则载入场景和载出场景都不设置. 需要在之后再
	 * 		自行设置. 一般如果是想在popScene的时候, 需要把这个参数设置为NULL, 这样\link wyDirector wyDirector\endlink
	 * 		会自动设置载入和载出场景.
	 */
	wyCrossFadeTransition(float duration, wyScene* inScene);

	/**
	 * 析构函数
	 */
	virtual ~wyCrossFadeTransition();
};

#endif // __wyCrossFadeTransition_h__
