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
//#include "AHelper.h"
///Decorator primitive that sets current color. Overriden members description see Decorator class documentation
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
///Decorator primitive for boundary lines. Overriden members description see Decorator class documentation
class DecoratorBoundaryLine : public Decorator
{
public:
	enum Position
	{
		Left,
		Top,
		Right,
		Bottom,
	};
private:
	Position _position;
public:
	DecoratorBoundaryLine(Position position) :_position(position)
	{

	}
	void Draw(DC *dc, int left, int top, int width, int height)
	{
		switch (_position)
		{
		case Left:
			dc->Line(0, 0,0, height);
			break;
		case Right:
			dc->Line(width, 0, width, height);
			break;
		case Top:
			dc->Line(0, 0, width, 0);
			break;
		case Bottom:
			dc->Line(0, height, width, height);
			break;
		}
	}
};
///Decorator primitive for round rect filled area. Overriden members description see Decorator class documentation
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
///Decorator primitive for round rect filled area. Overriden members description see Decorator class documentation
class DecoratorRectGradientFill : public Decorator
{
	Color _color1;
	Color _color2;
public:
	DecoratorRectGradientFill(Color color1,Color color2):_color1(color1),_color2(color2)
	{

	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		//dc->FillGradientRect (left, top, left+width, top+height/2,_color1,_color2);
		//dc->FillGradientRect (left, top+height/2, left+width, top+height,_color2,_color1);
		dc->FillGradientRect (left, top, left+width, top+height,_color2,_color1);
	}
};
///Decorator primitive for round rectangle. Overriden members description see Decorator class documentation
class DecoratorRoundRect : public Decorator
{
	Color _color;
public:
	DecoratorRoundRect(Color color):_color(color)
	{

	}
	void Draw(DC *dc,int left,int top,int width,int height)
	{
		dc->SetColor(_color);
		dc->DrawRoundRect (left, top, width+left, height+top);
	}
};
///Decorator primitive for 3D rectangle. Overriden members description see Decorator class documentation
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

///Decorator primitive for 3D Circle. Overriden members description see Decorator class documentation
class Decorator3DSquare : public Decorator
{
	Color _color1;
	Color _color2;
	Color _colorBack;
	bool  _withCircle;
public:
	Decorator3DSquare(Color color1, Color color2, Color colorBack,bool withCircle) :_color1(color1), _color2(color2), _colorBack(colorBack), _withCircle(withCircle)
	{

	}
	void Draw(DC *dc, int left, int top, int width, int height)
	{
		dc->SetColor(_colorBack);
		int size = 15;
		int radius = size / 2;
		int x0 = left + radius;
		int y0 = top + height / 2;
		dc->FillRect(x0- radius, y0- radius, x0 + radius, y0+ radius);
		dc->Rectangle3D(x0 - radius, y0 - radius, x0 + radius, y0 + radius,_color1, _color2);
		if (_withCircle)
		{
			dc->SetColor(Color::Black);
			dc->FillCircle(x0,y0, radius-2);
		}
	}
};
///Axis decorator primitive. It is shared between gauge and chart objects. Overriden members description see Decorator class documentation
class DecoratorAxis : public Decorator
{
public:
	enum Orientation
	{
		HorizontalTop,     //!<Not implemented yet
		HorizontalBottom, //!<Not implemented yet
		VerticalLeft,    //!<Vertcal axis with the labels left to the axis
		VerticalRight   //!<Vertcal axis with the labels right to the axis
	};
private:
	Orientation _orientation; //!< Axis orientation
	AFont *_font;            //!< Labels font
	int _offsetX;           //!< Horizontal offset of decorator relative to the parent window
	int _offsetY;          //!< Vertical offset of decorator relative to the parent window
	int _length;          //!< Length of axis
	float _minValue;     //!< Min value
	float _maxValue;    //!< Max value
	int _numTicks;     //!< Number of short line perpendicular to the axis
	static const int _tick_length=5;
public:
///Constructor
/**
\param orientation axis orientation
\param font labels font
\param length length of axis
\param minValue min value
\param maxValue max value
\param nTicks number of short line perpendicular to the axis
*/
	DecoratorAxis(Orientation orientation,AFont *font,int length,float minValue,float maxValue,int nTicks): 
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
	///Sets decorator offset in the parent window coordinate system
	void SetOffset(int offsetX,int offsetY)
	{
		_offsetX=offsetX;
		_offsetY=offsetY;
	}
	///Return min and max label values
	void GetMinMax(float &minVal,float &maxVal)
	{
		minVal=_minValue;
		maxVal=_maxValue;
	}
	///Sets min and max for label values
	void SetMinMax(float minVal,float maxVal)
	{
		_minValue=minVal;
		_maxValue=maxVal;
	}
	///Returns axis length
	int GetLength()
	{
		return _length;
	}
	//Returns axis orientation
	Orientation Orientation()
	{
		return _orientation;
	}
	int EstimateLeft(DC *dc)
	{
		return _offsetX;
	}
	int EstimateRight(DC *dc)
	{
		dc->SetFont(_font);
		switch(_orientation)
		{
		case VerticalRight:
		case VerticalLeft:
			return (AHelper::GetNumberLength(max(_minValue,_maxValue),1)*dc->FontWidth()+_tick_length)+_offsetX;
		case HorizontalTop:
		case HorizontalBottom:
			return _length+_offsetX;
		}
	}
	int EstimateTop(DC *dc)
	{
		return _offsetY;
	}
	int EstimateBottom(DC *dc)
	{
		dc->SetFont(_font);
		switch(_orientation)
		{
		case VerticalRight:
		case VerticalLeft:
			return _length+_offsetY;
		case HorizontalTop:
		case HorizontalBottom:
			return _offsetY+dc->FontHeight()+_tick_length;
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
				float scale_step=_length/(_numTicks-1.0);
				int axis_right=EstimateRight(dc);
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
						dc->MoveTo(axis_right,y);
						dc->LineTo(axis_right-_tick_length,y);
						int val_width=AHelper::GetNumberLength(value,1)*dc->FontWidth();
						dc->DrawNumber(value,1,axis_right-val_width,y-(dc->FontHeight()*(i==_numTicks-1?0:1)));
					}
				}
			}
		}
	}
};