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

#include "TextBox.h"

template <class T> class TextBoxString : public TextBox
{
	const T * _text;
public:
	TextBoxString(int left,int top,int width,int height,T *text,Color textColor):TextBox(left,top,width,height,textColor)
	{
		_text=text;
	}
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		dc->DrawText(_text,_offset_x,_offset_y);
	}
	void SetText(T *text)
	{
		_text=text;
	}
};