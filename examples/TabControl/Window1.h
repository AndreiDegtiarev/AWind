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
#include <Label.h>
///Example window with red background
class Window1 : public Window
{
public:
	Window1(const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		Label *label = new Label(0, 0, 0, 0, F("Tab 1"));
		label->SetFont(BigFont);
		AddChild(label);
		SetDecorators(Environment::Get()->FindDecorators(F("Window")));
	}
	virtual void Move(int left, int top, int width, int height)
	{
		Window::Move(left, top, width, height);
		Children()[0]->Move(width/2-40, height/2, width-20,40);
	}
};