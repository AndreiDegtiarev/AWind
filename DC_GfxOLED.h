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

#include "DC_GFX.h"

class AFontOLED : public AFont
{
public:
	const uint8_t Size;
	AFontOLED(const __FlashStringHelper * fontName, uint8_t size) : AFont(fontName), Size(size)
	{

	}
};

class DC_GfxOLED : public DC_Gfx
{
private:
	void(*_fncDisplay)();
public:
	///Constructor that used locally. This constructor assumes that UTFT library is initialized already
	DC_GfxOLED(Adafruit_GFX *oled, void(*fncDisplay)()) :DC_Gfx(oled)
	{
		_fncDisplay = fncDisplay;
	}

	void setColor(byte r, byte g, byte b)
	{
		_color = Color::GetColor565(r,g,b)>565 / 2 ? 1 : 0;
	}
	void SetDeviceColor(Color color)
	{
		_color = color.GetColor565()>565 / 2 ? 1 : 0;
	}
	void SetBackColor(Color color)
	{
		_color = color.GetColor565()>565 / 2 ? 1 : 0;
	}
	///Set active font
	virtual void SetFontImpl(AFont *font)
	{
		if (font != NULL)
			SetFont(((AFontOLED *)font)->Size);
		else 
			SetFont(NULL);
	}
	void SetFont(uint8_t size)
	{
		_display->setTextSize(size);
	}
	virtual void Display()
	{
		if(_fncDisplay != NULL)
			_fncDisplay();
	}
	static void RegisterDefaultFonts()
	{
		Environment::Get()->RegisterFont(new AFontOLED(F("Big"), 2));
		Environment::Get()->RegisterFont(new AFontOLED(F("Small"), 1));
	}
};