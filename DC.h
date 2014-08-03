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
#include "Color.h"
class DC
{
	UTFT *_lcd;
	int _offset_x;
	int _offset_y;
	int _last_x;
	int _last_y;
	char _buffer[15];
public:
	DC(UTFT *lcd)
	{
		_lcd=lcd;
		Reset();
	}
	DC()
	{
		extern UTFT *globalLcd;
		_lcd=globalLcd;
		Reset();
	}
	int DeviceWidth()
	{
		return _lcd->getDisplayXSize()-1;
	}
	int DeviceHeight()
	{
		return _lcd->getDisplayYSize()-1;
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
		_offset_x+=offset_x;
		_offset_y+=offset_y;
	}
	int ToDC_X(int x)
	{
		return x+_offset_x;
	}
	int ToDC_Y(int y)
	{
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
		sprintf(_buffer,"%d",number);
		DrawText(_buffer,x,y);
	}
	void DrawNumber(float number,int dec,int x,int y)
	{
		dtostrf(number,0,dec,_buffer);
		DrawText(_buffer,x,y);
	}
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
	int FontWidth()
	{
		return _lcd->cfont.x_size;
	}
	int FontHeight()
	{
		return _lcd->cfont.y_size;
	}
	void DrawSymbol(const char c,int dc_x,int dc_y)
	{
		_lcd->printChar(c, dc_x, dc_y);
	}
	void DrawText(const char * text,int x,int y)
	{
		x=ToDC_X(x);
		y=ToDC_Y(y);
		//_lcd->print(text,x,y);
		//out<<text<<" "<<strlen(text)<<endl;
		for(int i=0;i<strlen(text);i++)
		{
			char c=text[i];
			if(c==' ')
				break;
			DrawSymbol(c, x + (i*(_lcd->cfont.x_size)), y);
		}
	}
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