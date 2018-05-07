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
#include "MyDotPageIndicator.h"
#include "wyTypes.h"
#include "wyUtils.h"

MyDotPageIndicator::MyDotPageIndicator(wyTexture2D* dot, wyRect dotRect, wyTexture2D* selectedDot, wyRect selectedDotRect) :
		m_pageCount(0),
		m_selectedIndex(-1),
		m_dotSpacing(DP(4)) {
	m_dot = dot;
	m_dotRect = dotRect;
	m_selectedDot = selectedDot;
	m_selectedDotRect = selectedDotRect;

	wyObjectRetain(m_dot);
	wyObjectRetain(m_selectedDot);
}

MyDotPageIndicator::~MyDotPageIndicator() {
	wyObjectRelease(m_dot);
	wyObjectRelease(m_selectedDot);
}

MyDotPageIndicator* MyDotPageIndicator::make(wyTexture2D* dot, wyTexture2D* selectedDot) {
	MyDotPageIndicator* i = WYNEW MyDotPageIndicator(dot,
			wyr(0, 0, dot->getWidth(), dot->getHeight()),
			selectedDot,
			wyr(0, 0, selectedDot->getWidth(), selectedDot->getHeight()));
	return (MyDotPageIndicator*)i->autoRelease();
}

MyDotPageIndicator* MyDotPageIndicator::make(wyTexture2D* dot, wyRect dotRect, wyTexture2D* selectedDot, wyRect selectedDotRect) {
	MyDotPageIndicator* i = WYNEW MyDotPageIndicator(dot, dotRect, selectedDot, selectedDotRect);
	return (MyDotPageIndicator*)i->autoRelease();
}

void MyDotPageIndicator::initWithPages(int page, int selectedIndex) {
	m_pageCount = page;
	m_selectedIndex = selectedIndex;
	updateContentSize();
}

void MyDotPageIndicator::onPageAdded(int index) {
	m_pageCount++;
	updateContentSize();
}

void MyDotPageIndicator::onPageRemoved(wyNode* page, int index) {
	m_pageCount--;
	updateContentSize();
}

void MyDotPageIndicator::onPageAllRemoved() {
	m_pageCount = 0;
	m_selectedIndex = -1;
	updateContentSize();
}

void MyDotPageIndicator::onPageChanged(int index) {
	m_selectedIndex = index;
}

void MyDotPageIndicator::updateContentSize() {
	if(m_pageCount <= 0)
		setContentSize(0, 0);
	else
		setContentSize(MAX(m_dotRect.width*this->getScaleX(), m_selectedDotRect.width*this->getScaleX()) * m_pageCount + m_dotSpacing * (m_pageCount - 1)*this->getScaleX(),
				MAX(m_dotRect.height*this->getScaleY(), m_selectedDotRect.height*this->getScaleY()));
}

void MyDotPageIndicator::draw() {
	// if no draw flag is set, call wyNode::draw and it
	// will decide forward drawing to java layer or not
	if(m_noDraw) {
		wyNode::draw();
		return;
	}

//	float dotW = MAX(m_dot->getWidth(), m_selectedDot->getWidth());
//	float dotH = MAX(m_dot->getHeight(), m_selectedDot->getHeight());
//	float x = dotW / 2;
//	float y = dotH / 2;
//	for(int i = 0; i < m_pageCount; i++) {
//		if(m_selectedIndex == i) {
//			m_selectedDot->draw(x - m_selectedDot->getWidth() / 2,
//					y - m_selectedDot->getHeight() / 2,
//					m_selectedDotRect.width,
//					m_selectedDotRect.height,
//					false,
//					false,
//					m_selectedDotRect);
//		} else {
//			m_dot->draw(x - m_dot->getWidth() / 2,
//					y - m_dot->getHeight() / 2,
//					m_dotRect.width,
//					m_dotRect.height,
//					false,
//					false,
//					m_dotRect);
//		}
//
//		x += dotW + m_dotSpacing;
//	}
    
    float dotW = MAX(m_dotRect.width*this->getScaleX(), m_selectedDotRect.width*this->getScaleX());
	float dotH = MAX(m_dotRect.height*this->getScaleY(), m_selectedDotRect.height*this->getScaleY());
	float x = dotW / 2;
	float y = dotH / 2;
	for(int i = 0; i < m_pageCount; i++) {
		if(m_selectedIndex == i) {
			m_selectedDot->draw(x - m_selectedDotRect.width / 2*this->getScaleX(),
                                y - m_selectedDotRect.height / 2*this->getScaleY(),
                                m_selectedDotRect.width*this->getScaleX(),
                                m_selectedDotRect.height*this->getScaleY(),
                                false,
                                false,
                                m_selectedDotRect);
		} else {
			m_dot->draw(x - m_dotRect.width / 2*this->getScaleX(),
                        y - m_dotRect.height / 2*this->getScaleY(),
                        m_dotRect.width*this->getScaleX(),
                        m_dotRect.height*this->getScaleY(),
                        false,
                        false,
                        m_dotRect);
		}
        
		x += dotW + m_dotSpacing*this->getScaleX();
	}
}
