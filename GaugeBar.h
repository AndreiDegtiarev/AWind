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
#include "Gauge.h"
///Bar gauge class
class GaugeBar : public Gauge
{
public:
	DecoratorAxis *_axis; //!<Axis with values or text labels
	///Constructor
	/**
	\param axis defines representation of bar axis
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	GaugeBar(DecoratorAxis *axis,int left,int top,int width,int height)
		:Gauge(left,top,width,height),_axis(axis)
	{
		//Make copy of gauge decorators because of axis decorator
		SetDecorators(new DecoratorList(*GetDecorators())); 
		AddDecorator(axis);
	}
	///Implements drawing code
	/**
	\param dc Device context
	*/
	void OnDraw(DC *dc)
	{
		//out<<"value :"<<_value<<endln;
		dc->SetFont(SmallFont);
		//float range=_maxValue-_minValue;
		float saling_factor=_axis->GetLength()/(_maxValue-_minValue);
		int right_offset=_axis->EstimateLeft(dc);
		int top=_axis->EstimateTop(dc);
		int bottom=_axis->EstimateBottom(dc);
		if(!_drawOnlyPointer)
			dc->DrawRoundRect(2,2,right_offset-1,Height()-2);
		int dc_level=bottom-_value*saling_factor;
		if(_oldValue>_value)
		{
			dc->SetColor(_fillColor);
			dc->FillRoundRect(4,bottom-_oldValue*saling_factor,right_offset-3,min(bottom,dc_level+3));
		}
		dc->SetColor(Color::Red);
		dc->FillRoundRect(4,dc_level,right_offset-3,bottom);
	}
};