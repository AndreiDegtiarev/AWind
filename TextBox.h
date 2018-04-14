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
#pragma once
#include "Window.h"
#include "IContentChangedEventReceiver.h"


///Base class for window with text content
class TextBox : public Window
{
	AFont *_font;
protected:
	int _offset_x;
	int _offset_y;
	DC::HorizontalAlignment _horizontal_alignment;
	IContentChangedEventReceiver *_changedEvent;

	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	TextBox(int left,int top,int width,int height):Window(F("text"),left,top,width,height)
	{
		_font = Environment::Get()->FindFont(F("Small"));
		_offset_x=0;
		_offset_y=0;
		_horizontal_alignment = DC::Left;
		_changedEvent=NULL;
	}
	///Implements drawing code
	void OnDraw(DC *dc)
	{
		dc->SetFont(_font);
	}
public:
	///Application need to call this function if it wants receive notification about this window content changing
	void RegisterContentChangedReceiver(IContentChangedEventReceiver *event)
	{
		_changedEvent=event;
	}
	///
	void SetHorizontalAlignment(DC::HorizontalAlignment alignment)
	{
		_horizontal_alignment = alignment;
	}
	///Defines offset from left and top for text
	void SetMargins(int offset_x,int offset_y)
	{
		_offset_x=offset_x;
		_offset_y=offset_y;
	}
	///Sets font
	void SetFont(AFont *font)
	{
		_font = font;
	}
	///Set active font
	void SetFont(const __FlashStringHelper * fontName)
	{
		auto font = Environment::Get()->FindFont(fontName);
		if (font != NULL)
			SetFont(font);
	}
};

