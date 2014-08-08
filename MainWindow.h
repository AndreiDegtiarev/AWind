#pragma once
/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  

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
#include "Window.h"
#include "KeyboardWindow.h"
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
	Window *DlgWindow;
};
class TextBoxNumber;
class MainWindow : public Window, IDialogClosedEventReceiver
{
	Window *_modalWindow;
	KeyboardWindow _keyboardWindow;
	ILoopProcess *_idleProcess;
	LinkedList<DialogEntry> _dialogs;
	DialogResults _lastDialogResults;
public:
	MainWindow(int width,int height):Window(F("Main"),0,0,width,height),_keyboardWindow(3,90)
	{
		_keyboardWindow.RegisterEndDialogEventReceiver(this);
		_modalWindow=NULL;
		RegisterDialog(F("Keyboard"),&_keyboardWindow);
	}
	void RegisterDialog(const __FlashStringHelper *id,Window * widnow)
	{
		DialogEntry *dlgEntry=new DialogEntry();
		dlgEntry->ID=id;
		dlgEntry->DlgWindow=widnow;
		_dialogs.Add(dlgEntry);
		widnow->SetVisible(false);
		AddChild(widnow);
	}
	Window *FindDialog(const __FlashStringHelper *id)
	{
		for(int i=0;i<_dialogs.Count();i++)
		{
			//out<<"Find dialog: "<<_dialogs[i]->ID<<endl;
			if(strcmp_P(reinterpret_cast<const char*>(id), reinterpret_cast<const char*>(_dialogs[i]->ID)))
				return _dialogs[i]->DlgWindow;

		}
		return NULL;
	}
	DialogResults DoDialog(Window *dlg)
	{
		//out<<"IDialogProcessor::DialogResults"<<endl;
		SetModalWindow(dlg);
		dlg->SetVisible(true);
		dlg->Invalidate();

		while(_modalWindow!=NULL)
		{
			_idleProcess->loop();
		}
		dlg->SetVisible(false);
		Invalidate();
		return _lastDialogResults;
	}
	void SetLoopProcess(ILoopProcess *process)
	{
		_idleProcess=process;
	}
	void NotifyDialogClosed(Window *window,DialogResults results)
	{
		//out<<"NotifyDialogClosed"<<endl;
		//if(window == &_keyboardWindow) //End edit
		{
			SetModalWindow(NULL);
			_lastDialogResults=results;
		}
	}
	Window *ModalWnd()
	{
		return _modalWindow;
	}
	void SetModalWindow(Window * modalWindow)
	{
		_modalWindow=modalWindow;
	}
	void Move(int left,int top,int width,int height)
	{
		Window::Move(left,top,width,height);
	}
	KeyboardWindow * Keyboard()
	{
		return &_keyboardWindow;
	}
};