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
#include "MainWindow.h"
#include "ICriticalProcess.h"

class WindowsManager
{
	DC _dc;
	MainWindow _mainWindow;
	ICriticalProcess *_criticalProcess;
public:
	WindowsManager(UTFT *lcd):_dc(lcd)
	{
		_criticalProcess=NULL;
	}
	void Initialize()
	{
		_mainWindow.Move(0,0,_dc.DeviceWidth(),_dc.DeviceHeight());
	}
	void SetCriticalProcess(ICriticalProcess *criticalProcess)
	{
		_criticalProcess=criticalProcess;
	}
	Window *HitTest(int x,int y)
	{
		if(MainWnd()->ModalWnd()!=NULL)
			return HitTest(MainWnd()->ModalWnd(),x,y);
		else
			return HitTest(&_mainWindow,x,y);
	}
	Window *HitTest(Window *window,int x,int y)
	{
		//out<<F("Test wnd touch ")<<window->Name()<<endl;
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
		if(MainWnd()->ModalWnd() == NULL)
			redraw(MainWnd(),false);
		else 
			redraw(MainWnd()->ModalWnd(),false);
	}
	MainWindow *MainWnd()
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