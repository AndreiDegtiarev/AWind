#pragma once 
/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  

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
///Gauge window class, that allows visualisaion of data in form of bar or radial pointer
class Gauge : public Window
{
public:
	Color _fillColor;  //!< Gauge fill color 
	float _value;     //!< Current gauge value
	float _oldValue;  //!< Previos gauge value
	float _minValue;  //!< Gauge min limit
	float _maxValue;  //!< Gauge max limit
	bool  _drawOnlyPointer; //!< Defines whether only pointer (not scale has to be drawn)

	static const float _sector_angle_rad;

	///Constructor
	/**
	\param gaugeType defines gauge appearance
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	Gauge(int left,int top,int width,int height)
		:Window(F("gauge"),left,top,width,height),
		_value(0),
		_oldValue(0),
		_minValue(0),
		_maxValue(100),
		_fillColor(Color::Black),
		_drawOnlyPointer(false)
	{
		SetDecorators(Environment::Get()->FindDecorators(F("Gauge")));
	}
	///Sets fill color
	void SetFillColor(Color fillColor)
	{
		_fillColor=fillColor;
	}
	///Sets values range
	/**
	\param minVal min value
	\param maxVal max value
	*/
	void SetMinMax(float minVal,float maxVal)
	{
		_minValue=minVal;
		_maxValue=maxVal;
	}
	///Sets display value
	/**
	\param value desired value
	\return if value is above limits than limit otherwise value
	*/
	float SetValue(float value)
	{
		_value=max(_minValue,value);
		_value=min(_maxValue,_value);
		out<<F("Gauge value: ")<<_value<<endln;
		_drawOnlyPointer=true;
		DC dc;
		PrepareDC(&dc);
		OnDraw(&dc);
		_oldValue=_value;
		_drawOnlyPointer=false;
	}
};
const float Gauge::_sector_angle_rad = 3.14 / 2 * 0.8;