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
#include "Window.h"
#include "IContentChangedEventReceiver.h"

extern uint8_t BigFont[];
extern uint8_t SmallFont[];


///Base class for window with text content
class TextBox : public Window
{
	uint8_t *_font;
protected:
	int _offset_x;
	int _offset_y;
	DC::HorizontalAligment _horizontal_aligment;
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
		_font = SmallFont;
		_offset_x=0;
		_offset_y=0;
		_horizontal_aligment = DC::Left;
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
	void SetHorizontalAligment(DC::HorizontalAligment aligment)
	{
		_horizontal_aligment = aligment;
	}
	///Defines offset from left and top for text
	void SetMargins(int offset_x,int offset_y)
	{
		_offset_x=offset_x;
		_offset_y=offset_y;
	}
	///Sets font
	void SetFont(uint8_t *font)
	{
		_font = font;
	}
};

