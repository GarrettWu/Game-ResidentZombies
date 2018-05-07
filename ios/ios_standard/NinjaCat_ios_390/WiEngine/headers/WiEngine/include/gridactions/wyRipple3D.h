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
#ifndef __wyRipple3D_h__
#define __wyRipple3D_h__

#include "wyGrid3DAction.h"
#include "wyTypes.h"

/**
 * @class wyRipple3D
 *
 * \if English
 * Node performing this action will produce the ripple effect.
 * \else
 * 波纹动作。目标会呈现类似于水面被投入石子的效果
 * \endif
 */
class WIENGINE_API wyRipple3D : public wyGrid3DAction {
protected:
	/**
	 * \if English
	 * x coordinate of the ripple center
	 * \else
	 * 波动中心位置x值
	 * \endif
	 */
	float m_centerX;

	/**
	 * \if English
	 * y coordinate of the ripple center
	 * \else
	 * 波动中心位置值
	 * \endif
	 */
	float m_centerY;

	/**
	 * \if English
	 * tuning of the ripple center on the x direction
	 * \else
	 * 波动中心横向变化值
	 * \endif
	 */
	float m_deltaX;

	/**
	 * \if English
	 * tuning of the ripple center on the y direction
	 * \else
	 * 波动中心中心变化值
	 * \endif
	 */
	float m_deltaY;

	/**
	 * \if English
	 * radius of the ripple
	 * \else
	 * 波动波及范围的半径
	 * \endif
	 */
	float m_radius;

	/**
	 * \if English
	 * tuning of the ripple radius
	 * \else
	 * 波动半径变化值
	 * \endif
	 */
	float m_deltaRadius;

	/**
	 * \if English
	 * wave count
	 * \else
	 * 波数
	 * \endif
	 */
	float m_waves;

	/**
	 * \if English
	 * tuning of the amplitude
	 * \else
	 * 幅度变化值
	 * \endif
	 */
	float m_deltaAmplitude;

	// last values
	float m_lastX;
	float m_lastY;
	float m_lastRadius;
	float m_lastAmplitude;

public:
	/**
	 * \if English
	 * factory function, used to create a new instance with autoRelease enabled
	 *
	 * @param duration time, in seconds
	 * @param gridX row count
	 * @param gridY column count
	 * @param centerX x coordinate of the ripple center
	 * @param centerY y coordinate of the ripple center
	 * @param deltaX tuning of the ripple center on the x direction
	 * @param deltaY tuning of the ripple center on the y direction
	 * @param radius radius of the ripple
	 * @param deltaRadius tuning of radius
	 * @param amplitude amplitude
	 * @param deltaAmplitude tuning of amplitude
	 * @param waves wave count
	 * \else
	 * 静态构造函数
	 *
	 * @param duration 动作持续时间
	 * @param gridX 网格行数
	 * @param gridY 网格列数
	 * @param centerX 波动中心位置x值
	 * @param centerY 波动中心位置值
	 * @param deltaX 波动中心横向变化值
	 * @param deltaY 波动中心中心变化值
	 * @param radius 波动波及范围的半径
	 * @param deltaRadius 波动波及范围的半径变化值
	 * @param amplitude 幅度
	 * @param deltaAmplitude 幅度变化值
	 * @param waves 波数
	 * \endif
	 */
	static wyRipple3D* make(float duration, int gridX, int gridY, float centerX, float centerY,
			float deltaX, float deltaY, float radius, float deltaRadius, float amplitude, float deltaAmplitude, float waves);

	/**
	 * \if English
	 * constructor
	 *
	 * @param duration time, in seconds
	 * @param gridX row count
	 * @param gridY column count
	 * @param centerX x coordinate of the ripple center
	 * @param centerY y coordinate of the ripple center
	 * @param deltaX tuning of the ripple center on the x direction
	 * @param deltaY tuning of the ripple center on the y direction
	 * @param radius radius of the ripple
	 * @param deltaRadius tuning of radius
	 * @param amplitude amplitude
	 * @param deltaAmplitude tuning of amplitude
	 * @param waves wave count
	 * \else
	 * 构造函数
	 *
	 * @param duration 动作持续时间
	 * @param gridX 网格行数
	 * @param gridY 网格列数
	 * @param centerX 波动中心位置x值
	 * @param centerY 波动中心位置值
	 * @param deltaX 波动中心横向变化值
	 * @param deltaY 波动中心中心变化值
	 * @param radius 波动波及范围的半径
	 * @param deltaRadius 波动波及范围的半径变化值
	 * @param amplitude 幅度
	 * @param deltaAmplitude 幅度变化值
	 * @param waves 波数
	 * \endif
	 */
	wyRipple3D(float duration, int gridX, int gridY, float centerX, float centerY,
			float deltaX, float deltaY, float radius, float deltaRadius, float amplitude, float deltaAmplitude, float waves);

	/**
	 * \if Englsih
	 * destructor
	 * \else
	 * 析构函数
	 * \endif
	 */
	virtual ~wyRipple3D();

	/// @see wyAction::copy
	virtual wyAction* copy();

	/// @see wyAction::update
	virtual void update(float t);
};

#endif // __wyRipple3D_h__
