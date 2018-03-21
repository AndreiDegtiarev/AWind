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
#include "Log.h"
#include "Color.h"

#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE
extern char *dtostrf(double val, signed char width, unsigned char prec, char *sout);
#endif

///Device context. Abstraction layer to the device specific drawing code. Coordinates in drawing function are in window coordinate system that internaly translated into screen coordinate system
class DC
{
public:
	enum HorizontalAlignment
	{
		Left,
		Center,
		Right
	};
private:
	int _offset_x; //!< Offset of coordinate system alon x axis
	int _offset_y;//!< Offset of coordinate system along y axis
	int _last_x;  //!< Last x coordinate. It is needed in MoveTo and LineTo functions
	int _last_y;  //!< Last x coordinate. It is needed in MoveTo and LineTo functions
	char _buffer[15]; //!< Internal buffer for numbers convertion into string
public:
	enum ScreenOrientationType
	{
		Landscape,
		Portrate,
	};
	///Constructor
	DC()
	{
		Reset();
	}
	///Returns screen orientation vertical or horisontal
	virtual ScreenOrientationType ScreenOrientation()=0;

	///Returns screen width
	virtual int DeviceWidth()=0;

	///Returns screen height
	virtual int DeviceHeight() = 0;

	///Resets device context into initial condition
	void Reset()
	{
		_offset_x = 0;
		_offset_y = 0;
		_last_x = 0;
		_last_y = 0;
	}
	///Initializes drawing coordinate system offset
	/**
	\param offset_x offset in x direction
	\param offset_y offset in y direction
	*/
	void Offset(int offset_x,int offset_y)
	{
		_offset_x+=offset_x;
		_offset_y+=offset_y;
	}
	///Converts x coordinate from window into screen coordinate system
	int ToDC_X(int x)
	{
		return x+_offset_x;
	}
	///Converts y coordinate from window into screen coordinate system
	int ToDC_Y(int y)
	{
		return y+_offset_y;
	}
	///Draws rectangle. Input coordinates have to be defined in the window coordinate system
	virtual void Rectangle(int left, int top, int right, int bottom) = 0;

	///Draws rectangle with 3D border. Input coordinates have to be defined in the window coordinate system
	void Rectangle3D(int left,int top,int right,int bottom,Color color1,Color color2)
	{
		SetColor(color2);
		MoveTo(left,bottom);
		LineTo(right,bottom);
		LineTo(right,top);
		SetColor(color1);
		LineTo(left,top);
		LineTo(left,bottom);
	}
	///Fills rectangle. Input coordinates have to be defined in the window coordinate system
	virtual void FillRect(int left, int top, int right, int bottom) = 0;
	virtual void setColor(byte r, byte g, byte b) = 0;
	virtual void drawHLine(int x, int y, int l)=0;
	///Fills rectangle with gradient color. Input coordinates have to be defined in the window coordinate system
	void FillGradientRect(int left,int top,int right,int bottom,Color color1,Color color2)
	{
		int start_x=ToDC_X(left);
		int start_y=ToDC_Y(top);
		int length_x=ToDC_X(right)-start_x;
		int length_y=ToDC_Y(bottom)-start_y;
		byte rgb[3];
		byte rgb1[3];
		byte rgb2[3];
		rgb1[0]=color1.GetR();
		rgb1[1]=color1.GetG();
		rgb1[2]=color1.GetB();
		rgb2[0]=color2.GetR();
		rgb2[1]=color2.GetG();
		rgb2[2]=color2.GetB();
		float fctr;
		for(int i=0;i<=length_y;i++)
		{
			fctr=(float)i/length_y;
			for(int j=0;j<3;j++)
			{
				rgb[j]=rgb1[j]+fctr*(rgb2[j]-rgb1[j]);
				//out<<rgb2[j]<<" ";
			}
			//out<<endln;
			setColor(rgb[0],rgb[1],rgb[2]);
			drawHLine (start_x,start_y+i,length_x);
		}
	}
	///Fills rounded rectangle. Input coordinates have to be defined in the window coordinate system
	virtual void FillRoundRect(int left,int top,int right,int bottom) = 0;
	///Draws rounded rectangle. Input coordinates have to be defined in the window coordinate system
	virtual void DrawRoundRect(int left,int top,int right,int bottom) = 0;
	///Draws circle. Input coordinates have to be defined in the window coordinate system
	virtual void FillCircle(int x0, int y0, int radius) = 0;

	///Draws integer number. Input coordinates have to be defined in the window coordinate system
	void DrawNumber(int number,int x,int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		sprintf(_buffer,"%d",number);
		DrawText(_buffer,x,y, aligment, width);
	}
	///Draws float number. Input coordinates have to be defined in the window coordinate system
	/**
	\param number float input value
	\param dec number decimal places
	*/
	void DrawNumber(float number,int dec,int x,int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		dtostrf(number,0,dec,_buffer);
		DrawText(_buffer,x,y, aligment, width);
	}
	///Draws PROGMEM string. Input coordinates have to be defined in the window coordinate system
	virtual void DrawText(const __FlashStringHelper * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0) = 0;

	///Returns symbol width for the current font 
	virtual int FontWidth() = 0;

	///Returns symbol jeight for the current font 
	virtual int FontHeight() = 0;

	///Draws symbol. Input coordinates have to be defined in the screen system
	virtual void DrawSymbol(const char c,int dc_x,int dc_y) = 0;

	///Draws a character. Input coordinates have to be defined in the window coordinate system
	void DrawChar(const char c,int x, int y)
	{
		x=ToDC_X(x);
		y=ToDC_Y(y);
		DrawSymbol(c, x, y);
	}
	///Draw caret. Input coordinates have to be defined in the window coordinate system
	virtual void DrawCaret(int pos, int x, int y) = 0;

	///Draws string. Input coordinates have to be defined in the window coordinate system
	virtual void DrawText(const char * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0) = 0;


	virtual void	drawPixel(int x, int y) = 0;

	///Draws sector. Input coordinates have to be defined in the window coordinate system
	void Sector(int x0, int y0, int radius,float start_angle_rad,float angle_rad) 
	{
		x0=ToDC_X(x0);
		y0=ToDC_Y(y0);
		int x = 0;
		int y = radius;
		float cos_factor = cos(-start_angle_rad);
		float sin_factor = sin(-start_angle_rad);
		float tan_stop=tan(angle_rad/2);
		int delta = 2 - 2 * radius;
		int error = 0;
		float ratio;
		float rot_x,rot_y;
		while(y >= 0) {
			ratio=x/(float)y;
			if(ratio>tan_stop)
				break;
			rot_x = x*cos_factor - y*sin_factor;
			rot_y = x*sin_factor + y*cos_factor;
			drawPixel(x0 + rot_x, y0 - rot_y);
			rot_x =-x*cos_factor - y*sin_factor;
			rot_y = -x*sin_factor + y*cos_factor;
			drawPixel(x0 + rot_x, y0 - rot_y);
			//_lcd->drawPixel(x0 + x, y0 - y);
			//_lcd->drawPixel(x0 - x, y0 - y);
			error = 2 * (delta + y) - 1;
			if(delta < 0 && error <= 0)
			{
				++x;
				delta += 2 * x + 1;
				continue;
			}
			error = 2 * (delta - x) - 1;
			if(delta > 0 && error > 0)
			{
				--y;
				delta += 1 - 2 * y;
				continue;
			}
			++x;
			delta += 2 * (x - y);
			--y;
		}
	}
	virtual void SetDeviceColor(Color color) = 0;


	void SetColor(Color color)
	{
		SetDeviceColor(color);
	}
	virtual void SetBackColor(Color color) = 0;

	virtual void SetFont(uint8_t *font) = 0;

	void MoveTo(int x,int y)
	{ 
		_last_x=ToDC_X(x);
		_last_y=ToDC_Y(y);
	}
	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
	void LineTo(int x,int y)
	{ 
		drawLine(_last_x,_last_y,ToDC_X(x),ToDC_Y(y));
		_last_x=ToDC_X(x);
		_last_y=ToDC_Y(y);
	}
	void Line(int x1,int y1,int x2,int y2)
	{
		MoveTo(x1,y1);
		LineTo(x2,y2);
	}
	virtual void Display()
	{

	}
};