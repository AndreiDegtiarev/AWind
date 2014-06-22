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
class TextBoxNumber : public TextBox
{
	float _number;
	bool _isOK;
	int _precission;
public:
	TextBoxNumber(int left,int top,int width,int height,int precission,Color textColor,bool isOK=false):TextBox(left,top,width,height,textColor)
	{
		_precission=precission;
		_isOK=isOK;
		_number=0;
	}
	void SetNumber(float number)
	{
		_number = number;
	}
	float GetNumber()
	{
		return _number;
	}
	void SetResultStatus(bool isOK)
	{
		_isOK = isOK;
	}
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		if(_isOK)
		{
			dc->DrawNumber(_number,_precission,0,0);
		}
		//else
			//lcd->print("...",1,_left,_top);
	}
};