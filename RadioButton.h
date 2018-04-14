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
#include "TextBoxString.h"
///Implement radio button control
class RadioButton : public TextBoxTString<const __FlashStringHelper>
{
	bool _isChecked;
public:
	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	\param text button name
	*/
	RadioButton(int left, int top, int width, int height, const __FlashStringHelper * text) :TextBoxTString<const __FlashStringHelper>(left, top, width, height, text, F("Button")), _isChecked(false)
	{
		SetFont(F("Big"));
		SetDecorators(Environment::Get()->FindDecorators(F("RadioButtonInactive")));
	}
	bool IsChecked()
	{
		return _isChecked;
	}
	void SetChecked(bool isChecked)
	{
		_isChecked = isChecked;
		if (_isChecked)
			SetDecorators(Environment::Get()->FindDecorators(F("RadioButtonActive")));
		else
			SetDecorators(Environment::Get()->FindDecorators(F("RadioButtonInactive")));
		Invalidate();
	}
};