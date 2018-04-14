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

#include "DC.h"
#include "Adafruit_GFX.h"

class AFontGfx : public AFont
{
public:
	const GFXfont *Font;
	AFontGfx(const __FlashStringHelper * fontName, const GFXfont *font, BaseLinePosition baseLinePosition = AFont::BaselineTop) : AFont(fontName, baseLinePosition), Font(font)
	{

	}
};

class DC_Gfx : public DC
{
protected:
	uint16_t _color;
	Adafruit_GFX * _display;
public:
	///Constructor that used locally. This constructor assumes that UTFT library is initialized already
	DC_Gfx(Adafruit_GFX *oled) :DC()
	{
		_display = oled;
	}
	///Returns screen orientation vertical or horisontal
	ScreenOrientationType ScreenOrientation()
	{
		return Landscape;
	}
	///Returns screen width
	int DeviceWidth()
	{
		return _display->width();
	}
	///Returns screen height
	int DeviceHeight()
	{
		return _display->height();
	}
	///Fills rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRect(int left, int top, int right, int bottom)
	{
		_display->fillRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, _color);
	}
	void setColor(byte r, byte g, byte b)
	{
		_color = Color::GetColor565(r,g,b);
	}
	void drawHLine(int x, int y, int l)
	{
		_display->drawFastHLine(x, y, l, _color);
	}
	///Draws rectangle. Input coordinates have to be defined in the window coordinate system
	void Rectangle(int left, int top, int right, int bottom)
	{
		_display->drawRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, _color);
	}
	///Fills rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRoundRect(int left, int top, int right, int bottom)
	{
		_display->fillRoundRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, 3, _color);
	}
	///Draws rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void DrawRoundRect(int left, int top, int right, int bottom)
	{
		_display->drawRoundRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, 3, _color);
	}
	///Draws circle. Input coordinates have to be defined in the window coordinate system
	void FillCircle(int x0, int y0, int radius)
	{
		_display->fillCircle(ToDC_X(x0), ToDC_Y(y0), radius, _color);
	}
#if !defined(ESP8266) && !defined(ESP32)
	///Draws PROGMEM string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const __FlashStringHelper * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		x = ToDC_X(x);
		y = ToDC_Y(y);
		int16_t x1, y1;
		uint16_t w, h;
		_display->getTextBounds(text, (int16_t)x, (int16_t)y, &x1, &y1, &w, &h);
		if (aligment == HorizontalAlignment::Right)
			x = (x + width - w);
		else if (aligment == HorizontalAlignment::Center)
			x = (x + width / 2 - w / 2);
		_display->setTextColor(_color);
		_display->setCursor(x, y);
		_display->println(text);
	}
#endif
	///Draws string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const char * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		x = ToDC_X(x);
		y = ToDC_Y(y);
		int16_t x1, y1;
		uint16_t w, h;
		_display->getTextBounds((char *)text, (int16_t)x, (int16_t)y, &x1, &y1, &w, &h);
		if (aligment == HorizontalAlignment::Right)
			x = (x + width - w);
		else if (aligment == HorizontalAlignment::Center)
			x = (x + width / 2 - w / 2);
		_display->setTextColor(_color);
		if(GetCurrentFont() != NULL && GetCurrentFont()->BaselinePosition == AFont::BaselineBottom)
			_display->setCursor(x, y + h);
		else
			_display->setCursor(x, y);
		_display->print(text); 
	}

	///Returns symbol width for the current font 
	int FontWidth()
	{
		int16_t x1, y1;
		uint16_t w, h;
		_display->getTextBounds((char *)"0", 0, 0, &x1, &y1, &w, &h);
		return w;
	}
	///Returns symbol height for the current font 
	int FontHeight()
	{
		int16_t x1, y1;
		uint16_t w, h;
		_display->getTextBounds((char *)"0", 0, 0, &x1, &y1, &w, &h);
		return h;
	}
	///Draws symbol. Input coordinates have to be defined in the screen system
	void DrawSymbol(const char c, int dc_x, int dc_y)
	{
		_display->setCursor(dc_x, dc_y);
		_display->write(c);
	}

	///Draw caret. Input coordinates have to be defined in the window coordinate system
	void DrawCaret(int pos, int x, int y)
	{
	}
	void drawPixel(int x, int y)
	{
		_display->drawPixel(x, y, _color);
	}
	void SetDeviceColor(Color color)
	{
		_color = color.GetColor565();
	}
	void SetBackColor(Color color)
	{
		_color = color.GetColor565();
	}
	void drawLine(int x1, int y1, int x2, int y2)
	{
		_display->drawLine(x1, y1, x2, y2,_color);
	}
};