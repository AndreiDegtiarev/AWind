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
#include "AHelper.h"
///Gauge radial pointer class
class GaugeRadialPointer : public Gauge
{
	int _numTicks; //!< Number of ticks (lines) on gauge scale 

public:

	static const float _sector_angle_rad=3.14/2*0.8;

	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	GaugeRadialPointer(int left,int top,int width,int height)
		:Gauge(left,top,width,height),_numTicks(5)

	{
	}
	///Sets number of ticks
	/**
	\param _numTicks number of short lines on scale
	*/
	void SetNumberOfTicks(int numTicks)
	{
		_numTicks=numTicks;
	}
	///Implements drawing code
	/**
	\param dc Device context
	*/
	void OnDraw(DC *dc)
	{
		dc->SetFont(SmallFont);
		float range=_maxValue-_minValue;
		float step_val=(range)/(_numTicks-1);
		float factor=1000/range;
		int radius=Height()*1;
		int x0=Width()/2;
		int y0=Height()*1.4;
		if(!_drawOnlyPointer)
		{
			dc->DrawRoundRect(2,2,Width()-2,Height()-2);
			dc->Sector(x0,y0,radius,_sector_angle_rad);
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
				x_offset=AHelper::GetNumberLength(value,1);
				if(value<0)
					x_offset++;
				if(i==0)
					x_offset=0;
				else if(i == _numTicks-1)
					x_offset=-(x_offset-0.5)*dc->FontWidth();
				else
					x_offset=-x_offset/2*dc->FontWidth();
				dc->DrawNumber(value,1,x0+radius*1.1*sin_val+x_offset,y0-radius*1.1*cos_val-(dc->FontHeight()*(i==0||i==_numTicks-1?1.5:1)));
			}
		}
		dc->SetColor(_fillColor);
		DrawPointer(_oldValue,x0,y0,radius,dc);
		dc->SetColor(Color::Red);
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