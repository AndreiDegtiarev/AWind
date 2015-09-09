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
	///Implements drawing code
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		dc->DrawText(_text,_offset_x,_offset_y);
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