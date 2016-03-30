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
class TabVacuum : public Window, public ITouchEventReceiver, public ISensorHasDataEventReceiver
{
	TextBoxNumber  *_txtPause;
	TextBoxNumber  *_txtActive;
	TextBoxNumber  *_txtPressure;
	TextBoxNumber  *_txtTemperature;
	Button         *_btnStart;
	Button         *_btnStop;
	GaugeBar       * _gaugeBar;
	TimerSensorManager    *_updateTimerManager;
	PumpController *_pumpController;
public:
	TabVacuum(PumpController *pumpController,const __FlashStringHelper * name, int left, int top, int width, int height) :Window(name, left, top, width, height)
	{
		_pumpController = pumpController;

		SetDecorators(Environment::Get()->FindDecorators(F("Window")));

		initLabel(new Label(0, 0, 0, 0, F("Active")));
		initLabel(new Label(0, 0, 0, 0, F("min.")));
		initLabel(new Label(0, 0, 0, 0, F("Pause")));
		initLabel(new Label(0, 0, 0, 0, F("min.")));
		initLabel(new Label(0, 0, 0, 0, F("Pressure")));
		initLabel(new Label(0, 0, 0, 0, F("bar")));
		initLabel(new Label(0, 0, 0, 0, F("Tempr")));
		initLabel(new Label(0, 0, 0, 0, F("grad.")));

		_txtActive = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtActive->SetNumber(0.5);
		initTextWindow(_txtActive,false);
		_txtPause = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtPause->SetNumber(10);
		initTextWindow(_txtPause, false);

		_txtPressure = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtPressure->SetNumber(0);
		initTextWindow(_txtPressure, true);

		_txtTemperature = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtTemperature->SetNumber(0);
		initTextWindow(_txtTemperature, true);

		_btnStart = new Button(0, 0, 0, 0, F("Start"));
		initButton(_btnStart);
		_btnStart->SetMargins(20, 20);
		_btnStop = new Button(0, 0, 0, 0, F("Stop"));
		initButton(_btnStop);
		_btnStop->SetMargins(30, 20);

		_pumpController->RegisterHasDataEventReceiver(this);

	}
	void initLabel(Label *label)
	{
		label->SetFont(BigFont);
		AddChild(label);
	}
	void initTextWindow(TextBoxNumber *wnd,bool isReadOnly)
	{
		wnd->SetDecorators(GetDecorators()); // here we save one decorator beacuse main window and text window have thae same decorator properties: black background
		wnd->SetFont(BigFont);
		wnd->SetMargins(5, 5);
		wnd->SetPrecission(1);
		wnd->SetIsReadOnly(isReadOnly);
		AddChild(wnd);
	}
	void initButton(Button *btn)
	{
		btn->RegisterTouchEventReceiver(this);
		AddChild(btn);
	}
	///Initilizes positions of child windows + timers
	void Initialize(LinkedList<SensorManager> &sensors)
	{
		int x = 1;
		int szx = Width() - 10;
		int szy = 30;
		int gauge_axis_margins = 5;
		DecoratorAxis *gaugeAxis = new DecoratorAxis(DecoratorAxis::HorizontalBottom, SmallFont, szx - gauge_axis_margins * 2, 0, 100, 5);
		gaugeAxis->SetOffset(szx, 10);
		_gaugeBar = new GaugeBar(gaugeAxis, 5, 115, szx, szy);
		_gaugeBar->SetFillColor(Color::LightGray);
		_gaugeBar->SetDecorators(Environment::Get()->FindDecorators(F("EditTextBoxReadOnly")));
		AddChild(_gaugeBar);

		const static int column_1_x = 25;
		const static int column_2_x = 172;
		const static int column_3_x = 248;
		const static int wnd_height = 25;
		const static int wnd_height_space = 2;
		const static int row_1_y = 5;
		const static int row_2_y = row_1_y + wnd_height + wnd_height_space;
		const static int row_3_y = row_2_y + wnd_height + wnd_height_space;
		const static int row_4_y = row_3_y + wnd_height + wnd_height_space;

		Children()[0]->Move(column_1_x, row_1_y, 50, wnd_height);
		Children()[1]->Move(column_3_x, row_1_y, 50, wnd_height);
		Children()[2]->Move(column_1_x, row_2_y+5, 50, wnd_height);
		Children()[3]->Move(column_3_x, row_2_y+5, 50, wnd_height);
		Children()[4]->Move(column_1_x, row_3_y + 5, 50, wnd_height);
		Children()[5]->Move(column_3_x, row_3_y + 5, 50, wnd_height);
		Children()[6]->Move(column_1_x, row_4_y + 5, 50, wnd_height);
		Children()[7]->Move(column_3_x, row_4_y + 5, 50, wnd_height);
		_txtActive->Move(column_2_x, row_1_y, 70, wnd_height);
		_txtPause->Move(column_2_x, row_2_y, 70, wnd_height);
		_txtPressure->Move(column_2_x, row_3_y, 70, wnd_height);
		_txtTemperature->Move(column_2_x, row_4_y, 70, wnd_height);

		_btnStart->Move(5, 150, 135, 50);
		_btnStop->Move(172, 150, 135, 50);

		//TimerSensor *updateTimer = new TimerSensor();
		_updateTimerManager = new TimerSensorManager();
		sensors.Add(_updateTimerManager);
		_updateTimerManager->SetAutoReset(true);
		_updateTimerManager->SetInterval(0.5*1000);
		_updateTimerManager->RegisterHasDataEventReceiver(this);
		_updateTimerManager->Enable();

	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if (window == _btnStart)
		{
			//_pumpController->initVacuumValve();
			_btnStart->SetDecorators(Environment::Get()->FindDecorators(F("RedRectangle")));

			//_activeTimer.SetInterval(_txtActive->GetNumber() * 60 * 1000);
			//_pauseTimer.SetInterval(_txtPause->GetNumber() * 60 * 1000);
			_txtActive->SetIsReadOnly(true);
			_txtPause->SetIsReadOnly(true);
			//_activeTimer.Enable();
			_gaugeBar->SetBarColor(Color::Red);
			_gaugeBar->SetMinMax(0, _txtActive->GetNumber()*60*1000);
			_gaugeBar->Invalidate();
			_txtActive->Invalidate();
			_txtPause->Invalidate();
			//_pumpController->startVacuum();
			_pumpController->StartTimerProgramm(_txtActive->GetNumber() * 60 * 1000, _txtPause->GetNumber() * 60 * 1000);
		}
		else if (window == _btnStop)
		{
			_pumpController->StopProgramm();
			//_pumpController->stopVacuum();
			_btnStart->SetDecorators(Environment::Get()->FindDecorators(F("Button")));
			_btnStart->Invalidate();
			//_activeTimer.Reset();
			//_pauseTimer.Reset();
			_gaugeBar->SetValue(0);
			_txtActive->SetIsReadOnly(false);
			_txtPause->SetIsReadOnly(false);
			_txtActive->Invalidate();
			_txtPause->Invalidate();

		}
	}
	///If sensor data was changed this notification is call
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		if (sensorManager == _updateTimerManager)
		{
			if (_pumpController->Programm() == PumpController::TimerControll)
			{
				if (_pumpController->Status() == PumpStatus::On)
				{
					_gaugeBar->SetValue(_pumpController->ElapsedActiveTime());
				}
				else if (_pumpController->Status() == PumpStatus::Off)
				{
					_gaugeBar->SetValue(_pumpController->ElapsedPauseTime());
				}
			}
		}
		else
		{
			switch (_pumpController->GetSensorType(sensorManager))
			{
			case PumpController::Temperature:
				_txtTemperature->SetNumber(sensorManager->GetData());
				break;
			case PumpController::Pressure:
				_txtPressure->SetNumber(sensorManager->GetData());
				break;
			case PumpController::ActiveTimer:
				_gaugeBar->SetValue(0);
				_gaugeBar->SetBarColor(Color::Green);
				_gaugeBar->SetMinMax(0, _txtPause->GetNumber() * 60 * 1000);
				break;
			case PumpController::PauseTimer:
				_gaugeBar->SetValue(0);
				_gaugeBar->SetBarColor(Color::Red);
				_gaugeBar->SetMinMax(0, _txtActive->GetNumber() * 60 * 1000);
				break;
			default:
				break;
			}
		}
	}
};