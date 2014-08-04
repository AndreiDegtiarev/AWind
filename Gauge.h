#pragma once 
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
#include "Window.h"
#include "AHelper.h"
///Gauge window class, that allows visualisaion of data in form of bar or radial pointer
class Gauge : public Window
{
public:
	enum GaugeType
	{
		Bar, 
		RadialPointer,
	};
	int _numTicks; //!< Number of ticks (lines) on gauge scale 
	Color _color;  //!< Gauge color 
	GaugeType  _gaugeType; //!< Gauge appearance
	float _value;     //!< Current gauge value
	float _oldValue;  //!< Previos gauge value
	float _minValue;  //!< Gauge min limit
	float _maxValue;  //!< Gauge max limit
	bool  _drawOnlyPointer; //!< Defines whether only pointer (not scale has to be drawn)

	static const float _sector_angle_rad=3.14/2*0.8;

	///Constructor
	/**
	\param gaugeType defines gauge appearance
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	Gauge(GaugeType gaugeType,int left,int top,int width,int height)
		:Window(F("gauge"),left,top,width,height),
		_gaugeType(gaugeType),
		_numTicks(5),
		_value(0),
		_oldValue(0),
		_minValue(0),
		_maxValue(100),
		_color(Color::Green),
		_drawOnlyPointer(false)
	{
	}
	///Sets base gauge color
	void SetColor(Color color)
	{
		_color=color;
	}
	///Sets number of ticks
	/**
	\param _numTicks number of short lines on scale
	*/
	void SetNumberOfTicks(int numTicks)
	{
		_numTicks=numTicks;
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
		_value=min(_maxValue,value);
		_drawOnlyPointer=true;
		DC dc;
		PrepareDC(&dc);
		OnDraw(&dc);
		_oldValue=_value;
		_drawOnlyPointer=false;
	}
	///Implements drawing code
	/**
	\param dc Device context
	*/
	void OnDraw(DC *dc)
	{
		dc->SetColor(_color);
		dc->SetFont(SmallFont);
		float range=_maxValue-_minValue;
		float step_val=(range)/(_numTicks-1);
		switch(_gaugeType)
		{
		case Bar:
		{
			const int tick_length=5;
			int right_offset=Width()-(AHelper::GetNumberLength(max(_minValue,_maxValue),1)*dc->FontWidth()+dc->FontWidth()+tick_length);
			if(!_drawOnlyPointer)
			{
				dc->DrawRoundRect(0,0,right_offset,Height());
				int y;
				int scale_step=Height()/(_numTicks-1);
				for(int i=0;i<_numTicks;i++)
				{
					y=Height()-scale_step*i;
					dc->MoveTo(right_offset,y);
					dc->LineTo(right_offset+tick_length,y);
					dc->DrawNumber(_minValue+i*step_val,1,right_offset+tick_length+dc->FontWidth(),y-(dc->FontHeight()*(i==_numTicks-1?0:1)));
				}
			}
			if(_oldValue>_value)
			{
				dc->SetColor(GetBackColor());
				dc->FillRoundRect(2,1,right_offset-2,min(Height()-2,Height()-_value*Height()/range+2));
			}
			dc->SetColor(Color::Red);
			dc->FillRoundRect(2,Height()-_value*Height()/range,right_offset-2,Height()-1);
			break;
		}
		case RadialPointer:
		{
			float factor=1000/range;
			int radius=Height()*1;
			int x0=Width()/2;
			int y0=Height()*1.4;
			if(!_drawOnlyPointer)
			{
				dc->DrawRoundRect(0,0,Width(),Height());
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
			DrawPointer(_oldValue,x0,y0,radius,dc,GetBackColor());
			DrawPointer(_value,x0,y0,radius,dc,_color);
			break;
		}
		}
	}
private:
	///Draws pointer in RadialPointer gauge
	void DrawPointer(float value,int x0,int y0,int radius,DC *dc,Color color)
	{
		dc->SetColor(color);
		float dc_val=1000/(_maxValue-_minValue)*(value-_minValue);
		float angle=-_sector_angle_rad/2.0+_sector_angle_rad/1000.0*dc_val;
		float sin_val=sin(angle);
		float cos_val=cos(angle);
		dc->MoveTo(x0+radius*0.95*sin_val,y0-radius*0.95*cos_val);
		dc->LineTo(x0+radius*0.7*sin_val,y0-radius*0.7*cos_val);

	}
};