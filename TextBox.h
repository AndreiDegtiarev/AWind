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
#include "Window.h"
#include "IContentChangedEventReceiver.h"
extern uint8_t SmallFont[];

class TextBox : public Window
{
	Color _textColor;
	uint8_t *_font;
protected:
	int _offset_x;
	int _offset_y;
	IContentChangedEventReceiver *_changedEvent;
public:
	TextBox(int left,int top,int width,int height,Color textColor):Window(F("text"),left,top,width,height),_textColor(textColor)
	{
		_font = SmallFont;
		_offset_x=0;
		_offset_y=0;
		_changedEvent=NULL;
	}
	void RegisterContentChangedReceiver(IContentChangedEventReceiver *event)
	{
		_changedEvent=event;
	}
	void SetMargins(int offset_x,int offset_y)
	{
		_offset_x=offset_x;
		_offset_y=offset_y;
	}
	void SetFont(uint8_t *font)
	{
		_font = font;
	}
	void SetColor(Color textColor)
	{
		_textColor=textColor;
	}
	void OnDraw(DC *dc)
	{
		dc->SetColor(_textColor);
		if(_font !=NULL)
			dc->SetFont(_font);
	}
};

