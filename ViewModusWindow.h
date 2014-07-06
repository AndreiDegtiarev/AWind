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

class ViewModusWindow : public TextBoxString<const __FlashStringHelper>
{
	enum Modus
	{
		Day,
		Night
	};
	SensorWindow::BkColorMode _modus;
	LinkedList<SensorWindow> _vis_sensors;
public:
	ViewModusWindow(LinkedList<SensorWindow> vis_sensors,int left,int top,int width,int height):TextBoxString<const __FlashStringHelper>(left,top,width,height,F("Night"),Color::White)
	{
		SetBackColor(Color::CadetBlue);
		_modus=SensorWindow::Day;
		_vis_sensors=vis_sensors;
		SetFont(BigFont);
		SetTextOffset(15,10);
	}
	virtual bool OnTouch(int x, int y)
	{
		SensorWindow::BkColorMode mode=SensorWindow::Day;
		if(_modus==Day)
		{
			_modus=SensorWindow::Night;
			SetText(F("Day"));
		}
		else
		{
			_modus=SensorWindow::Day;	
			SetText(F("Night"));
		}
		for(int i=0;i<_vis_sensors.Count();i++)
		{
			_vis_sensors[i]->SetBkColorMode(_modus);
			_vis_sensors[i]->Invalidate();
		}
		SetBackColor(_modus==SensorWindow::Day?SensorWindow::DaylightBkColor:SensorWindow::NightBkColor);
		Invalidate();
		return true;
	}
};
