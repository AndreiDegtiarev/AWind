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
#include "ICriticalProcess.h"

class WindowsManager : public IEvent<Window>
{
	DC _dc;
	Window _mainWindow;
	KeyboardWindow _keyboardWindow;
	ICriticalProcess *_criticalProcess;
	Window *_modalWindow;
public:
	WindowsManager(UTFT *lcd,int width,int height):_dc(lcd),
													_mainWindow(F("Main"),0,0,width-1,height-1),
													_keyboardWindow(3,90)
	{
		_criticalProcess=NULL;
		_modalWindow=NULL;
		_keyboardWindow.SetVisible(false);
		_mainWindow.AddChild(&_keyboardWindow);
		_keyboardWindow.SetEndEditEvent(this);
	}
	void SetCriticalProcess(ICriticalProcess *criticalProcess)
	{
		_criticalProcess=criticalProcess;
	}
	void SetModalWindow(Window * modalWindow)
	{
		_modalWindow=modalWindow;
	}
	void Notify(Window *window)
	{
		if(window == &_keyboardWindow) //End edit
		{
			SetModalWindow(NULL);
			_mainWindow.Invalidate();
		}
	}
	Window *HitTest(int x,int y)
	{
		if(_modalWindow!=NULL)
			return HitTest(_modalWindow,x,y);
		else
			return HitTest(&_mainWindow,x,y);
	}
	Window *HitTest(Window *window,int x,int y)
	{
		/*Log::Number("Test wnd touch x: ",x);
		Log::Number(" y : ",y,true);
		Serial.println(Name());*/
		if(window->IsVisible()
			&&x>=window->Left() && x<=window->Left()+window->Width()
			&&y>=window->Top() && y<=window->Top()+window->Height())
		{
			for(int i=0;i<window->Children().Count();i++)
			{
				Window * retWnd=HitTest(window->Children()[i],x-window->Left(),y-window->Top());
				if(retWnd!=NULL)
					return retWnd;
			}
			return window;
		}
		return NULL;
	}
	void loop()
	{
		//if(_keyboardWindow.IsVisible())
		//	redraw(&_keyboardWindow,false);
		if(_modalWindow == NULL)
			redraw(MainWindow(),false);
		else 
			redraw(_modalWindow,false);
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
		if(_criticalProcess!=NULL)
			_criticalProcess->loop();
		bool needRedraw=isForceRedraw || window->IsDirty();
		if(needRedraw)
		{
			//Log::Line(window->GetType());
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