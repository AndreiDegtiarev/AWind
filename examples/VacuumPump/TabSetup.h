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
#include <Window.h>
#include "PumpController.h"
///Tab for manual control of the pump
class TabSetup : public Window, public IContentChangedEventReceiver
{
	TextBoxNumber  *_txtPause;
	TextBoxNumber  *_txtActive;
	TextBoxNumber  *_txtMinPressure;
	TextBoxNumber  *_txtMaxPressure;
	TextBoxNumber  *_txtMaxTemperature;
	PumpController *_pumpController;
public:
	TabSetup(PumpController *pumpController, const __FlashStringHelper * name, int left, int top, int width, int height) :Window(name, left, top, width, height)
	{
		_pumpController = pumpController;

		SetDecorators(Environment::Get()->FindDecorators(F("Window")));

		initLabel(new Label(0, 0, 0, 0, F("Active")));
		initLabel(new Label(0, 0, 0, 0, F("min.")));
		initLabel(new Label(0, 0, 0, 0, F("Pause")));
		initLabel(new Label(0, 0, 0, 0, F("min.")));
		initLabel(new Label(0, 0, 0, 0, F("Min vacuum")));
		initLabel(new Label(0, 0, 0, 0, F("bar")));
		initLabel(new Label(0, 0, 0, 0, F("Max vacuum")));
		initLabel(new Label(0, 0, 0, 0, F("bar")));
		initLabel(new Label(0, 0, 0, 0, F("Max temper.")));
		initLabel(new Label(0, 0, 0, 0, F("grad")));

		_txtActive = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtActive->SetNumber(pumpController->Settings().ActiveTime_ms/1000.0/60.0);
		initTextWindow(_txtActive, 1);
		_txtPause = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtPause->SetNumber(pumpController->Settings().PauseTime_ms / 1000.0 / 60.0);
		initTextWindow(_txtPause, 1);

		_txtMinPressure = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtMinPressure->SetNumber(pumpController->Settings().MinPressure_bar);
		initTextWindow(_txtMinPressure, 2);

		_txtMaxPressure = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtMaxPressure->SetNumber(pumpController->Settings().MaxPressure_bar);
		initTextWindow(_txtMaxPressure, 2);

		_txtMaxTemperature = new TextBoxNumber(0, 0, 0, 0, 0);
		_txtMaxTemperature->SetNumber(pumpController->Settings().MaxTemperature);
		initTextWindow(_txtMaxTemperature, 1);
	}
	void initLabel(Label *label)
	{
		label->SetFont(F("Big"));
		AddChild(label);
	}
	void initTextWindow(TextBoxNumber *wnd, int precission)
	{
		wnd->SetDecorators(GetDecorators()); // here we save one decorator beacuse main window and text window have thae same decorator properties: black background
		wnd->SetFont(F("Big"));
		wnd->SetMargins(5, 5);
		wnd->SetPrecission(precission);
		wnd->SetIsReadOnly(false);
		wnd->RegisterContentChangedReceiver(this);
		AddChild(wnd);
	}
	virtual void Move(int left, int top, int width, int height)
	{
		Window::Move(left, top, width, height);

		const static int column_1_x = 5;
		const static int column_2_x = 172;
		const static int column_3_x = 248;
		const static int wnd_height = 25;
		const static int wnd_height_space = 10;
		const static int row_1_y = 20;
		const static int row_2_y = row_1_y + wnd_height + wnd_height_space;
		const static int row_3_y = row_2_y + wnd_height + wnd_height_space;
		const static int row_4_y = row_3_y + wnd_height + wnd_height_space;
		const static int row_5_y = row_4_y + wnd_height + wnd_height_space;

		Children()[0]->Move(column_1_x, row_1_y, 50, wnd_height);
		Children()[1]->Move(column_3_x, row_1_y, 50, wnd_height);
		Children()[2]->Move(column_1_x, row_2_y + 5, 50, wnd_height);
		Children()[3]->Move(column_3_x, row_2_y + 5, 50, wnd_height);
		Children()[4]->Move(column_1_x, row_3_y + 5, 50, wnd_height);
		Children()[5]->Move(column_3_x, row_3_y + 5, 50, wnd_height);
		Children()[6]->Move(column_1_x, row_4_y + 5, 50, wnd_height);
		Children()[7]->Move(column_3_x, row_4_y + 5, 50, wnd_height);
		Children()[8]->Move(column_1_x, row_5_y + 5, 50, wnd_height);
		Children()[9]->Move(column_3_x, row_5_y + 5, 50, wnd_height);

		_txtActive->Move(column_2_x, row_1_y, 70, wnd_height);
		_txtPause->Move(column_2_x, row_2_y, 70, wnd_height);
		_txtMinPressure->Move(column_2_x, row_3_y, 70, wnd_height);
		_txtMaxPressure->Move(column_2_x, row_4_y, 70, wnd_height);
		_txtMaxTemperature->Move(column_2_x, row_5_y, 70, wnd_height);


	}
	virtual void NotifyContentChanged(Window *window)
	{
		//out << F("TabSetup::NotifyContentChanged") << endln;
		_pumpController->Settings().ActiveTime_ms  = _txtActive->GetNumber() * 60 * 1000;
		_pumpController->Settings().PauseTime_ms = _txtPause->GetNumber() * 60 * 1000;
		_pumpController->Settings().MinPressure_bar = _txtMinPressure->GetNumber();
		_pumpController->Settings().MaxTemperature = _txtMaxTemperature->GetNumber();
		_pumpController->InvalidateSettings();
	}
};