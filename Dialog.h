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
#include "Environment.h"
#include "Button.h"
///Base class for dialog objects. See Dialogs example Provides basic window functionality
class Dialog : public Window, public ITouchEventReceiver
{
	IDialogClosedEventReceiver *_dialogClosedEventReceiver;
protected:
	///Button OK has to be initialized in derived class
	Button * _btnOK;
	///Button Cancel has to be initialized in derived class
	Button * _btnCancel;
public:
	///Constructor for dialog with caption
	/**
	\param caption caption of dialog
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
    Dialog(const __FlashStringHelper * caption,int left,int top,int width,int height):Window(caption,left,top,width,height)
	{
		_btnOK=NULL;
		_btnCancel=NULL;
		_dialogClosedEventReceiver=NULL;
		SetDecorators(Environment::Get()->FindDecorators(F("Dialog")));
		TextBoxFString *captionText=new TextBoxFString(2,2,width-4,20,caption);
		captionText->SetDecorators(Environment::Get()->FindDecorators(F("DialogCaption")));
		captionText->SetFont(F("Big"));
		captionText->SetMargins(0,1);
		AddChild(captionText);
	}
	///Constructor for dialog without caption
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
    Dialog(int left,int top,int width,int height):Window(F("Dialog"),left,top,width,height)
	{
		_btnOK=NULL;
		_btnCancel=NULL;
		_dialogClosedEventReceiver=NULL;
		SetDecorators(Environment::Get()->FindDecorators(F("Dialog")));
	}

	///Registers extern interface that will be called if dialog is closed
	void RegisterEndDialogEventReceiver(IDialogClosedEventReceiver *receiver)
	{
		_dialogClosedEventReceiver=receiver;
	}
	///Process touch notifications
	void NotifyTouch(Window *window)
	{
		if(window!=NULL && (window == _btnOK || window == _btnCancel))
		{
			if(_dialogClosedEventReceiver!=NULL)
			{
				_dialogClosedEventReceiver->NotifyDialogClosed(this,window == _btnOK?IDialogClosedEventReceiver::OK:IDialogClosedEventReceiver::Cancel);
			}
		}
		else
			DoControlMessage(window);
	}
	///Need to be redefinded in derived class 
	virtual void DoControlMessage(Window *window)=0;
};
