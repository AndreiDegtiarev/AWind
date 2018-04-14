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
#include "TextBoxNumber.h"
#include "TextBoxString.h"
#include "IDialogClosedEventReceiver.h"
#include "DecoratorPrimitives.h"
#include "Dialog.h"


extern uint8_t BigFont[];
extern uint8_t SmallFont[];
///Implements Keyboard window, that helps to enter text/number information into text fields
class KeyboardWindow :  public Dialog
{
	//TextBoxNumber *_targetTextBox;
	TextBoxString * _editField;
	TextBoxNumber _digidWindows[10];
	TextBoxFString * _pointSymbol;
	TextBoxFString * _backspaceSymbol;

	char _editBuffer[15];
	int  _editPosition;
	const static int _buttonSize=39;
	const static int _buttonDistance=5;
	const static int _textOffset=9;
public:
	///Constructor
	/**
	\param left left coordinate relative to screen
	\param top top coordinate relative to screen
	*/
	KeyboardWindow(int left,int top):Dialog(left,top,7*(_buttonSize+_buttonDistance)+_buttonDistance,3*(_buttonSize+_buttonDistance)+_buttonDistance)
	{

		int x=_buttonDistance;
		int y=_buttonDistance;
		_editField=new TextBoxString(x,y,Width()-2*_buttonDistance,_buttonSize,"");
		_editField->SetDecorators(Environment::Get()->FindDecorators(F("EditTextBoxReadOnly")));
		_editField->SetFont(Environment::Get()->FindFont(F("Big")));
		y+=_buttonSize+_buttonDistance;
		_backspaceSymbol=new Button(5*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("<-"));
		_btnOK=new Button(6*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("E"));
		DecoratorList * btnDecorators=Environment::Get()->FindDecorators(F("Button"));
		for(int i=0;i<10;i++)
		{
			x=(i-(i<5?0:5))*(_buttonSize+_buttonDistance);
			if(i==5)
				y+=_buttonSize+_buttonDistance;
			//_digidWindows[i]=new TextBoxNumber(x+_buttonDistance,y,_buttonSize,_buttonSize,0);
			_digidWindows[i].Move(x+_buttonDistance,y,_buttonSize,_buttonSize);
			_digidWindows[i].SetNumber(i);
			_digidWindows[i].SetDecorators(btnDecorators);
			initTextBox(&_digidWindows[i]);
		}
		_pointSymbol=new Button(5*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("."));
		_btnCancel=new Button(6*(_buttonSize+_buttonDistance)+_buttonDistance,y,_buttonSize,_buttonSize,F("C"));
		initTextBox(_editField);
		initTextBox(_backspaceSymbol);
		_backspaceSymbol->SetMargins(_textOffset,_textOffset*1.5);
		_backspaceSymbol->SetFont(Environment::Get()->FindFont(F("Small")));
		initTextBox(_pointSymbol);
		initTextBox(_btnCancel);
		initTextBox(_btnOK);
	}
protected:
	void initTextBox(TextBox *text)
	{
		text->SetMargins(_textOffset,_textOffset);
		text->RegisterTouchEventReceiver(this);
		AddChild(text);
	}
public:
	///Initializes edit field with float value of desired precission
	void Initialize(float value,int precision)
	{
		_editPosition=0;
		dtostrf(value,0,precision,_editBuffer);
		_editField->SetText(_editBuffer);
		_editField->Invalidate();
		_editPosition=strlen(_editBuffer);
	}
	///Returns number from edit window
	float GetNumber()
	{
		return atof(_editBuffer);
	}
	///Process touch message from child controls
	void DoControlMessage(Window *window)
	{
		//out<<F("Keybord notify")<<endln;
		//if(Process_OK_Cancel_Touch(window))
		//	return;
		/*if(window == _enterSymbol || window == _cancelSymbol)
		{
			if(_dialogClosedEventReceiver!=NULL)
				_dialogClosedEventReceiver->NotifyDialogClosed(this,window == _enterSymbol?IDialogClosedEventReceiver::OK:IDialogClosedEventReceiver::Cancel);
		}
		else*/
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
					if(window == &_digidWindows[i] && _editPosition<14)
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