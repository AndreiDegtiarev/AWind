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
#include "Environment.h"
#include "Button.h"
///Base class for dialog objects. Provides basic window functionality
class Dialog : public Window, public ITouchEventReceiver
{
	IDialogClosedEventReceiver *_dialogClosedEventReceiver;
protected:
	Button * _btnOK;
	Button * _btnCancel;
public:
	///Constructor
	/**
	\param title internal window name that help by debugging
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
		SetDecorators(*Environment::Get()->FindDecorators(F("Dialog")));
		TextBoxFString *captionText=new TextBoxFString(2,2,width-4,20,caption);
		captionText->SetDecorators(*Environment::Get()->FindDecorators(F("DialogCaption")));
		captionText->SetFont(BigFont);
		captionText->SetMargins(0,1);
		AddChild(captionText);
	}
    Dialog(int left,int top,int width,int height):Window(F("Dialog"),left,top,width,height)
	{
		_btnOK=NULL;
		_btnCancel=NULL;
		_dialogClosedEventReceiver=NULL;
		SetDecorators(*Environment::Get()->FindDecorators(F("Dialog")));
	}

	///Registers extern interface that will be called if dialog is closed
	void RegisterEndDialogEventReceiver(IDialogClosedEventReceiver *receiver)
	{
		_dialogClosedEventReceiver=receiver;
	}
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
	virtual void DoControlMessage(Window *window)=0;
};
