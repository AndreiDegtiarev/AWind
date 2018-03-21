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

#include "TextBox.h"
///Text box for string. It is templat class and can be used for differnt type of string (see bellow typedef's)
template <class T> class TextBoxTString : public TextBox
{
	const T * _text;
public:
	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	\param text text to visualize
	*/	
	TextBoxTString(int left,int top,int width,int height,T *text):TextBox(left,top,width,height)
	{
		_text=text;
		//_type=F("TextBoxString");
	}
	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	\param text text to visualize
	\param decorators name of predefined decorator
	*/	
	TextBoxTString(int left,int top,int width,int height,T *text,const __FlashStringHelper *decorators):TextBox(left,top,width,height)
	{
		_text=text;
		SetDecorators(Environment::Get()->FindDecorators(decorators));
		//_type=F("TextBoxString");
	}
	///Implements drawing code
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		dc->DrawText(_text, _offset_x, _offset_y, _horizontal_alignment,Width()- _offset_x*2);
	}
	///Initialize window with text
	void SetText(T *text)
	{
		_text=text;
		if(_changedEvent!=NULL)
			_changedEvent->NotifyContentChanged(this);
		Invalidate();
	}
};
///typedef for __FlashStringHelper - very usefull type that allows to save very limitted amount of on-board memory 
typedef	TextBoxTString<const __FlashStringHelper> TextBoxFString;
///typedef for common string
typedef TextBoxTString<char> TextBoxString;