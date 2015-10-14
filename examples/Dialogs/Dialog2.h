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
#pragma once
#include <Dialog.h>
#include <Label.h>
#include <TextBoxNumber.h>
///Example window with yellow background
class Dialog2 : public Dialog
{
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
	Dialog2(const __FlashStringHelper * caption,int left,int top,int width,int height):Dialog(caption,left,top,width,height)
	{
		_txtNumber2=InitNumberFields(0,F("Enter number 1:"));
		_txtNumber2->SetIsReadOnly(false);
		_btnOK=new Button(0,0,0,0,F("OK"));
		InitButton(_btnOK,10,80,60);
		_btnCancel=new Button(0,0,0,0,F("Cancel"));
		InitButton(_btnCancel,80,80,110);
	}
	///Init text box value
	void SetNumber(float number2)
	{
		_txtNumber2->SetNumber(number2);
	}
	///Return value from text box
	float GetNumber()
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
		//out<<F("Button pressed: ")<<sel_index<<endl;
	}
};