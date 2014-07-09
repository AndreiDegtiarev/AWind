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

#include "TouchWindow.h"

class TouchManager : public ICriticalProcess
{
	WindowsManager *_windowsManager;
	UTouch  *_touch;
public :
	TouchManager(UTouch  *touch,WindowsManager *windowsManager)
	{
		_windowsManager=windowsManager;
		_touch = touch;
	}
	void loop()
	{
		out<<F("Touch loop")<<endl;
		if (_touch->dataAvailable())
		{
			_touch->read();
			int x=_touch->getX();
			int y=_touch->getY();
			out<<F("Touch begins x:")<<x<<F(" y:")<<y<<endl;
			if(x>0 && y>0)
			{
				Window *window=_windowsManager->HitTest(x,y);
				Window *touchWnd=NULL;
				if(window!=NULL)
				{
					Window *crWindow=window;
					out<<F("Searching touch window: ")<<endl;
					//Serial.println(F("Searching touch window: "));
					while(crWindow!=NULL && (!crWindow->IsAwaitTouch()))
					{
						/*Serial.print(crWindow->Name());
						Serial.print(" ");
						Serial.println(crWindow->GetType());*/
						out<<crWindow->Name()<<endl;
						crWindow=crWindow->Parent();	
					}
					if(crWindow != NULL)
					{
						//Serial.println(F("Touch found"));
						out<<F("Touch found")<<endl;
						touchWnd=crWindow;
						touchWnd->PrepareDC(_windowsManager->GetDC());
						touchWnd->OnTouching(_windowsManager->GetDC());

					}
				}
				if(touchWnd!=NULL)
				{
					while (_touch->dataAvailable())
					{
						_touch->read();
					}
					out<<F("Touch: ")<<touchWnd->Name()<<endl;
					if(touchWnd->IsOfType(F("TextBoxNumber")) && !((TextBoxNumber *)touchWnd)->IsReadOnly())
					{
						_windowsManager->SetModalWindow(_windowsManager->Keyboard());
						_windowsManager->Keyboard()->BeginEdit((TextBoxNumber *)touchWnd);
					}
					else
					{
						Window *crWindow=touchWnd;
						while(crWindow!=NULL && ((crWindow->IsAwaitTouch()) && !((TouchWindow *)crWindow)->OnTouch(x,y))||!crWindow->IsAwaitTouch())
						{
							crWindow=crWindow->Parent();	
						}
						//if(crWindow !=NULL)
						//	crWindow->Invalidate();
						touchWnd->Invalidate();
					}
				}
				out<<F("Touch finish")<<endl;
			}
		}
	}
};