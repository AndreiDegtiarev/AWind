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
///Gauge radial pointer class
class GaugeRadialPointer : public Gauge
{
	int _numTicks; //!< Number of ticks (lines) on gauge scale 
	Color _pointerColor;
	int _precision;
public:

	static const float _sector_angle_rad;

	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	\param precission number of digits after decimal point
	*/
	GaugeRadialPointer(int left,int top,int width,int height, int precision = 1)
		:Gauge(left,top,width,height),_numTicks(5), _pointerColor(Color::Red), _precision(precision)

	{
	}
	///Sets number of ticks
	/**
	\param_numTicks number of short lines on scale
	*/
	void SetNumberOfTicks(int numTicks)
	{
		_numTicks=numTicks;
	}
	///Sets number of ticks
	/**
	\param color pointer color
	*/
	void SetPointerColor(Color color)
	{
		_pointerColor = color;
	}
	///Implements drawing code
	/**
	\param dc Device context
	*/
	void OnDraw(DC *dc)
	{
		dc->SetFont(Environment::Get()->FindFont(F("Small")));
		float range=_maxValue-_minValue;
		float step_val=(range)/(_numTicks-1);
		float factor=1000/range;
		int radius=Height()*1;
		int x0=Width()/2;
		int y0=Height()*1.4;
		if(!_drawOnlyPointer)
		{
			dc->DrawRoundRect(2,2,Width()-2,Height()-2);
			dc->Sector(x0,y0,radius,0, _sector_angle_rad);
			float angle_step=_sector_angle_rad/(_numTicks-1.0);
			float angle;
			float sin_val;
			float cos_val;
			float value;
			int x_offset;
			for(int i=0;i<_numTicks;i++)
			{
				angle=-_sector_angle_rad/2.0+angle_step*i;
				sin_val=sin(angle);
				cos_val=cos(angle);
				dc->MoveTo(x0+radius*sin_val,y0-radius*cos_val);
				dc->LineTo(x0+radius*1.1*sin_val,y0-radius*1.1*cos_val);
				value=_minValue+step_val*i;
				x_offset=AHelper::GetNumberLength(value, _precision);
				if(value<0)
					x_offset++;
				if(i==0)
					x_offset=0;
				else if(i == _numTicks-1)
					x_offset=-(x_offset-0.5)*dc->FontWidth();
				else
					x_offset=-x_offset/2*dc->FontWidth();
				dc->DrawNumber(value, _precision,x0+radius*1.1*sin_val+x_offset,y0-radius*1.1*cos_val-(dc->FontHeight()*(i==0||i==_numTicks-1?1.5:1)));
			}
		}
		dc->SetColor(_fillColor);
		DrawPointer(_oldValue,x0,y0,radius,dc);
		dc->SetColor(_pointerColor);
		DrawPointer(_value,x0,y0,radius,dc);
	}
private:
	///Draws pointer in RadialPointer gauge
	void DrawPointer(float value,int x0,int y0,int radius,DC *dc)
	{
		//dc->SetColor(color);
		float dc_val=1000/(_maxValue-_minValue)*(value-_minValue);
		float angle=-_sector_angle_rad/2.0+_sector_angle_rad/1000.0*dc_val;
		float sin_val=sin(angle);
		float cos_val=cos(angle);
		for(int i=-3;i<4;i++)
		{
			dc->MoveTo(x0+radius*0.95*sin_val,y0-radius*0.95*cos_val);
			dc->LineTo(x0+i+radius*0.5*sin_val,y0-radius*0.5*cos_val);
		}

	}
};
const float GaugeRadialPointer::_sector_angle_rad = 3.14 / 2 * 0.8;