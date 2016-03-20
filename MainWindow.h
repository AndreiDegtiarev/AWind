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
#include "Dialog.h"
#include "ATimer.h"
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
	LinkedList<ATimer> _timers;
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
	void RegisterTimer(ATimer *timer)
	{
		_timers.Add(timer);
	}
	LinkedList<ATimer> &Timers()
	{
		return _timers;
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