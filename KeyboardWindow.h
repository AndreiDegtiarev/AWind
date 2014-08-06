#pragma once
#include "TextBoxNumber.h"
#include "TextBoxString.h"
#include "IDialogClosedEventReceiver.h"
#include "DecoratorPrimitives.h"


extern uint8_t BigFont[];
extern uint8_t SmallFont[];

class KeyboardWindow :  public Window,ITouchEventReceiver
{
	TextBoxNumber *_targetTextBox;

	TextBoxString * _editField;
	TextBoxNumber * _digidWindows[10];
	TextBoxFString * _enterSymbol;
	TextBoxFString * _cancelSymbol;
	TextBoxFString * _pointSymbol;
	TextBoxFString * _backspaceSymbol;

	char _editBuffer[15];
	int  _editPosition;
	const static int _buttonSize=39;
	const static int _buttonDistance=5;
	const static int _textOffset=9;
	IDialogClosedEventReceiver *_dialogClosedEventReceiver;
public:
	KeyboardWindow(int left,int top):Window(F("Keyboard"),left,top,7*(_buttonSize+_buttonDistance)+_buttonDistance,3*(_buttonSize+_buttonDistance)+_buttonDistance)
	{
		_targetTextBox=NULL;
		AddDecorator(new DecoratorRectFill(Color::Black));
		AddDecorator(new DecoratorRaundRect(Color::CornflowerBlue));
		AddDecorator(new DecoratorColor(Color::CornflowerBlue));

		//SetBorder(Color::CornflowerBlue);
		//SetBackColor(Color::Black);
		int x=_buttonDistance;
		int y=_buttonDistance;
		_editField=new TextBoxString(x,y,Width()-2*_buttonDistance,_buttonSize,"");
		y+=_buttonSize+_buttonDistance;
		_backspaceSymbol=new TextBoxFString(5*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("<-"));
		_enterSymbol=new TextBoxFString(6*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("E"));
		for(int i=0;i<10;i++)
		{
			x=(i-(i<5?0:5))*(_buttonSize+_buttonDistance);
			if(i==5)
				y+=_buttonSize+_buttonDistance;
			_digidWindows[i]=new TextBoxNumber(x+_buttonDistance,y,_buttonSize,_buttonSize,0);
			_digidWindows[i]->SetNumber(i);
			initTextBox(_digidWindows[i]);
		}
		_pointSymbol=new TextBoxFString(5*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("."));
		_cancelSymbol=new TextBoxFString(6*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("C"));
		initTextBox(_editField);
		initTextBox(_backspaceSymbol);
		_backspaceSymbol->SetMargins(_textOffset,_textOffset*1.5);
		_backspaceSymbol->SetFont(SmallFont);
		initTextBox(_pointSymbol);
		initTextBox(_cancelSymbol);
		initTextBox(_enterSymbol);
		_dialogClosedEventReceiver=NULL;
	}
protected:
	void initTextBox(TextBox *text)
	{
		//text->SetBorder(Color::CornflowerBlue);
		//text->SetBackColor(Color::Black);
		text->SetDecorators(GetDecorators());
		text->SetFont(BigFont);
		text->SetMargins(_textOffset,_textOffset);
		text->RegisterTouchEventReceiver(this);
		AddChild(text);
	}
public:
	void BeginEdit(TextBoxNumber * targetTextBox)
	{
		_editPosition=0;
		_targetTextBox=targetTextBox;
		dtostrf( _targetTextBox->GetNumber(),0,_targetTextBox->Precission(),_editBuffer);
		_editField->SetText(_editBuffer);
		_editField->Invalidate();
		_editPosition=strlen(_editBuffer);
		SetVisible(true);
		Invalidate();
	}
	void RegisterEndDialogEventReceiver(IDialogClosedEventReceiver *receiver)
	{
		_dialogClosedEventReceiver=receiver;
	}
	void NotifyTouch(Window *window)
	{
		out<<F("Keybord notify")<<endl;
		if(window == _enterSymbol || window == _cancelSymbol)
		{
			if(window == _enterSymbol)
			{
				float number=atof(_editBuffer);
				_targetTextBox->SetNumber(number);
				_targetTextBox->Invalidate();
			}
			SetVisible(false);
			if(_dialogClosedEventReceiver!=NULL)
				_dialogClosedEventReceiver->NotifyDialogClosed(this);
		}
		else
		{
			bool needUpdate=false;
			if(window == _backspaceSymbol && _editPosition>0)
			{
				_editPosition--;
				_editBuffer[_editPosition]=0;
				needUpdate=true;
			}
			else if(window == _pointSymbol && _editPosition<14)
			{
				_editBuffer[_editPosition++]='.';
				_editBuffer[_editPosition]=0;
				needUpdate=true;
			}
			else
			{
				for(int i=0;i<10;i++)
					if(window == _digidWindows[i] && _editPosition<14)
					{
						_editBuffer[_editPosition++]='0'+i;
						_editBuffer[_editPosition]=0;
						needUpdate=true;
						break;
					}
			}
			if(needUpdate)
				_editField->SetText(_editBuffer);
		}
	}
};