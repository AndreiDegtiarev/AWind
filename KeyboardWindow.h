#pragma once
#include "TextBoxNumber.h"
#include "TextBoxString.h"

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

class KeyboardWindow :  public TouchWindow
{
	TextBoxNumber *_targetTextBox;

	TextBoxString<char> * _editField;
	TextBoxNumber * _digidWindows[10];
	TextBoxString<const __FlashStringHelper> * _enterSymbol;
	TextBoxString<const __FlashStringHelper> * _cancelSymbol;
	TextBoxString<const __FlashStringHelper> * _pointSymbol;
	TextBoxString<const __FlashStringHelper> * _backspaceSymbol;

	char _editBuffer[15];
	int  _editPosition;
	const static int _buttonSize=40;
	const static int _buttonDistance=5;
	const static int _textOffset=9;
public:
	KeyboardWindow(int left,int top):TouchWindow(F(""),left,top,7*(_buttonSize+_buttonDistance),3*(_buttonSize+_buttonDistance)+_buttonDistance)
	{
		_targetTextBox=NULL;
		SetBorder(Color::CornflowerBlue);
		SetBackColor(Color::Black);
		//_digidWindows=new TextBoxNumber *[10];
		int x=_buttonDistance;
		int y=_buttonDistance;
		_editField=new TextBoxString<char>(x,y,7*(_buttonSize+_buttonDistance)-2*_buttonDistance,_buttonSize,"",Color::DarkBlue);
		y+=_buttonSize+_buttonDistance;
		_backspaceSymbol=new TextBoxString<const __FlashStringHelper>(5*(_buttonSize+_buttonDistance),y,_buttonSize,_buttonSize,F("<-"),Color::DarkBlue);
		_enterSymbol=new TextBoxString<const __FlashStringHelper>(6*(_buttonSize+_buttonDistance),y,_buttonSize,_buttonSize,F("E"),Color::DarkBlue);
		for(int i=0;i<10;i++)
		{
			x=(i-(i<5?0:5))*(_buttonSize+_buttonDistance);
			if(i==5)
				y+=_buttonSize+_buttonDistance;
			_digidWindows[i]=new TextBoxNumber(x+_buttonDistance,y,_buttonSize,_buttonSize,0,Color::DarkBlue,true);
			_digidWindows[i]->SetNumber(i);
			initTextBox(_digidWindows[i]);
			AddChild(_digidWindows[i]);
		}
		_pointSymbol=new TextBoxString<const __FlashStringHelper>(5*(_buttonSize+_buttonDistance),y,_buttonSize,_buttonSize,F("."),Color::DarkBlue);
		_cancelSymbol=new TextBoxString<const __FlashStringHelper>(6*(_buttonSize+_buttonDistance),y,_buttonSize,_buttonSize,F("C"),Color::DarkBlue);
		initTextBox(_editField);
		initTextBox(_backspaceSymbol);
		_backspaceSymbol->SetTextOffset(_textOffset,_textOffset*1.5);
		_backspaceSymbol->SetFont(SmallFont);
		initTextBox(_pointSymbol);
		initTextBox(_cancelSymbol);
		initTextBox(_enterSymbol);
		AddChild(_editField);
		AddChild(_backspaceSymbol);
		AddChild(_pointSymbol);
		AddChild(_enterSymbol);
		AddChild(_cancelSymbol);
	}
protected:
	void initTextBox(TextBox *text)
	{
		text->SetFont(BigFont);
		text->SetColor(Color::CornflowerBlue);
		text->SetBorder(Color::CornflowerBlue);
		text->SetBackColor(Color::Black);
		text->SetTextOffset(_textOffset,_textOffset);
	}
public:
	void BeginEdit(TextBoxNumber * targetTextBox)
	{
		_editPosition=0;
		_targetTextBox=targetTextBox;
		dtostrf( _targetTextBox->GetNumber(),0,_targetTextBox->Precission(),_editBuffer);
		//sprintf(_editBuffer, "%f", _targetTextBox->GetNumber());
		//Serial.println(_editBuffer);
		_editField->SetText(_editBuffer);
		_editField->Invalidate();
		_editPosition=strlen(_editBuffer);
		SetVisible(true);
		Invalidate();
	}
	virtual bool OnTouch(int x,int y)
	{
		Window *hitTestWnd=this->HitTest(x,y);
		if(hitTestWnd == _enterSymbol || hitTestWnd == _cancelSymbol)
		{
			if(hitTestWnd == _enterSymbol)
			{
				float number=atof(_editBuffer);
				//Log::Number("KeyboardWindow end eidt: ",number,true);
				_targetTextBox->SetNumber(number);
				_targetTextBox->Invalidate();
			}
			SetVisible(false);
			Parent()->Invalidate();
		}
		else
		{
			bool needUpdate=false;
			if(hitTestWnd == _backspaceSymbol && _editPosition>0)
			{
				//Log::Number("KeyboardWindow backspace touch: ",_editPosition,true);
				_editPosition--;
				_editBuffer[_editPosition]=0;
				needUpdate=true;
			}
			else if(hitTestWnd == _pointSymbol && _editPosition<14)
			{
				_editBuffer[_editPosition++]='.';
				_editBuffer[_editPosition]=0;
				needUpdate=true;
			}
			else
			{
				for(int i=0;i<10;i++)
					if(hitTestWnd == _digidWindows[i] && _editPosition<14)
					{
						//Log::Number("KeyboardWindow touch: ",((TextBoxNumber *)hitTestWnd)->GetNumber(),true);
						_editBuffer[_editPosition++]='0'+i;
						_editBuffer[_editPosition]=0;
						needUpdate=true;
						break;
					}
			}
			if(needUpdate)
			{
				_editField->SetText(_editBuffer);
				_editField->Invalidate();
			}
		}
		return true;
	}
};