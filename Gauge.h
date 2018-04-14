#pragma once 
/*
GUI library for Arduino TFT and OLED displays

Copyright (c) 2014-2018 Andrei Degtiarev

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this file except in compliance with the License.  You may
obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.
*/
#include "Window.h"
extern DC *globalLcd;
/*extern uint8_t *BigFont;
extern uint8_t *SmallFon*/;

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
		//out<<F("Gauge value: ")<<_value<<endln;
		_drawOnlyPointer=true;
		if (this->IsVisible())
		{
			PrepareDC(globalLcd);
			OnDraw(globalLcd);
		}
		_oldValue=_value;
		_drawOnlyPointer=false;
	}
};
const float Gauge::_sector_angle_rad = 3.14 / 2 * 0.8;