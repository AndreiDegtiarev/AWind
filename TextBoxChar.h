#pragma once
#include "TextBox.h"

class TextBoxChar : public TextBox
{
	char _chr;
public:
	TextBoxChar(int left,int top,int width,int height, char ch):TextBox(left,top,width,height)
	{
		_chr = ch;
	}
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		dc->DrawChar(_chr,_offset_x,_offset_y);
	}
	void SetChar(char ch)
	{
    _chr = ch;
		if(_changedEvent!=NULL)
			_changedEvent->NotifyContentChanged(this);
		Invalidate();
	}
	char GetChar()
	{
		return _chr;
	}
};