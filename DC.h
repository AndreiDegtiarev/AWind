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
#pragma once
#include "Color.h"
class DC
{
	UTFT *_lcd;
	int _offset_x;
	int _offset_y;
	int _last_x;
	int _last_y;
public:
	DC(UTFT *lcd)
	{
		_lcd=lcd;
		Reset();
	}
	void Reset()
	{
		_offset_x = 0;
		_offset_y = 0;
		_last_x = 0;
		_last_y = 0;
	}
	void Offset(int offset_x,int offset_y)
	{
		//Log::Number("Offset x: ",offset_x);
		//Log::Number(" y: ",offset_y,true);
		_offset_x+=offset_x;
		_offset_y+=offset_y;
	}
	int ToDC_X(int x)
	{
		//Log::Number("ToDC_X x: ",x);
		//Log::Number(" offset: ",_offset_x,true);
		return x+_offset_x;
	}
	int ToDC_Y(int y)
	{
		//Log::Number("ToDC_Y y: ",y);
		//Log::Number(" offset: ",_offset_y,true);
		return y+_offset_y;
	}
	void Rectangle(int left,int top,int right,int bottom)
	{
		_lcd->drawRect(ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
	void FillRoundRect(int left,int top,int right,int bottom)
	{
		_lcd->fillRoundRect (ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
	void DrawRoundRect(int left,int top,int right,int bottom)
	{
		_lcd->drawRoundRect (ToDC_X(left),ToDC_Y(top),ToDC_X(right),ToDC_Y(bottom));
	}
	void DrawNumber(int number,int x,int y)
	{
		_lcd->printNumI(number,ToDC_X(x),ToDC_Y(y));
	}
	void DrawNumber(float number,int dec,int x,int y)
	{
		if(dec == 0)
			_lcd->printNumI(number,ToDC_X(x),ToDC_Y(y));
		else
			_lcd->printNumF(number,dec,ToDC_X(x),ToDC_Y(y));
	}
	void DrawText(const __FlashStringHelper * _text,int x,int y)
	{
		_lcd->print(_text,ToDC_X(x),ToDC_Y(y));
	}
	void DrawText(const char * _text,int x,int y)
	{
		_lcd->print(_text,ToDC_X(x),ToDC_Y(y));
	}
	void SetColor(Color color)
	{
		/*Serial.print(color.GetRed());Serial.print(" ");
		Serial.print(color.GetGreen());Serial.print(" ");
		Serial.println(color.GetBlue());*/
		//lcd->setColor(color.GetValue());
		_lcd->setColor(color.GetR(),color.GetG(),color.GetB());
	}
	void SetBackColor(Color color)
	{
			_lcd->setBackColor(VGA_TRANSPARENT);
			SetColor(color);
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