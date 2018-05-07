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
#include "ArmsPageControl.h"
#include "wyGlobal.h"
#include "wyLog.h"
#include "wyTypes.h"
#include "wyUtils.h"
#include "wyEventDispatcher.h"

ArmsPageControl::ArmsPageControl() :
m_pages(wyArrayNew(5)),
m_container(NULL),
m_lastX(0),
m_lastY(0),
m_scroller(WYNEW wyScroller()),
m_flinging(false),
m_scrolling(false),
m_vertical(false),
m_initialPageIndex(-1),
m_indicator(NULL),
m_data(NULL),
m_centerY(-1),
m_centerX(-1) {
	// set page control properties
	setRelativeAnchorPoint(false);
	setContentSize(wyDevice::winWidth, wyDevice::winHeight);
	memset(&m_callback, 0, sizeof(ArmsPageControlCallback));
    
	// create container
	m_container = WYNEW wyLayer();
	m_container->setPosition(0, 0);
	addChildLocked(m_container);
	m_container->release();
    
	// enable events
	setTouchEnabled(true);
    
	// schedule fling timer
	wyTargetSelector* ts = wyTargetSelector::make(this, SEL(ArmsPageControl::updateFling));
	wyTimer* timer = wyTimer::make(ts);
	scheduleLocked(timer);
}

ArmsPageControl::~ArmsPageControl() {    
	m_scroller->release();
	wyArrayEach(m_pages, releasePage, NULL);
	wyArrayDestroy(m_pages);
}

ArmsPageControl* ArmsPageControl::make() {
	ArmsPageControl* n = WYNEW ArmsPageControl();
	return (ArmsPageControl*)n->autoRelease();
}

bool ArmsPageControl::releasePage(wyArray* arr, void* ptr, int index, void* data) {
	wyObjectRelease((wyObject*)ptr);
	return true;
}

void ArmsPageControl::notifyOnPagePositionChanged() {
	if(!m_callback.onPagePositionChanged
       )
		return;
    
	if(m_vertical) {
		for(int i = 0; i < m_pages->num; i++) {
			wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
			if(m_callback.onPagePositionChanged != NULL) {
				m_callback.onPagePositionChanged(this, page,
                                                 m_container->getPositionY() + page->getOriginY() + page->getHeight()*page->getScaleY() / 2 - m_height / 2, m_data);
			}
		}
	} else {
		for(int i = 0; i < m_pages->num; i++) {
			wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
			if(m_callback.onPagePositionChanged != NULL) {
				m_callback.onPagePositionChanged(this, page,
                                                 m_container->getPositionX() + page->getPositionX() - m_width / 2, m_data);
//                LOGE("%f, %f, %f, %f:", m_container->getPositionX(), page->getPositionX(), page->getWidth()*page->getScaleX() / 2, m_width);
			}
		}
	}
}

void ArmsPageControl::updateFling(wyTargetSelector* ts) {
	if(m_flinging) {
		if(m_scroller->computeScrollOffset()) {
			if(m_vertical) {
				m_container->setPosition(m_container->getPositionX(), m_scroller->getCurrY());
			} else {
				m_container->setPosition(m_scroller->getCurrX(), m_container->getPositionY());
			}
            
			// notify page position changed
			notifyOnPagePositionChanged();
            
			// update page indicator
			if(m_indicator) {
				int best = getBestIndex();
				m_indicator->onPageChanged(best);
			}
		} else {
			m_flinging = false;
            
			// callback
			notifyOnPageChanged();
		}
	}
}

void ArmsPageControl::notifyOnPageChanged() {
	// notify indicator
	if(m_indicator)
		m_indicator->onPageChanged(getBestIndex());
    
	if(m_callback.onPageChanged != NULL) {
		m_callback.onPageChanged(this, getBestIndex(), m_data);
	}
}

void ArmsPageControl::notifyOnPageClicked(int index) {
	if(m_callback.onPageClicked != NULL) {
		m_callback.onPageClicked(this, index, m_data);
	}
}

void ArmsPageControl::setInitialPage(int index) {
	if(index < 0 || index >= m_pages->num)
		return;
    
	m_initialPageIndex = index;
	float pos = m_vertical ? (m_height / 2 - getPageCenterY(index)) : (m_width / 2 - getPageCenterX(index));
	if(m_vertical)
		m_container->setPosition(0, pos);
	else
		m_container->setPosition(pos, 0);
    
	// notify page position changed
	notifyOnPagePositionChanged();
}

void ArmsPageControl::gotoPage(int index, bool animate, float duration) {
	if(animate) {
		m_flinging = true;
		float pos = m_vertical ? (m_height / 2 - getPageCenterY(index)) : (m_width / 2 - getPageCenterX(index));
		float start = m_vertical ? m_container->getPositionY() : m_container->getPositionX();
		if(m_vertical)
			m_scroller->startScroll(0.0f, start, 0.0f, pos - start, duration * 1000);
		else
			m_scroller->startScroll(start, 0.0f, pos - start, 0.0f, duration * 1000);
	} else {
		setInitialPage(index);
        
		// ensure not fling now, if it is, container will be reset in updateFling
		m_flinging = false;
        
		// notify callback
		notifyOnPageChanged();
	}
}

float ArmsPageControl::getPageCenterX(int index) {
	if(index < 0)
		return m_width / 2;
	else if(index >= m_pages->num)
		index = m_pages->num - 1;
    
	float center = m_width / 2;
	for(int i = 0; i <= index; i++) {
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
        
		// add this page offset
		if(i > 0) {
			center += page->getWidth()*page->getScaleX() / 2;
			center += m_pageSpacing;
		}
        
		// get over this page
		if(i < index)
			center += page->getWidth()*page->getScaleX() / 2;
	}
	return center;
}

float ArmsPageControl::getPageCenterY(int index) {
	if(index < 0)
		return m_height / 2;
	else if(index >= m_pages->num)
		index = m_pages->num - 1;
    
	float center = m_height / 2;
	for(int i = 0; i <= index; i++) {
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
        
		// add this page offset
		if(i > 0) {
			center += page->getHeight()*page->getScaleY() / 2;
			center += m_pageSpacing;
		}
        
		// get over this page
		if(i < index)
			center += page->getHeight()*page->getScaleY() / 2;
	}
	return center;
}

void ArmsPageControl::updatePagePositions() {
	float center = m_vertical ? (m_height / 2) : (m_width / 2);
//    LOGE("width:%f, height:%f", m_width, m_height);
	for(int i = 0; i < m_pages->num; i++) {
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
        
		// add this page offset
		if(i > 0) {
			center += m_vertical ? (page->getHeight()*page->getScaleY() / 2) : (page->getWidth()*page->getScaleX() / 2);
			center += m_pageSpacing;
		}
        
		// set this page position
		float aX = page->getAnchorX();
		float aY = page->getAnchorY();
		bool relative = page->isRelativeAnchorPoint();
		float centerX, centerY;
		if(m_vertical) {
			centerX = m_centerX == -1 ? (m_width / 2) : m_centerX;
			centerY = center;
		} else {
			centerX = center;
			centerY = m_centerY == -1 ? (m_height / 2) : m_centerY;
		}
		if(relative) {
			page->setPosition(centerX + (aX - 0.5f) * page->getWidth()*page->getScaleX(), centerY + (aY - 0.5f) * page->getHeight()*page->getScaleY());
		} else {
			page->setPosition(centerX - page->getWidth()*page->getScaleX() / 2, centerY - page->getHeight()*page->getScaleY() / 2);
		}
        
		// get over this page
		center += m_vertical ? (page->getHeight()*page->getScaleY() / 2) : (page->getWidth()*page->getScaleX() / 2);
	}
}

void ArmsPageControl::setVertical(bool flag) {
	m_vertical = flag;
	updatePagePositions();
	setInitialPage(m_initialPageIndex);
}

void ArmsPageControl::addPage(wyNode* page) {
	// add page to array
	wyArrayPush(m_pages, page);
	page->retain();
	m_container->addChildLocked(page);
	updatePagePositions();
    
	// notify indicator
	if(m_indicator)
		m_indicator->onPageAdded(m_pages->num - 1);
    
	// ensure page position changed is set
	notifyOnPagePositionChanged();
}

void ArmsPageControl::addPageAt(wyNode* page, int index) {
	if(index < 0 || index > m_pages->num)
		return;
    
	wyArrayInsert(m_pages, page, index);
	page->retain();
	m_container->addChildLocked(page);
	updatePagePositions();
    
	// notify indicator
	if(m_indicator)
		m_indicator->onPageAdded(index);
    
	// ensure page position changed is set
	notifyOnPagePositionChanged();
}

void ArmsPageControl::removePage(wyNode* page) {
//	int index = wyArrayIndexOf(m_pages, page, NULL, NULL);
	if(wyArrayDeleteObj(m_pages, page, NULL, NULL) != NULL) {
		// remove from container
		m_container->removeChildLocked(page, true);
        
		// notify indicator before page is released
//		if(m_indicator)
//			m_indicator->onPageRemoved(page, index);
        
		// release page and update positions
		page->release();
		updatePagePositions();
	}
}

void ArmsPageControl::removePageAt(int index) {
	wyNode* page = (wyNode*)wyArrayDeleteIndex(m_pages, index);
	if(page != NULL) {
		// remove from container
		m_container->removeChildLocked(page, true);
        
		// notify indicator before page is released
		if(m_indicator)
			m_indicator->onPageRemoved(page, index);
        
		// release page and update positions
		page->release();
		updatePagePositions();
	}
}

void ArmsPageControl::removeAllPages() {
	for(int i = 0; i < m_pages->num; i++) {
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
		m_container->removeChildLocked(page, true);
	}
	wyArrayEach(m_pages, releasePage, NULL);
	wyArrayClear(m_pages);
    
	// notify indicator
	if(m_indicator)
		m_indicator->onPageAllRemoved();
}

void ArmsPageControl::setPageSpacing(float spacing) {
	m_pageSpacing = spacing;
	updatePagePositions();
}

float ArmsPageControl::getContainerWidth() {
	return getPageCenterX(m_pages->num - 1) + m_width / 2;
}

float ArmsPageControl::getContainerHeight() {
	return getPageCenterY(m_pages->num - 1) + m_height / 2;
}

void ArmsPageControl::setCallback(ArmsPageControlCallback* callback, void* data) {
	if(callback == NULL) {
		memset(&m_callback, 0, sizeof(ArmsPageControlCallback));
		m_data = NULL;
	} else {
		memcpy(&m_callback, callback, sizeof(ArmsPageControlCallback));
		m_data = data;
	}
}

int ArmsPageControl::getLeftIndex() {
	float position = m_vertical ? (m_height / 2 - m_container->getPositionY())
    : (m_width / 2 - m_container->getPositionX());
	float center = m_vertical ? (m_height / 2) : (m_width / 2);
	for(int i = 0; i < m_pages->num; i++) {
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
        
		// add this page offset
		if(i > 0) {
			center += m_vertical ? (page->getHeight()*page->getScaleY() / 2) : (page->getWidth()*page->getScaleX() / 2);
			center += m_pageSpacing;
		}
        
		if(position <= center)
			return i - 1;
        
		// get over this page
		if(i < m_pages->num - 1)
			center += m_vertical ? (page->getHeight()*page->getScaleY() / 2) : (page->getWidth()*page->getScaleX() / 2);
	}
    
	return m_pages->num - 1;
}

int ArmsPageControl::getRightIndex() {
	return getLeftIndex() + 1;
}

int ArmsPageControl::getVisibleFirstIndex() {
	int count = m_pages->num;
	if(count <= 0)
		return -1;
    
	int lower = 0;
	int upper = m_vertical ? m_height : m_width;
	wyNode* firstPage = (wyNode*)wyArrayGet(m_pages, 0);
	float start = m_vertical ? (m_height / 2 - firstPage->getHeight()*firstPage->getScaleY() / 2) :
    (m_width / 2 - firstPage->getWidth()*firstPage->getScaleX() / 2);
	start += m_vertical ? m_container->getPositionY() : m_container->getPositionX();
	for(int i = 0; i < count; i++) {
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
		int end = start + (m_vertical ? page->getHeight()*page->getScaleY() : page->getWidth()*page->getScaleX());
		if(lower < end && upper > start)
			return i;
        
		// add this page offset
		start += m_vertical ? page->getHeight()*page->getScaleY() : page->getWidth()*page->getScaleX();
		start += m_pageSpacing;
	}
    
	return -1;
}

int ArmsPageControl::getVisibleLastIndex() {
	int count = m_pages->num;
	if(count <= 0)
		return -1;
    
	int lower = 0;
	int upper = m_vertical ? m_height : m_width;
	wyNode* lastPage = (wyNode*)wyArrayGet(m_pages, count - 1);
	float start = m_vertical ? (getPageCenterY(count - 1) - lastPage->getHeight()*lastPage->getScaleY() / 2) :
    (getPageCenterX(count - 1) - lastPage->getWidth()*lastPage->getScaleX() / 2);
	start += m_vertical ? m_container->getPositionY() : m_container->getPositionX();
	for(int i = count - 1; i >= 0; i--) {
		// subtract this page offset
		wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
		if(i < count - 1) {
			start -= m_vertical ? page->getHeight()*page->getScaleY() : page->getWidth()*page->getScaleX();
			start -= m_pageSpacing;
		}
        
		int end = start + (m_vertical ? page->getHeight()*page->getScaleY() : page->getWidth()*page->getScaleX());
		if(lower < end && upper > start)
			return i;
	}
    
	return -1;
}

int ArmsPageControl::getBestIndex() {
	int leftIndex = getLeftIndex();
	int rightIndex = leftIndex + 1;
	float left = m_vertical ? (m_height / 2 - getPageCenterY(leftIndex)) : (m_width / 2 - getPageCenterX(leftIndex));
	float right = m_vertical ? (m_height / 2 - getPageCenterY(rightIndex)) : (m_width / 2 - getPageCenterX(rightIndex));
	float start = m_vertical ? m_container->getPositionY() : m_container->getPositionX();
	float leftDelta = fabs(left - start);
	float rightDelta = fabs(right - start);
	if(leftDelta < rightDelta)
		return leftIndex == -1 ? 0 : leftIndex;
	else
		return rightIndex == m_pages->num ? (m_pages->num - 1) : rightIndex;
}

bool ArmsPageControl::touchesBegan(wyMotionEvent& e) {
	// stop fling
	if(m_flinging) {
		m_scroller->forceFinished(true);
		m_flinging = false;
	}
    
	m_lastX = e.x[0];
	m_lastY = e.y[0];
	m_beginX = e.x[0];
	m_beginY = e.y[0];
	m_scrolling = true;
	m_xMove = 0;
	m_yMove = 0;
	m_largeMove = false;
    isStartMoving = false;
	return true;
}

bool ArmsPageControl::touchesMoved(wyMotionEvent& e) {
	if(m_scrolling) {
		// get delta move and accumulate them
		float dx = e.x[0] - m_lastX;
		float dy = e.y[0] - m_lastY;
		m_xMove += dx;
		m_yMove += dy;
        
		// if small move is not set yet, need check
		if(!m_largeMove) {
			m_largeMove = m_vertical ? (fabs(m_yMove) >= DP(20)) : (fabs(m_xMove) >= DP(20));
            
			// if in large move, this event should be processed by page control exclusively
			if(m_largeMove)
				wyEventDispatcher::getInstance()->cancelTouchExcept(e, this);
		}
        
		// move container
        if (!isStartMoving && fabs(m_xMove) < DP(20)) {
            
        } else {
            if(m_vertical) {
                m_container->setPosition(m_container->getPositionX(),
                                         MAX(MIN(m_height / 2, m_container->getPositionY() + dy), m_height / 2 - getContainerHeight()));
            } else {
                m_container->setPosition(MAX(MIN(m_width / 2, m_container->getPositionX() + dx), m_width / 2 - getContainerWidth()),
                                         m_container->getPositionY());
            }
            isStartMoving = true;
        }
//        isFirstTouch = false;
        
		m_lastX = e.x[0];
		m_lastY = e.y[0];
        
		// notify page position changed
		notifyOnPagePositionChanged();
        
		return true;
	}
    
	return false;
}

bool ArmsPageControl::touchesEnded(wyMotionEvent& e) {
    isStartMoving = false;
    
	if(m_scrolling) {
		m_scrolling = false;
        
		if(!m_flinging) {
			int curIndex = getBestIndex();
			float start = m_vertical ? m_container->getPositionY() : m_container->getPositionX();
			wyPoint loc = wyp(e.x[0], e.y[0]);
            
			// if move is small, check page clicking
			// if move is not small, check fling
			bool clicked = false;
			if(!m_largeMove) {
				int firstIndex = getVisibleFirstIndex();
				int lastIndex = getVisibleLastIndex();
				for(int i = firstIndex; i <= lastIndex; i++) {
					wyNode* page = (wyNode*)wyArrayGet(m_pages, i);
					if(page) {
						wyRect bound = page->getBoundingBoxRelativeToWorld();
						if(wyrContains(bound, loc)) {
							// normal center of this page
							float center = m_vertical ? (m_height / 2 - getPageCenterY(i)) : (m_width / 2 - getPageCenterX(i));
                            
							// if it is current page, check whether it is clicked
							if(curIndex == i) {
								if(fabs(center - start) < DP(20)) {
									clicked = true;
//									notifyOnPageClicked(i);
								}
							}
                            
							// if not
							if(!clicked) {
								m_flinging = true;
								if(m_vertical)
									m_scroller->startScroll(0, start, 0, center - start, 1000);
								else
									m_scroller->startScroll(start, 0, center - start, 0, 1000);
							}
                            
							break;
						}
					}
				}
			}
            
			// if not clicked, fling
			if(!clicked && !m_flinging) {
				m_flinging = true;
				if(!m_largeMove) {
					int best = getBestIndex();
					float center = m_vertical ? (m_height / 2 - getPageCenterY(best)) : (m_width / 2 - getPageCenterX(best));
					if(m_vertical)
						m_scroller->startScroll(0, start, 0, center - start, 1000);
					else
						m_scroller->startScroll(start, 0, center - start, 0, 1000);
				} else {
					float start = m_vertical ? m_container->getPositionY() : m_container->getPositionX();
					float end = m_vertical ? (m_height / 2 - getPageCenterY(e.y[0] < m_beginY ? getRightIndex() : getLeftIndex()))
                    : (m_width / 2 - getPageCenterX(e.x[0] < m_beginX ? getRightIndex() : getLeftIndex()));
					if(m_vertical)
						m_scroller->startScroll(0, start, 0, end - start, 1000);
					else
						m_scroller->startScroll(start, 0, end - start, 0, 1000);	
				}
                
				return true;	
			}
		}
        
		return true;
	}
    
	return false;
}

bool ArmsPageControl::touchesCancelled(wyMotionEvent& e) {
	return touchesEnded(e);
}

void ArmsPageControl::setPageIndicator(MyPageIndicator* indicator) {
	if(m_indicator != indicator) {
		if(m_indicator)
			removeChildLocked(m_indicator, true);
		m_indicator = indicator;
		if(m_indicator) {
			addChildLocked(m_indicator, 0, 10000);
			m_indicator->initWithPages(m_pages->num, getBestIndex());
		}
	}
}






