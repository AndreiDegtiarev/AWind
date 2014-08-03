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

#include "TextBoxString.h"
#include "SensorWindow.h"

class ViewModusWindow : public MainWindow,ITouchEventReceiver
{
	enum Modus
	{
		Day,
		Night
	};
	static const ARGB DaylightBkColor=Color::CadetBlue;
    static const ARGB NightBkColor=Color::Black;
	TextBoxFString *_text;
	Modus _modus;
public:
	ViewModusWindow(int width,int height):MainWindow(width,height)
	{
		SetBackColor(Color::Black);
		_text=new TextBoxFString(width-100,height-45,95,35,F("Night"),Color::White);
		_text->SetBackColor(DaylightBkColor);
		_modus=Day;
		_text->SetFont(BigFont);
		_text->SetMargins(15,10);
		AddChild(_text);
		this->RegisterTouchEventReceiver(this);
	}
	void Initialize()
	{
		updateBackColor();
	}
	void NotifyTouch(Window *wnd)
	{
		if(_modus==Day)
		{
			_modus=Night;
			_text->SetText(F("Day"));
		}
		else
		{
			_modus=Day;	
			_text->SetText(F("Night"));
		}
		updateBackColor();
		Invalidate();
	}
protected:
	void updateBackColor()
	{
		for(int i=0;i<Children().Count();i++)
		{
			Children()[i]->SetBackColor(_modus==Day?DaylightBkColor:NightBkColor);
		}
	}
};
