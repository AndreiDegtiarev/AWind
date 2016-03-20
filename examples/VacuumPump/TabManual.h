/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
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
#include <Window.h>
#include "PumpController.h"
///Tab for manual control of the pump
class TabManual: public Window, public ITouchEventReceiver
{
	Button *_btnStart;
	Button *_btnStop;
	PumpController *_pumpController;
public:
	TabManual(PumpController *pumpController, const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		_pumpController = pumpController;

		SetDecorators(Environment::Get()->FindDecorators(F("Window")));

		_btnStart = new Button(0, 0, 0, 0, F("Start"));
		initButton(_btnStart);
		_btnStart->SetMargins(20, 20);
		_btnStop = new Button(0, 0, 0, 0, F("Stop"));
		initButton(_btnStop);
		_btnStop->SetMargins(30, 20);
	}
	void initButton(Button *btn)
	{
		btn->RegisterTouchEventReceiver(this);
		AddChild(btn);
	}
	virtual void Move(int left, int top, int width, int height)
	{
		Window::Move(left, top, width, height);
		_btnStart->Move(5, 70, 135, 50);
		_btnStop->Move(172, 70, 135, 50);
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if (window == _btnStart)
		{
			_btnStart->SetDecorators(Environment::Get()->FindDecorators(F("RedRectangle")));
			_pumpController->startPump();
		}
		else if (window == _btnStop)
		{
			_pumpController->stopPump();
			_btnStart->SetDecorators(Environment::Get()->FindDecorators(F("Button")));
			_btnStart->Invalidate();
		}
	}
};