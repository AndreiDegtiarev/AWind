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
///Text box for numbers
class TextBoxNumber : public TextBox
{
	float _number;
	bool _isOK;
	int _precission;
	bool _isReadOnly;
public:
	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	\param precission number of digits after decimal point
	*/
	TextBoxNumber(int left=0,int top=0,int width=0,int height=0,int precission=0):TextBox(left,top,width,height)
	{
		_precission=precission;
		_isOK=true;
		_number=0;
		SetFont(BigFont);
		SetDecorators(Environment::Get()->FindDecorators(F("EditTextBoxReadOnly")));
		SetMargins(0,1);
		SetIsReadOnly(true);
	}
	///Returns actual number of digits after decimal point
	int Precission()
	{
		return _precission;
	}
    void SetPrecission(int precission)
	{
		_precission=precission;
	}
	bool IsAwaitTouch()
	{
		return !_isReadOnly||TextBox::IsAwaitTouch();
	}
	bool OnTouch(int x,int y);
	///Defines whether window is readonly
	void SetIsReadOnly(bool isReadOnly)
	{
	   SetDecorators(Environment::Get()->FindDecorators(isReadOnly?F("EditTextBoxReadOnly"):F("EditTextBox")));
		_isReadOnly=isReadOnly;
	}
	bool IsReadOnly()
	{
		return _isReadOnly;
	}
	///Initialize window with number
	void SetNumber(float number)
	{
		if(_number!=number)
		{
			_number = number;
			if(_changedEvent!=NULL)
				_changedEvent->NotifyContentChanged(this);
			Invalidate();
		}
	}
	float GetNumber()
	{
		return _number;
	}
	void SetStatus(bool isOK)
	{
		_isOK = isOK;
	}
	///Implements drawing code
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		if(_isOK)
		{
			dc->DrawNumber(_number,_precission,_offset_x,_offset_y, _horizontal_alignment, Width() - _offset_x * 2);
		}
	}
};