/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2016 Andrei Degtiarev. All right reserved
  
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
#include <Window.h>
#include <Label.h>
#include <TextBoxNumber.h>
#include <ATimer.h>
#include <GaugeBar.h>
#include "PumpController.h"

///Tab  that implements GUI to vacuum controller
class TabVacuum : public Window, public ITouchEventReceiver, public ITimerEventReceiver
{
	TextBoxNumber *_txtPause;
	TextBoxNumber *_txtActive;
	Button *_btnStart;
	Button *_btnStop;
	GaugeBar * _gaugeBar;
	ATimer  _pauseTimer;
	ATimer  _activeTimer;
	ATimer  _updateTimer;
	PumpController *_pumpController;
public:
	TabVacuum(PumpController *pumpController,const __FlashStringHelper * name, int left, int top, int width, int height) :Window(name, left, top, width, height)
	{
		_pumpController = pumpController;

		SetDecorators(Environment::Get()->FindDecorators(F("Window")));

		initLabel(new Label(0, 0, 0, 0, F("Active")));
		initLabel(new Label(0, 0, 0, 0, F("min")));
		initLabel(new Label(0, 0, 0, 0, F("Pause")));
		initLabel(new Label(0, 0, 0, 0, F("min")));

		_txtActive = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtActive->SetNumber(0.5);
		initTextWindow(_txtActive);
		_txtPause = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtPause->SetNumber(10);
		initTextWindow(_txtPause);

		_btnStart = new Button(0, 0, 0, 0, F("Start"));
		initButton(_btnStart);
		_btnStart->SetMargins(20, 20);
		_btnStop = new Button(0, 0, 0, 0, F("Stop"));
		initButton(_btnStop);
		_btnStop->SetMargins(30, 20);

	}
	void initLabel(Label *label)
	{
		label->SetFont(BigFont);
		AddChild(label);
	}
	void initTextWindow(TextBoxNumber *wnd)
	{
		wnd->SetDecorators(GetDecorators()); // here we save one decorator beacuse main window and text window have thae same decorator properties: black background
		wnd->SetFont(BigFont);
		wnd->SetMargins(5, 5);
		wnd->SetPrecission(1);
		wnd->SetIsReadOnly(false);
		AddChild(wnd);
	}
	void initButton(Button *btn)
	{
		btn->RegisterTouchEventReceiver(this);
		AddChild(btn);
	}
	///Initilizes positions of child windows + timers
	void Initialize()
	{
		int x = 1;
		int szx = Width() - 10;
		int szy = 35;
		int gauge_axis_margins = 5;
		DecoratorAxis *gaugeAxis = new DecoratorAxis(DecoratorAxis::HorizontalBottom, SmallFont, szx - gauge_axis_margins * 2, 0, 100, 5);
		gaugeAxis->SetOffset(szx, 10);
		_gaugeBar = new GaugeBar(gaugeAxis, 5, 105, szx, szy);
		_gaugeBar->SetFillColor(Color::LightGray);
		_gaugeBar->SetDecorators(Environment::Get()->FindDecorators(F("EditTextBoxReadOnly")));
		AddChild(_gaugeBar);

		Children()[0]->Move(5, 20, 50, 30);
		Children()[1]->Move(220, 20, 50, 30);
		Children()[2]->Move(5, 75, 50, 30);
		Children()[3]->Move(220, 75, 50, 30);
		_txtActive->Move(140, 15, 70, 30);
		_txtPause->Move(140, 70, 70, 30);

		_btnStart->Move(5, 150, 135, 50);
		_btnStop->Move(172, 150, 135, 50);

		_updateTimer.SetAutoReset(true);
		_updateTimer.SetInterval(1*1000);
		_activeTimer.RegisterTimerEventReceiver(this);
		_pauseTimer.RegisterTimerEventReceiver(this);
		_updateTimer.RegisterTimerEventReceiver(this);
		((MainWindow *)RootWindow())->RegisterTimer(&_activeTimer);
		((MainWindow *)RootWindow())->RegisterTimer(&_pauseTimer);
		((MainWindow *)RootWindow())->RegisterTimer(&_updateTimer);
		_updateTimer.Enable();

	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if (window == _btnStart)
		{
			_pumpController->initVacuumValve();
			_btnStart->SetDecorators(Environment::Get()->FindDecorators(F("RedRectangle")));
			_activeTimer.SetInterval(_txtActive->GetNumber() * 60 * 1000);
			_pauseTimer.SetInterval(_txtPause->GetNumber() * 60 * 1000);
			_txtActive->SetIsReadOnly(true);
			_txtPause->SetIsReadOnly(true);
			_activeTimer.Enable();
			_gaugeBar->SetBarColor(Color::Red);
			_gaugeBar->SetMinMax(0, _txtActive->GetNumber()*60*1000);
			_gaugeBar->Invalidate();
			_txtActive->Invalidate();
			_txtPause->Invalidate();
				_pumpController->startVacuum();
		}
		else if (window == _btnStop)
		{
			_pumpController->stopVacuum();
			_btnStart->SetDecorators(Environment::Get()->FindDecorators(F("Button")));
			_btnStart->Invalidate();
			_activeTimer.Reset();
			_pauseTimer.Reset();
			_gaugeBar->SetValue(0);
			_txtActive->SetIsReadOnly(false);
			_txtPause->SetIsReadOnly(false);
			_txtActive->Invalidate();
			_txtPause->Invalidate();

		}
	}
	///Events routing for timer
	void NotifyTimer(ATimer *timer)
	{
		if (timer == &_updateTimer)
		{
			if (_activeTimer.IsEnabled())
			{
				out << F("Active timer: ")<<_activeTimer.Elapsed()<<F(" ");
				_gaugeBar->SetValue(_activeTimer.Elapsed());
			}
			else if (_pauseTimer.IsEnabled())
			{
				out << F("Pause timer: ") << _activeTimer.Elapsed() << F(" ");
				_gaugeBar->SetValue(_pauseTimer.Elapsed());
			}
		}
		else if (timer == &_activeTimer)
		{
			_pumpController->stopVacuum();
			_activeTimer.Reset();
			_pauseTimer.Enable();
			_gaugeBar->SetValue(0);
			_gaugeBar->SetBarColor(Color::Green);
			_gaugeBar->SetMinMax(0, _txtPause->GetNumber() * 60 * 1000);
		}
		else if (timer == &_pauseTimer)
		{
			_pauseTimer.Reset();
			_activeTimer.Enable();
			_gaugeBar->SetValue(0);
			_gaugeBar->SetBarColor(Color::Red);
			_gaugeBar->SetMinMax(0, _txtActive->GetNumber() * 60 * 1000);
			_pumpController->startVacuum();
		}

	}
};