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

#include "TextBoxString.h"
#include "TextBoxNumber.h"
#include "DecoratorPrimitives.h"
#include "ChartWindow.h"
#include "ChargerController.h"


extern uint8_t SmallOledFont[];
extern uint8_t BigOledFont[];

///Osciloscope main window
class ChargerMainWindwo : public MainWindow, public  ISensorHasDataEventReceiver
{
	SensorManager *_inTemprManager;;
	ChartWindow *_chartWnd;
	DecoratorAxis *_chartYAxis;
public:
	ChargerMainWindwo(int width,int height):MainWindow(width, height)
	{
		_chartYAxis = new DecoratorAxis(DecoratorAxis::VerticalLeft, SmallOledFont, height - 5, 0, 6, 3);
		_chartYAxis->SetOffset(0, 0);
		_chartWnd = new ChartWindow(NULL, _chartYAxis, 0, 0, width - 5, height - 5);
		_chartWnd->AddDecorator(new DecoratorColor(Color::Black));
		_chartWnd->AddDecorator(_chartYAxis);
		AddChild(_chartWnd);
	}
	ChartWindow *ChartWnd()
	{
		return _chartWnd;
	}
	void Initialize(SensorManager *inTemprManager)
	{
		_inTemprManager = inTemprManager;
		_chartWnd->SetBuffer(inTemprManager->SecBuffer());
	}
	///Event sensor has new data. If data is the same as measured previosly. This event is not generated (see RegisterReceiver and public ISensorHasDataEventReceiver declaration)
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		if (sensorManager == _inTemprManager)
		{
			_chartWnd->Invalidate();
		}
	}
};