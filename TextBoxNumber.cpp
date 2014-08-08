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
#include "UTFT.h"
#include "TextBoxNumber.h"
#include "MainWindow.h"

bool TextBoxNumber::OnTouch(int x,int y)
{
	bool retCode=TextBox::OnTouch(x,y);
	if(!_isReadOnly)
	{
		KeyboardWindow * kbrWnd=(KeyboardWindow *)FindDialog(F("Keyboard"));
		if(kbrWnd!=NULL)
		{
			kbrWnd->Initialize(GetNumber(),Precission());
			if(DoDialog(kbrWnd) == IDialogClosedEventReceiver::OK)
				SetNumber(kbrWnd->GetNumber());
		}
	}
	return true;
}
