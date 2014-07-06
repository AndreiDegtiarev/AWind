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
#include "KeyboardWindow.h"

class WindowsManager
{
	DC _dc;
	Window _mainWindow;
	KeyboardWindow _keyboardWindow;
	void  (*_fncCritical)();
public:
	WindowsManager(UTFT *lcd,void (*fncCritical)(),int width,int height):_dc(lcd),
													_mainWindow(F("Main"),0,0,width-1,height-1),
													_keyboardWindow(1,90)
	{
		_fncCritical=fncCritical;
		_keyboardWindow.SetVisible(false);
		_mainWindow.AddChild(&_keyboardWindow);
	}
	void loop()
	{
		if(_keyboardWindow.IsVisible())
			redraw(&_keyboardWindow,false);
		else
			redraw(MainWindow(),false);
	}
	Window *MainWindow()
	{
		return &_mainWindow;
	}
	KeyboardWindow * Keyboard()
	{
		return &_keyboardWindow;
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