#pragma once
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
#include "Gauge.h"
class GaugesWindow : public Window
{
	///Constructor
	/**
	\param name internal window name that help by debugging
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	Gauge *_gaugeBar;
	Gauge *_gaugeRadialPointer;
	Gauge *_gaugeLevel;
public:
	GaugesWindow(int left,int top,int width,int height):Window(F("Gauges window"),left,top,width,height)
	{
		_gaugeBar=new Gauge(Gauge::Bar,0,0,width/2,height/2);
		_gaugeRadialPointer=new Gauge(Gauge::RadialPointer,width/2,0,width/2,height/2);
		_gaugeLevel=new Gauge(Gauge::Level,0,height/2,width/2,height/2);
		AddChild(_gaugeBar); 
		AddChild(_gaugeRadialPointer);
		AddChild(_gaugeLevel);
		SetBackColor(Color::WhiteSmoke);
	}
};