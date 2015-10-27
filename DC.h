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
#include "UTFT.h"
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE
extern char *dtostrf(double val, signed char width, unsigned char prec, char *sout);
#endif
///Device context. Abstraction layer to the device specific drawing code. Coordinates in drawing function are in window coordinate system that internaly translated into screen coordinate system
class DC
{
	UTFT *_lcd;  //!< Pointer to the UTFT class
	int _offset_x; //!< Offset of coordinate system alon x axis
	int _offset_y;//!< Offset of coordinate system along y axis
	int _last_x;  //!< Last x coordinate. It is needed in MoveTo and LineTo functions
	int _last_y;  //!< Last x coordinate. It is needed in MoveTo and LineTo functions
	char _buffer[15]; //!< Internal buffer for numbers convertion into string
public:
	///Constructor for global context, that created only once in WindowsManager
	DC(UTFT *lcd)
	{
		_lcd=lcd;
		Reset();
	}
	///Constructor that used locally. This constructor assumes that UTFT library is initialized already
	DC()
	{
		extern UTFT *globalLcd;
		_lcd=globalLcd;
		Reset();
	}
	///Returns screen width
	int DeviceWidth()
	{
		return _lcd->getDisplayXSize()-1;
	}
	///Returns screen height
	int DeviceHeight()
	{
		return _lcd->getDisplayYSize()-1;
	}
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
	void Rectangle(int left,int top,int right,int bottom)
	{
		_lcd->drawRect(ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
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
	void FillRect(int left,int top,int right,int bottom)
	{
		_lcd->fillRect (ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
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
			_lcd->setColor(rgb[0],rgb[1],rgb[2]);
			_lcd->drawHLine (start_x,start_y+i,length_x);
		}
	}
	///Fills rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRoundRect(int left,int top,int right,int bottom)
	{
		_lcd->fillRoundRect (ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
	///Draws rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void DrawRoundRect(int left,int top,int right,int bottom)
	{
		_lcd->drawRoundRect (ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
	///Draws integer number. Input coordinates have to be defined in the window coordinate system
	void DrawNumber(int number,int x,int y)
	{
		sprintf(_buffer,"%d",number);
		DrawText(_buffer,x,y);
	}
	///Draws float number. Input coordinates have to be defined in the window coordinate system
	/**
	\param number float input value
	\param dec number decimal places
	*/
	void DrawNumber(float number,int dec,int x,int y)
	{
		dtostrf(number,0,dec,_buffer);
		DrawText(_buffer,x,y);
	}
	///Draws PROGMEM string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const __FlashStringHelper * text,int x,int y)
	{
		x=ToDC_X(x);
		y=ToDC_Y(y);
		int stl, i;

		stl = strlen_P((const char PROGMEM *)text);
		if (_lcd->orient==PORTRAIT)
		{
			if (x==RIGHT)
				x=(_lcd->disp_x_size+1)-(stl*_lcd->cfont.x_size);
			if (x==CENTER)
				x=((_lcd->disp_x_size+1)-(stl*_lcd->cfont.x_size))/2;
		}
		else
		{
			if (x==RIGHT)
				x=(_lcd->disp_y_size+1)-(stl*_lcd->cfont.x_size);
			if (x==CENTER)
				x=((_lcd->disp_y_size+1)-(stl*_lcd->cfont.x_size))/2;
		}
		for (i=0; i<stl; i++)
		{
			unsigned char c = pgm_read_byte(&((const char PROGMEM *)text)[i]);
			DrawSymbol(c, x + (i*(_lcd->cfont.x_size)), y);
		}

	}
	///Returns symbol width for the current font 
	int FontWidth()
	{
		return _lcd->cfont.x_size;
	}
	///Returns symbol jeight for the current font 
	int FontHeight()
	{
		return _lcd->cfont.y_size;
	}
	///Draws symbol. Input coordinates have to be defined in the screen system
	void DrawSymbol(const char c,int dc_x,int dc_y)
	{
		_lcd->printChar(c, dc_x, dc_y);
	}
	///Draws a character. Input coordinates have to be defined in the window coordinate system
	void DrawChar(const char c,int x, int y)
	{
		x=ToDC_X(x);
		y=ToDC_Y(y);
		DrawSymbol(c, x, y);
	}
	///Draw caret. Input coordinates have to be defined in the window coordinate system
	void DrawCaret(int pos, int x, int y)
	{
		x = x+(pos*_lcd->cfont.x_size);
		DrawChar('_', x, y+2);
	}
	///Draws string. Input coordinates have to be defined in the window coordinate system
	///Draws string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const char * text,int x,int y)
	{
		x=ToDC_X(x);
		y=ToDC_Y(y);
		//_lcd->print(text,x,y);
		//out<<text<<" "<<strlen(text)<<endln;
		for(int i=0;i<strlen(text);i++)
		{
			char c=text[i];
			//if(c==' ')
			//	break;
			DrawSymbol(c, x + (i*(_lcd->cfont.x_size)), y);
		}
	}
	///Draws sector. Input coordinates have to be defined in the window coordinate system
	void Sector(int x0, int y0, int radius,float angle_rad) 
	{
		x0=ToDC_X(x0);
		y0=ToDC_Y(y0);
		int x = 0;
		int y = radius;
		float tan_stop=tan(angle_rad/2);
		int delta = 2 - 2 * radius;
		int error = 0;
		float ratio;
		while(y >= 0) {
			ratio=x/(float)y;
			//_lcd->drawPixel(x0 + x, y0 + y);
			if(ratio>tan_stop)
				break;
			_lcd->drawPixel(x0 + x, y0 - y);
			//_lcd->drawPixel(x0 - x, y0 + y);
			_lcd->drawPixel(x0 - x, y0 - y);
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
	void SetDeviceColor(Color color)
	{
		_lcd->setColor(color.GetR(),color.GetG(),color.GetB());
	}

	void SetColor(Color color)
	{
		SetDeviceColor(color);
	}
	void SetBackColor(Color color)
	{
		_lcd->setBackColor(VGA_TRANSPARENT);
		SetDeviceColor(color);
	}
	void SetFont(uint8_t *font)
	{
		_lcd->setFont(font);
	}
	void MoveTo(int x,int y)
	{ 
		_last_x=ToDC_X(x);
		_last_y=ToDC_Y(y);
	}
	void LineTo(int x,int y)
	{ 
		_lcd->drawLine(_last_x,_last_y,ToDC_X(x),ToDC_Y(y));
		_last_x=ToDC_X(x);
		_last_y=ToDC_Y(y);
	}
	void Line(int x1,int y1,int x2,int y2)
	{
		MoveTo(x1,y1);
		LineTo(x2,y2);
	}
};