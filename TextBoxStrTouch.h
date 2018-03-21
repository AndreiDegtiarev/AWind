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

class TextBoxStrTouch : public TextBox
{
	char *_text;
public:
	TextBoxStrTouch(int left,int top,int width,int height,char *text):TextBox(left,top,width,height)
	{
		_text=text;
	}
	bool IsAwaitTouch()
	{
		return !TextBox::IsAwaitTouch();
	}
	bool OnTouch(int x,int y);
	void SetText(char *text)
	{
		if(_text!=text)
		{
			_text = text;
			if(_changedEvent!=NULL)
				_changedEvent->NotifyContentChanged(this);
			Invalidate();
		}
	}
	char * GetText()
	{
		return _text;
	}
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		dc->DrawText(_text,_offset_x,_offset_y);
	}
};