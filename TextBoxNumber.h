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
		SetFont(Environment::Get()->FindFont(F("Big")));
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