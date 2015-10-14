#pragma once
#include "TextBox.h"

template <class T> class TextBoxEditTString : public TextBox
{
	const static int _bufSize = 70;
	const static int _editSize = 18;
	int _editPos, _caretPos, _bufLen;
	char _buf[_bufSize+1];
	char _tmpBuf[20];
public:
	TextBoxEditTString(int left,int top,int width,int height,T *text):TextBox(left,top,width,height)
	{
		_editPos = -1;
		_bufLen = 0;
		_caretPos = -1;
	}
	virtual void OnDraw(DC *dc)
	{
		TextBox::OnDraw(dc);
		dc->DrawText(_tmpBuf,_offset_x,_offset_y);
		dc->DrawCaret(_caretPos, _offset_x, _offset_y);
	}
	void GetEditText() {
		int s, e;
		s = _bufLen - _editSize;
		if (s<0)
			s = 0;
		e = (_bufLen > _editSize)? _editSize : _bufLen;
		strncpy(_tmpBuf, _buf+s, e);
		_tmpBuf[e] = 0;
	}
	void SetText(T *text)
	{
		if (_caretPos == -1)
		{
			int len;
			len = strlen(text);
			_bufLen = (len > _bufSize)? _bufSize : len;
			strncpy(_buf, text, _bufLen);
			_buf[_bufLen] = 0;
			_editPos = _bufLen;
			_caretPos = (_bufLen > _editSize)? _editSize : _bufLen;
			GetEditText();
		}
		if(_changedEvent!=NULL)
			_changedEvent->NotifyContentChanged(this);
		Invalidate();
	}
	T * GetText()
	{
		char *s;
		s = (char*) malloc(sizeof(char) * (strlen(_buf)+1) );
		strcpy(s, _buf);
		return s;
	}
	void Closed() {
		_editPos = -1;
		_bufLen = 0;
		_caretPos = -1;
		//_buf[0] = 0;
		_tmpBuf[0] = 0;
	}
	void UpdateEdit(byte b)
	{
		int i;
		switch(b) {
			case 8: //backspace
				if (_editPos>0) {
					for (i=_editPos-1; i<_bufLen; i++)
						_buf[i] = _buf[i+1];
					_editPos--;
					_bufLen--;
					if (_bufLen < _editSize)
						_caretPos--;
				}
				break;
			case 13: //clear
				_editPos = 0;
				_bufLen = 0;
				_caretPos = 0;
				_buf[0] = 0;
				break;
			default: //add/insert char
				if ((_editPos < _bufSize) && (_bufLen < _bufSize)) {
					if (_editPos != _bufLen) {
						for (i=_bufLen; i>=_editPos; i--)
							_buf[i+1] = _buf[i];
					}
					_buf[_editPos++] = (char)b;
					_bufLen++;
					if (_caretPos<_editSize)
						_caretPos++;
					if (_editPos == _bufLen)
						_buf[_editPos] = 0;
				}
				break;
		}
		if (_bufLen>0)
			GetEditText();
		else
			_tmpBuf[0] = 0;
		SetText(_tmpBuf);
	}
};
typedef	TextBoxEditTString<const __FlashStringHelper> TextBoxEditFString;
typedef TextBoxEditTString<char> TextBoxEditString;