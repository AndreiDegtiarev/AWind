/*
GUI library for Arduino TFT and OLED displays

Copyright (c) 2014-2018 Andrei Degtiarev

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this file except in compliance with the License.  You may
obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.
*/
#pragma once

#include "Window.h"
#include "MainWindow.h"
#include "ICriticalProcess.h"
#include "Touch.h"

DC *globalLcd;

///Main window manager. Implement core funcctionality of AWind library. It is tempate class that has to be parametrized with main window class
template <class T=MainWindow> class WindowsManager :  public ICriticalProcess, public ILoopProcess
{
	DC *_dc;
	T *_mainWindow;
	Touch  *_touch;
public:
	///Constructor
	/**
	\param lcd pointer to UTFT object (see UTFT library)
	\param touch pointer to UTouch object (see UTouch library)
	*/	
	WindowsManager(DC *lcd,Touch *touch=NULL):_dc(lcd),_touch(touch)
	{
		globalLcd=lcd;
	}
	///Initialization procedure. Has to be call once
	void Initialize()
	{
		_mainWindow=new T(_dc->DeviceWidth(),_dc->DeviceHeight());
		_mainWindow->Invalidate();
		_mainWindow->SetLoopProcess(this);
		_mainWindow->SetDecorators(Environment::Get()->FindDecorators(F("Window")));
	}
	///Returns topmost window that lais under x,y screen coordinate
	Window *HitTest(int x,int y)
	{
		if(MainWnd()->ModalWnd()!=NULL)
			return HitTest(MainWnd()->ModalWnd(),x,y);
		else
			return HitTest(MainWnd(),x,y);
	}
	///For defined window return topmost child window that lais under x,y screen coordinate
	Window *HitTest(Window *window,int x,int y)
	{
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
	///Main loop where drawing code only for "dirty" window is called
	void loop()
	{
		bool isRedrawn = false;
		if(MainWnd()->ModalWnd() == NULL)
			isRedrawn = redraw(MainWnd(),false);
		if(MainWnd()->ModalWnd() != NULL)
			isRedrawn = redraw(MainWnd()->ModalWnd(),MainWnd()->ModalWnd()->IsDirty()) || isRedrawn;
		if (isRedrawn)
			_dc->Display();
	}
	T *MainWnd()
	{
		return _mainWindow;
	}
	///Returns device context (interface class to UTFT library)
	DC *GetDC()
	{
		return _dc;
	}
	///If nothing happens, the touch event is checked
	void Idle()
	{
		if(_touch!=NULL)
			loopTouch();
	}
protected:
	///Internal draw code
	bool redraw(Window *window,bool isForceRedraw)
	{
		bool isRedrawn = false;
		Idle();
		if(window == MainWnd()->ModalWnd() && window->IsDirty() && isForceRedraw == false) // Modal window is updated as last one by use of isForceRedraw flag
			return false;
		bool needRedraw=isForceRedraw || window->IsDirty();
		if (needRedraw)
		{
			window->Redraw(_dc);
			isRedrawn = true;
		}
		for(int i=0;i<window->Children().Count();i++)
		{
			Window *child=window->Children()[i];
			if(child->IsVisible())
				isRedrawn = redraw(child,needRedraw) || isRedrawn;
		}
		return isRedrawn;
	}
	///Checks touch event
	void loopTouch()
	{
		//out<<F("Check touch")<<endln;
		if (_touch->dataAvailable())
		{
			_touch->read();
			int x=_touch->getX();
			int y=_touch->getY();
			//out<<F("Touch begins x:")<<x<<F(" y:")<<y<<endln;
			if(x>0 && y>0)
			{
				Window *window=_dc->ScreenOrientation()==DC::Landscape?HitTest(x,y): HitTest(y, _dc->DeviceHeight()-x);
				Window *touchWnd=NULL;
				if(window!=NULL)
				{
					Window *crWindow=window;
					//out<<F("Searching touch window: ")<<endln;
					while(crWindow!=NULL && (!crWindow->IsAwaitTouch()))
					{
						//out<<crWindow->Name()<<endln;
						crWindow=crWindow->Parent();	
					}
					if(crWindow != NULL)
					{
						//out<<F("Touch found")<<endln;
						touchWnd=crWindow;
						touchWnd->OnTouching(GetDC());
					}
				}
				if(touchWnd!=NULL)
				{
					while (_touch->dataAvailable())
					{
						_touch->read();
					}
					//out<<F("Touch: ")<<touchWnd->Name()<<endln;
					{
						Window *crWindow=touchWnd;
						while(crWindow!=NULL && ((crWindow->IsAwaitTouch()) && !(crWindow)->OnTouch(x,y))||!crWindow->IsAwaitTouch())
						{
							crWindow=crWindow->Parent();	
							//out<<F("Touch while")<<endln;
						}
						//out<<F("Touch invalidate")<<endln;
						touchWnd->Invalidate();
					}
				}
				//out<<F("Touch finish")<<endln;
			}
		}
	}
};