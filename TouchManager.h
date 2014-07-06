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
			if(x>0 && y>0)
			{
				Window *window=_windowsManager->MainWindow()->HitTest(x,y);
				TouchWindow *touchWnd=NULL;
				if(window!=NULL)
				{
					Window *crWindow=window;
					//Serial.println(F("Searching touch window: "));
					while(crWindow!=NULL && (!crWindow->IsOfType(F("TouchWindow"))))
					{
						/*Serial.print(crWindow->Name());
						Serial.print(" ");
						Serial.println(crWindow->GetType());*/
						crWindow=crWindow->Parent();	
					}
					if(crWindow != NULL)
					{
						//Serial.println(F("Touch found"));
						touchWnd=(TouchWindow *)crWindow;
						touchWnd->PrepareDC(_windowsManager->GetDC());
						touchWnd->OnTouching(_windowsManager->GetDC());

					}
				}
				/*Serial.print(F("Touch begins: ("));
				Serial.print(x);Serial.print(",");Serial.print(y);
				Serial.print(F(") "));
				Serial.println(window->Name());*/
				while (_touch->dataAvailable())
				{
					_touch->read();
				}
				//Serial.print("Touch: ");
				//Serial.println(window->Name());
				if(window->IsOfType(F("TextBoxNumber")) && !((TextBoxNumber *)window)->IsReadOnly())
				{
					_windowsManager->Keyboard()->BeginEdit((TextBoxNumber *)window);
					Serial.println(F("Begin edit"));
				}
				else
				{
					Window *crWindow=touchWnd;
					while(crWindow!=NULL && ((crWindow->IsOfType(F("TouchWindow")) && !((TouchWindow *)crWindow)->OnTouch(x,y))||!crWindow->IsOfType(F("TouchWindow"))))
					{
						crWindow=crWindow->Parent();	
					}
					//if(crWindow !=NULL)
					//	crWindow->Invalidate();
					touchWnd->Invalidate();
				}
				//Serial.println("Touch finish");
			}
		}
	}
};