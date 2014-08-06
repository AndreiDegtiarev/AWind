#pragma once
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
#include "Window.h"
#include "KeyboardWindow.h"
class TextBoxNumber;
class MainWindow : public Window, IDialogClosedEventReceiver
{
	Window *_modalWindow;
	KeyboardWindow _keyboardWindow;
public:
	MainWindow(int width,int height):Window(F("Main"),0,0,width,height),_keyboardWindow(3,90)

	{
		_keyboardWindow.SetVisible(false);
		AddChild(&_keyboardWindow);
		_keyboardWindow.RegisterEndDialogEventReceiver(this);
		_modalWindow=NULL;
		//SetBackColor(Color::Black);
	}
	void NotifyDialogClosed(Window *window)
	{
		if(window == &_keyboardWindow) //End edit
		{
			SetModalWindow(NULL);
			Invalidate();
		}
	}
	void StartKeyboard(TextBoxNumber *target)
	{
		SetModalWindow(MainWnd()->Keyboard());
		_keyboardWindow.BeginEdit(target);
	}
	Window *ModalWnd()
	{
		return _modalWindow;
	}
	void SetModalWindow(Window * modalWindow)
	{
		_modalWindow=modalWindow;
	}
	void Move(int left,int top,int width,int height)
	{
		Window::Move(left,top,width,height);
	}
	KeyboardWindow * Keyboard()
	{
		return &_keyboardWindow;
	}
};