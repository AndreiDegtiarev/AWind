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
#include "Gauge.h"
#include "DecoratorPrimitives.h"
///Bar gauge class
class GaugeBar : public Gauge
{
	Color _barColor;
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
		_barColor = Color::Red;
	}
	///Set Bar color
	void SetBarColor(Color color)
	{
		_barColor = color;
	}
	///Implements drawing code
	/**
	\param dc Device context
	*/
	void OnDraw(DC *dc)
	{
		//out<<"value :"<<_value<<endln;
		dc->SetFont(Environment::Get()->FindFont(F("Small")));
		//float range=_maxValue-_minValue;
		float saling_factor=_axis->GetLength()/(_maxValue-_minValue);
		int right_offset = _axis->EstimateLeft(dc);
		int top = _axis->EstimateTop(dc);
		int bottom = _axis->EstimateBottom(dc);
		if (!_drawOnlyPointer)
			dc->DrawRoundRect(2, 2, right_offset - 1, Height() - 2);
		if (_axis->Orientation() == DecoratorAxis::VerticalLeft || _axis->Orientation() == DecoratorAxis::VerticalRight)
		{
			int dc_level = bottom - _value*saling_factor;
			if (_oldValue > _value)
			{
				dc->SetColor(_fillColor);
				dc->FillRoundRect(4, bottom - _oldValue*saling_factor, right_offset - 3, min(bottom, dc_level + 3));
			}
			dc->SetColor(_barColor);
			dc->FillRoundRect(4, dc_level, right_offset - 3, bottom);
		}
		else
		{
			bottom = Height() - 4;
			int dc_level = _value*saling_factor;
			if (_oldValue > _value)
			{
				dc->SetColor(_fillColor);
				dc->FillRoundRect(max(4, dc_level - 3), 4, _oldValue*saling_factor, bottom);
			}
			dc->SetColor(_barColor);
			dc->FillRoundRect(4, 4, dc_level, bottom);
		}
	}
};