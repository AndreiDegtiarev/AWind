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
#include "AHelper.h"
///Decorator primitive that sets current color
///Overriden members description see Decorator class documentation
class DecoratorColor : public Decorator
{
	Color _color;
public:
	///Constructor
	/**
	\param current color
	*/
	DecoratorColor(Color color):_color(color)
	{

	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		dc->SetColor(_color);
	}
};
///Decorator primitive for raund rect filled area
///Overriden members description see Decorator class documentation
class DecoratorRectFill : public Decorator
{
	Color _color;
	bool _isRound;
public:
	DecoratorRectFill(Color color,bool isRound=true):_color(color),_isRound(isRound)
	{

	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		dc->SetBackColor(_color);
		if(_isRound)
			dc->FillRoundRect (left, top, left+width, top+height);
		else
			dc->FillRect (left, top, left+width, top+height);
	}
};
///Decorator primitive for raund rectangle
///Overriden members description see Decorator class documentation
class DecoratorRaundRect : public Decorator
{
	Color _color;
public:
	DecoratorRaundRect(Color color):_color(color)
	{

	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		dc->SetColor(_color);
		dc->DrawRoundRect (left, top, width+left, height+top);
	}
};
///Decorator primitive for 3D rectangle
///Overriden members description see Decorator class documentation
class Decorator3DRect : public Decorator
{
	Color _color1;
	Color _color2;
public:
	Decorator3DRect(Color color1,Color color2):_color1(color1),_color2(color2)
	{

	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		dc->Rectangle3D(left, top, left+width, top+height,_color1,_color2);
	}
};
///Decorator primitive for 3D rectangle
///Overriden members description see Decorator class documentation
class DecoratorAxis : public Decorator
{
public:
	enum Orientation
	{
		HorizontalTop,
		HorizontalBottom,
		VerticalLeft,
		VerticalRight
	};
private:
	Orientation _orientation;
	uint8_t *_font;
	int _offsetX;
	int _offsetY;
	int _length;
	float _minValue;
	float _maxValue;
	int _numTicks;
	static const int _tick_length=5;
public:
	DecoratorAxis(Orientation orientation,uint8_t *font,int length,float minValue,float maxValue,int nTicks): 
	                                                                             _orientation(orientation),
																				 _font(font),
																				 _offsetX(0),
																				 _offsetY(0),
																				 _length(length),
																				 _minValue(minValue),
																				 _maxValue(maxValue),
																				 _numTicks(nTicks)
	{

	}
	void SetOffset(int offsetX,int offsetY)
	{
		_offsetX=offsetX;
		_offsetY=offsetY;
	}
	void GetMinMax(float &minVal,float &maxVal)
	{
		minVal=_minValue;
		maxVal=_maxValue;
	}
	int EstimateWidth(DC *dc)
	{
		dc->SetFont(_font);
		switch(_orientation)
		{
		case VerticalRight:
		case VerticalLeft:
			return (AHelper::GetNumberLength(max(_minValue,_maxValue),1)*dc->FontWidth()+_tick_length);
		case HorizontalTop:
		case HorizontalBottom:
			return _length;
		}
	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		dc->SetFont(_font);
		float range=_maxValue-_minValue;
		float step_val=(range)/(_numTicks-1);
		switch(_orientation)
		{
		case VerticalLeft:
		case VerticalRight:
			{
				int y;
				int scale_step=(_length+_offsetY)/(_numTicks-1);
				int est_width=EstimateWidth(dc);
				for(int i=0;i<_numTicks;i++)
				{
					y=_length+_offsetY-scale_step*i;
					float value=_minValue+i*step_val;
					if(_orientation == VerticalRight)
					{
						dc->MoveTo(_offsetX,y);
						dc->LineTo(_offsetX+_tick_length,y);
						dc->DrawNumber(value,1,_offsetX+_tick_length,y-(dc->FontHeight()*(i==_numTicks-1?0:1)));
					}
					else
					{
						dc->MoveTo(_offsetX+est_width,y);
						dc->LineTo(_offsetX+est_width-_tick_length,y);
						int val_width=AHelper::GetNumberLength(value,1)*dc->FontWidth();
						dc->DrawNumber(value,1,_offsetX+est_width-val_width,y-(dc->FontHeight()*(i==_numTicks-1?0:1)));
					}
				}
			}
		}
	}
};