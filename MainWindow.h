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
#include "Dialog.h"
#include "ICriticalProcess.h"
class ILoopProcess
{
public:
	virtual void loop()=0;
};
class DialogEntry
{
public:
	const __FlashStringHelper *ID;
	Dialog *DlgWindow;
};
class TextBoxNumber;
///Base class for main application window. Each  application has to have one main window, which is root parent for all other application windows
class MainWindow : public Window, public IDialogClosedEventReceiver
{
	Window *_modalWindow;
	ILoopProcess *_idleProcess;
	LinkedList<DialogEntry> _dialogs;
	DialogResults _lastDialogResults;
	bool _isModalDialogActive;
public:
	///Constructor
	/**
	\param width screen width
	\param height screen height
	*/
	MainWindow(int width,int height):Window(F("Main"),0,0,width,height),_isModalDialogActive(false)
	{
		_modalWindow=NULL;
	}
	///Registers dialog window. All application dialogs have to be registered
	void RegisterDialog(const __FlashStringHelper *id,Dialog * widnow)
	{
		widnow->RegisterEndDialogEventReceiver(this);

		DialogEntry *dlgEntry=new DialogEntry();
		dlgEntry->ID=id;
		dlgEntry->DlgWindow=widnow;
		_dialogs.Add(dlgEntry);
		widnow->SetVisible(false);
		AddChild(widnow);
	}
	///Finds registered dialog by the name
	Dialog *FindDialog(const __FlashStringHelper *id)
	{
		for(int i=0;i<_dialogs.Count();i++)
		{
			//out<<F("Find dialog:")<<id<<F(" Candidate:")<<_dialogs[i]->ID<<F(" Res:");
			if(AHelper::compare_F(id, _dialogs[i]->ID))
				return _dialogs[i]->DlgWindow;

		}
		return NULL;
	}
	///Starts dialog
	DialogResults ProcessDoDialog(Window *dlg)
	{
		//out<<F("Begin::ProcessDoDialog")<<endln;
		_isModalDialogActive=true;
		Window *lastModalWindow=ModalWnd();
		SetModalWindow(dlg);
		dlg->SetVisible(true);
		dlg->Invalidate();

		while(_isModalDialogActive)
		{
			_idleProcess->loop();
		}
		dlg->SetVisible(false);
		_isModalDialogActive=lastModalWindow!=NULL?true:false;
		SetModalWindow(lastModalWindow);
		Invalidate();
		if(lastModalWindow!=NULL)
		{
			lastModalWindow->Invalidate();
			_idleProcess->loop();
		}
		//out<<F("End::ProcessDoDialog")<<endln;
		return _lastDialogResults;
	}
	void SetLoopProcess(ILoopProcess *process)
	{
		_idleProcess=process;
	}
	///Process dialog closed notification
	void NotifyDialogClosed(Window *window,DialogResults results)
	{
		//out<<F("NotifyDialogClosed")<<endln;
		_isModalDialogActive=false;
		_lastDialogResults=results;
	}
	///Returns pointer to active modal (window that received all user input, like dialog window) window
	Window *ModalWnd()
	{
		return _modalWindow;
	}
	///Sets active modal (window that received all user input, like dialog window) window
	void SetModalWindow(Window * modalWindow)
	{
		_modalWindow=modalWindow;
	}
	///Changes position and size
	void Move(int left,int top,int width,int height)
	{
		Window::Move(left,top,width,height);
	}
};