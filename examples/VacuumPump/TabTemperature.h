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
#include "TimeSerieBuffer.h"
#include <ChartWindow.h>
///Tab with chart for temperature logging 
class TabTemperature : public Window
{

	TimeSerieBuffer	*_dataBuffer;
	ChartWindow *_chartWnd;
	DecoratorAxis *_chartYAxis;

public:
	TabTemperature(const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		SetDecorators(Environment::Get()->FindDecorators(F("Window")));

		_dataBuffer = new TimeSerieBuffer(1, 1, 1000, 1000);

	}
	void Initialize()
	{
		int cy = Height();
		int axis_y_margins = 2;

		DecoratorAxis *chartYAxis = new DecoratorAxis(DecoratorAxis::VerticalLeft, SmallFont, cy - axis_y_margins * 2, 0, 120, 5);
		chartYAxis->SetOffset(4, axis_y_margins);
		_chartWnd = new ChartWindow(NULL, chartYAxis, 1, 1, Width() - 2, cy);
		//Chart decorators
		_chartWnd->AddDecorator(new Decorator3DRect(Color::White, Color::Gray));
		_chartWnd->AddDecorator(new DecoratorColor(Color::Black));
		_chartWnd->AddDecorator(chartYAxis);
		_chartWnd->SetBuffer(_dataBuffer);
		AddChild(_chartWnd);
	}
};