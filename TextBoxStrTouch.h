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