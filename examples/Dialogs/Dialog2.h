/*
This file is part of AWind library

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