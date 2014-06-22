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

class TouchManager
{
	WindowsManager *_windowsManager;
	UTouch  *_touch;
public :
	TouchManager(UTouch  *touch,WindowsManager *windowsManager)
	{
		_windowsManager=windowsManager;
		_touch = touch;
	}
	Window *HitTest(Window *window,int x,int y)
	{
		//Serial.print("Test wnd touch: ");
		//Serial.println(window->Name());
		if(window->IsVisible()
			&&x>=window->Left() && x<=window->Left()+window->Width()
			&&y>=window->Top() && y<=window->Top()+window->Height())
		{
			for(int i=0;i<window->Children().Count();i++)
			{
				Window * retWnd=HitTest(window->Children()[i],x,y);
				if(retWnd!=NULL)
					return retWnd;
			}
			return window;
		}
		return NULL;
	}
	void loop()
	{
		if (_touch->dataAvailable())
		{
			/*Log::Number("Signal window: ",_signalWindow->Left());
			Log::Number(" ",_signalWindow->Top());
			Log::Number(" ",_signalWindow->Width());
			Log::Number(" ",_signalWindow->Height(),true);*/
			_touch->read();
			int x=_touch->getX();
			int y=_touch->getY();
			Window *window=HitTest(_windowsManager->MainWindow(),x,y);
			TouchWindow *touchWnd=NULL;
			if(window!=NULL)
			{
				Window *crWindow=window;
				Serial.println(F("Searching touch window: "));
				while(crWindow!=NULL && (!crWindow->IsOfType(F("TouchWindow"))))
				{
					Serial.print(crWindow->Name());
					Serial.print(" ");
					Serial.println(crWindow->GetType());
					crWindow=crWindow->Parent();	
				}
				if(crWindow != NULL)
				{
					Serial.println(F("Touch found"));
					touchWnd=(TouchWindow *)crWindow;
					touchWnd->PrepareDC(_windowsManager->GetDC());
					touchWnd->OnTouching(_windowsManager->GetDC());

				}
			}
			Serial.print(F("Touch begins: ("));
			Serial.print(x);Serial.print(",");Serial.print(y);
			Serial.print(F(") "));
			Serial.print(window->Name());
			Serial.println();
			while (_touch->dataAvailable())
			{
				_touch->read();
			}
			Serial.print("Touch: ");
//			Window *window=HitTest(_windowsManager->MainWindow(),x,y);
			Serial.println(window->Name());
			Window *crWindow=touchWnd;
			while(crWindow!=NULL && ((crWindow->IsOfType(F("TouchWindow")) && !((TouchWindow *)crWindow)->OnTouch(x,y))||!crWindow->IsOfType(F("TouchWindow"))))
			{
				crWindow=crWindow->Parent();	
			}
			Serial.println("Touch finish");
			if(crWindow !=NULL)
				crWindow->Invalidate();
		}
	}
};