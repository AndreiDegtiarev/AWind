#pragma once
/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  

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
#include "TextBoxNumber.h"
#include "TextBoxString.h"
#include "IDialogClosedEventReceiver.h"
#include "DecoratorPrimitives.h"


extern uint8_t BigFont[];
extern uint8_t SmallFont[];
///Implements Keyboard window, that helps to enter text/number information into text fields
class KeyboardWindow :  public Window,ITouchEventReceiver
{
	//TextBoxNumber *_targetTextBox;
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
	///Constructor
	/**
	\param left left coordinate relative to screen
	\param top top coordinate relative to screen
	*/

	KeyboardWindow(int left,int top):Window(F("Keyboard"),left,top,7*(_buttonSize+_buttonDistance)+_buttonDistance,3*(_buttonSize+_buttonDistance)+_buttonDistance)
	{
		AddDecorator(new DecoratorRectFill(Color::LightGray));
		AddDecorator(new Decorator3DRect(Color::White,Color::Gray));
		AddDecorator(new DecoratorColor(Color::Black));

		int x=_buttonDistance;
		int y=_buttonDistance;
		_editField=new TextBoxString(x,y,Width()-2*_buttonDistance,_buttonSize,"");
		_editField->AddDecorator(new DecoratorRectFill(Color::LightGray));
		_editField->AddDecorator(new Decorator3DRect(Color::Gray,Color::White));
		_editField->AddDecorator(new DecoratorColor(Color::Black));
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
		initTextBox(_editField,&_editField->GetDecorators());
		initTextBox(_backspaceSymbol);
		_backspaceSymbol->SetMargins(_textOffset,_textOffset*1.5);
		_backspaceSymbol->SetFont(SmallFont);
		initTextBox(_pointSymbol);
		initTextBox(_cancelSymbol);
		initTextBox(_enterSymbol);
		_dialogClosedEventReceiver=NULL;
	}
protected:
	void initTextBox(TextBox *text,DecoratorList *decorators=NULL)
	{
		text->SetDecorators(decorators==NULL?GetDecorators():*decorators);
		text->SetFont(BigFont);
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
	///Registers extern interface that will be called if keyboard window is closed
	void RegisterEndDialogEventReceiver(IDialogClosedEventReceiver *receiver)
	{
		_dialogClosedEventReceiver=receiver;
	}
	///Returns number from edit window
	float GetNumber()
	{
		return atof(_editBuffer);
	}
	///Process touch notifications
	void NotifyTouch(Window *window)
	{
		//out<<F("Keybord notify")<<endl;
		if(window == _enterSymbol || window == _cancelSymbol)
		{
			if(_dialogClosedEventReceiver!=NULL)
				_dialogClosedEventReceiver->NotifyDialogClosed(this,window == _enterSymbol?IDialogClosedEventReceiver::OK:IDialogClosedEventReceiver::Cancel);
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