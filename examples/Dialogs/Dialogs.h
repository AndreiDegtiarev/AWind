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
#include "MainWindow.h"
#include "TextBoxString.h"
#include "DefaultDecorators.h"
#include "Dialog1.h"

///Dialogs main window
class Dialogs : public MainWindow, public ITouchEventReceiver
{
	Button * _btnDlg1;
	TextBoxNumber *_txtNumber1;
	TextBoxNumber *_txtNumber2;
public:
	///Constructor
	/**
	\param wnd_width screen width
	\param wnd_height screen height
	*/
	Dialogs(int wnd_width,int wnd_height):MainWindow(wnd_width,wnd_height)
	{
	}
	///Initilizes child controls
	void Initialize()
	{
		_btnDlg1=new Button(70,Height()/2,190,50,F("Get numbers"));
		_btnDlg1->SetMargins(5,15);
		_btnDlg1->RegisterTouchEventReceiver(this);
		AddChild(_btnDlg1);
		_txtNumber1=InitNumberFields(0,F("      Number 1:"));
		_txtNumber2=InitNumberFields(30,F("      Number 2:"));
	}
protected:
	///create child text box fields
	TextBoxNumber *InitNumberFields(int offset,const __FlashStringHelper * text)
	{
		Label *label=new Label(10,43+offset,150,25,text);
		AddChild(label);
		TextBoxNumber * txtNumber=new TextBoxNumber(130,40+offset,40,20,0);
		txtNumber->SetNumber(0);
		AddChild(txtNumber);
		return txtNumber;
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if(window == _btnDlg1)
		{
			Dialog1 *dlg=(Dialog1 *)this->FindDialog(F("Dialog1"));
			dlg->InitNumbers(_txtNumber1->GetNumber(),_txtNumber2->GetNumber());

			IDialogClosedEventReceiver::DialogResults dlgResults=this->DoDialog(dlg);
			if(dlgResults == IDialogClosedEventReceiver::OK)
			{
				_txtNumber1->SetNumber(dlg->GetNumber1());
				_txtNumber2->SetNumber(dlg->GetNumber2());
			}
		}
	}
};