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
#include "Adafruit_SSD1306.h"


class DC_GfxOLED : public DC
{
private:
	uint16_t _color;
	Adafruit_SSD1306 * _oled;
public:
	///Constructor that used locally. This constructor assumes that UTFT library is initialized already
	DC_GfxOLED(Adafruit_SSD1306 *oled) :DC()
	{
		_oled = oled;
	}
	///Returns screen orientation vertical or horisontal
	ScreenOrientationType ScreenOrientation()
	{
		return Landscape;
	}
	///Returns screen width
	int DeviceWidth()
	{
		return _oled->width();
	}
	///Returns screen height
	int DeviceHeight()
	{
		return _oled->height();
	}
	///Fills rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRect(int left, int top, int right, int bottom)
	{
		_oled->fillRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, _color);
	}
	void setColor(byte r, byte g, byte b)
	{
		_color = Color::GetColor565(r,g,b)>565 / 2 ? WHITE : BLACK;;
	}
	void drawHLine(int x, int y, int l)
	{
		_oled->drawFastHLine(x, y, l, _color);
	}
	///Draws rectangle. Input coordinates have to be defined in the window coordinate system
	void Rectangle(int left, int top, int right, int bottom)
	{
		_oled->drawRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, _color);
	}
	///Fills rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRoundRect(int left, int top, int right, int bottom)
	{
		_oled->fillRoundRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, 3, _color);
	}
	///Draws rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void DrawRoundRect(int left, int top, int right, int bottom)
	{
		_oled->drawRoundRect(ToDC_X(left), ToDC_Y(top), right - left, bottom - top, 3, _color);
	}
	///Draws circle. Input coordinates have to be defined in the window coordinate system
	void FillCircle(int x0, int y0, int radius)
	{
		_oled->fillCircle(ToDC_X(x0), ToDC_Y(y0), radius, _color);
	}
	///Draws PROGMEM string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const __FlashStringHelper * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		x = ToDC_X(x);
		y = ToDC_Y(y);
		int x1, y1, w, h;
		_oled->getTextBounds(text, x, y, &x1, &y1, &w, &h);
		if (aligment == HorizontalAlignment::Right)
			x = (x + width - w);
		else if (aligment == HorizontalAlignment::Center)
			x = (x + width / 2 - w / 2);
		_oled->setTextColor(_color);
		_oled->setCursor(x, y);
		_oled->println(text);
	}
	///Draws string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const char * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		x = ToDC_X(x);
		y = ToDC_Y(y);
		int x1, y1, w, h;
		_oled->getTextBounds(text, x, y, &x1, &y1, &w, &h);
		if (aligment == HorizontalAlignment::Right)
			x = (x + width - w);
		else if (aligment == HorizontalAlignment::Center)
			x = (x + width / 2 - w / 2);
		_oled->setTextColor(_color);
		_oled->setCursor(x, y);
		_oled->println(text); 
	}

	///Returns symbol width for the current font 
	int FontWidth()
	{
		return 10;
	}
	///Returns symbol jeight for the current font 
	int FontHeight()
	{
		return 10;
	}
	///Draws symbol. Input coordinates have to be defined in the screen system
	void DrawSymbol(const char c, int dc_x, int dc_y)
	{
		_oled->setCursor(dc_x, dc_y);
		_oled->write(c);
	}

	///Draw caret. Input coordinates have to be defined in the window coordinate system
	void DrawCaret(int pos, int x, int y)
	{
	}
	void drawPixel(int x, int y)
	{
		_oled->drawPixel(x, y, _color);
	}
	void SetDeviceColor(Color color)
	{
		_color = color.GetColor565()>565/2?WHITE:BLACK;
	}
	void SetBackColor(Color color)
	{
		_color = color.GetColor565()>565 / 2 ? WHITE : BLACK;
	}
	void SetFont(uint8_t *font)
	{
		if(font[0] == 1)
			_oled->setTextSize(font[0]);
		else
			_oled->setTextSize(2);
	}
	void drawLine(int x1, int y1, int x2, int y2)
	{
		_oled->drawLine(x1, y1, x2, y2,_color);
	}
	virtual void Display()
	{
		_oled->display();
	}
};