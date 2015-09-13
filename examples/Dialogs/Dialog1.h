#pragma once
/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
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
#include <Dialog.h>
#include <Label.h>
#include "Dialog2.h"
///Example Dialog with one edit, one readonly field + button that activates another dialog
class Dialog1 : public Dialog
{
	///Buttons that calls dialog2
	Button * _btnDlg2;
	///Text field where number can be entered
	TextBoxNumber *_txtNumber1;
	///Text field wwith readonly number
	TextBoxNumber *_txtNumber2;
public:
	///Constructor
	/**
	\param caption dialog caption
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/	
	Dialog1(const __FlashStringHelper * caption,int left,int top,int width,int height):Dialog(caption,left,top,width,height)
	{
		_txtNumber1=InitNumberFields(0,F("Enter number 1:"));
		_txtNumber1->SetIsReadOnly(false);
		_txtNumber2=InitNumberFields(30,F("      Number 2:"));

		_btnDlg2=new Button(0,0,0,0,F("Get"));
		InitButton(_btnDlg2,180,70,60);
		_btnOK=new Button(0,0,0,0,F("OK"));
		InitButton(_btnOK,60,110,60);
		_btnCancel=new Button(0,0,0,0,F("Cancel"));
		InitButton(_btnCancel,130,110,110);
	}
	///Init text box values
	void InitNumbers(float number1,float number2)
	{
		_txtNumber1->SetNumber(number1);
		_txtNumber2->SetNumber(number2);
	}
	///Return value from text box 1
	float GetNumber1()
	{
		return _txtNumber1->GetNumber();
	}
	///Return value from text box 2
	float GetNumber2()
	{
		return _txtNumber2->GetNumber();
	}
protected:
	///create child text box fields
	TextBoxNumber *InitNumberFields(int offset,const __FlashStringHelper * text)
	{
		Label *label=new Label(10,43+offset,150,25,text);
		AddChild(label);
		TextBoxNumber * txtNumber=new TextBoxNumber(130,40+offset,40,20,0);
		AddChild(txtNumber);
		return txtNumber;
	}
	///Create child button
	void InitButton(Button *btn,int left,int top,int width)
	{
		btn->Move(left,top,width,25);
		btn->SetMargins(5,5);
		btn->RegisterTouchEventReceiver(this);
		AddChild(btn);
	}
	///Process touch message from child controls
	void DoControlMessage(Window *window)
	{
		if(window == _btnDlg2)
		{
			Dialog2 *dlg=(Dialog2 *)this->FindDialog(F("Dialog2"));
			dlg->SetNumber(_txtNumber2->GetNumber());
			IDialogClosedEventReceiver::DialogResults dlgResults=this->DoDialog(dlg);
			if(dlgResults == IDialogClosedEventReceiver::OK)
			{
				_txtNumber2->SetNumber(dlg->GetNumber());
			}
		}		
		//out<<F("Button pressed: ")<<sel_index<<endl;
	}
};