/*
 *  CoreWindow.h
 *  sk_mt_test
 *
 *  Created by kioku on 10/08/11.
 *  Copyright 2010 System K. All rights reserved.
 *
 */

#ifndef INCLUDE_CORE_WINDOW_H
#define INCLUDE_CORE_WINDOW_H

//#include "CoreType.h"

class CoreWindowPlatform
{
protected:
	CoreWindowPlatform()
	{
	}
public:
	virtual ~CoreWindowPlatform()
	{
	}
	
	virtual void MouseLeftDown  (int x, int y) = 0;
	virtual void MouseLeftUp    (int x, int y) = 0;
	virtual void MouseRightDown (int x, int y) = 0;
	virtual void MouseRightUp   (int x, int y) = 0;
	virtual void MouseMiddleDown(int x, int y) = 0;
	virtual void MouseMiddleUp  (int x, int y) = 0;
	virtual void MouseMove      (int x, int y) = 0;
	virtual void Wheel          (float dx, float dy, float dz) = 0;
	virtual void Magnify        (float z)      = 0;
	virtual void Swipe          (float dx, float dy, float dz) = 0;
	virtual void Rotate         (float r)      = 0;
	virtual void KeyDown        (int key)      = 0;
	virtual void KeyUp          (int key)      = 0;
	virtual void Resize         (int w, int h) = 0;
	virtual void Close          (void)         = 0;
	virtual void Idle           (void)         = 0;
	virtual void Draw           (void)         = 0;
	virtual void Active         (void)         = 0;
	virtual void Toplevel       (bool top)     = 0;
    virtual void SwapBuffer     (void)         = 0;
};

#if defined (__APPLE__)
#include "CoreWindow_mac.h"
#define CoreWindow CoreWindow_mac
#elif defined(_WIN32)
#include "CoreWindow_wgl.h"
#define CoreWindow CoreWindow_wgl
#else
#include "CoreWindow_x11.h"
#define CoreWindow CoreWindow_x11
#endif

#endif // INCLUDE_CORE_WINDOW_H