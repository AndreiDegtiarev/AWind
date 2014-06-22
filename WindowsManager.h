/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  

  You can always find the latest version of the library at 
  https://github.com/AndreiDegtiarev/AWind


  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#pragma once

#include "Window.h"

class WindowsManager
{
	Window _mainWindow;
	DC _dc;
	void  (*_fncCritical)();
public:
	WindowsManager(UTFT *lcd,void (*fncCritical)()):_mainWindow(F("Main"),0,0,319,239),_dc(lcd)
	{
		_fncCritical=fncCritical;
	}
	void loop()
	{
		redraw(MainWindow(),false);
	}
	Window *MainWindow()
	{
		return &_mainWindow;
	}
	DC *GetDC()
	{
		return &_dc;
	}
protected:
	void redraw(Window *window,bool isForceRedraw)
	{
		if(_fncCritical!=NULL)
			_fncCritical();
		bool needRedraw=isForceRedraw || window->IsDirty();
		if(needRedraw)
		{
			window->PrepareDC(&_dc);
			window->OnDraw(&_dc);
		}
		for(int i=0;i<window->Children().Count();i++)
		{
			Window *child=window->Children()[i];
			if(child->IsVisible())
				redraw(child,needRedraw);
		}

	}
};