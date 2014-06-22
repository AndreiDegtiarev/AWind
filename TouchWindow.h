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
class TouchWindow : public Window
{
public:
	TouchWindow(const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		_type=F("TouchWindow");
	}
	virtual void OnTouching(DC *dc)
	{
		dc->SetColor(Color::Red);
		dc->DrawRoundRect(0,0,Width(),Height());
		dc->DrawRoundRect(1,1,Width()-1,Height()-1);
	}
	virtual bool OnTouch(int x,int y)
	{
		return false;
	}
};