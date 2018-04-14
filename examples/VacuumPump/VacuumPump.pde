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

// DEMO_SENSORS allows run of this sketch in DEMO mode without real sensor connections 
#define DEMO_SENSORS

#include <Log.h>
#include <LinkedList.h>
#include <WindowsManager.h>
#include <DefaultDecorators.h>
#include <TabControl.h>

#ifndef DEMO_SENSORS
#include <DRV8825.h>
#include <OneWire.h>
#endif

#include "SensorManager.h"
#include "MeasurementNode.h"

#include "DC_UTFT.h"
#include <TouchUTFT.h>

#include "TabStart.h"
#include "TabSetup.h"
#include "TabProcess.h"


// Setup TFT display + touch (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32, 25, 26, 27, 28);
URTouch  myTouch(6, 5, 32, 3, 2);
#else
UTFT    myGLCD(ITDB32S, 39, 41, 43, 45);
URTouch  myTouch(49, 51, 53, 50, 52);
#endif

DC_UTFT dc(&myGLCD);
TouchUTFT touch(&myTouch);

//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors, NULL);

//manager which is responsible for window updating process
WindowsManager<MainWindow> windowsManager(&dc, &touch);

PumpController pumpController;


void setup() {

	//setup log (out is wrap about Serial class)
	out.begin(9600);
	out << F("Setup") << endln;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	//sensors
	pumpController.Settings().ActiveTime_ms  = 0.5 * 60 * 1000;
	pumpController.Settings().PauseTime_ms = 10 * 60 * 1000;
	pumpController.Settings().MinPressure_bar = 0.3;
	pumpController.Settings().MaxPressure_bar = 0.5;
	pumpController.Settings().MaxTemperature = 37;
	pumpController.Initialize(sensors);

	DC_UTFT::RegisterDefaultFonts();
	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecorators::InitAll();

	//initialize window manager
	windowsManager.Initialize();
	TabControl *tabCtrl = new TabControl(F("TabControl"), 2, 2, windowsManager.MainWnd()->Width() - 4, windowsManager.MainWnd()->Height() - 4);
	windowsManager.MainWnd()->AddChild(tabCtrl);

	//create tabs
	TabStart *tabStart = new TabStart(&pumpController,F("Window1"), 0, 0, 0, 0);
	TabProcess *tabProcess = new TabProcess(&pumpController, F("Window3"), 0, 0, 0, 0);
	TabSetup *tabSetup = new TabSetup(&pumpController, F("Window2"), 0, 0, 0, 0);
	tabCtrl->AddTab(F("Start"), tabStart);
	tabCtrl->AddTab(F("Proc."), tabProcess);
	tabCtrl->AddTab(F("Setup"), tabSetup);
	tabStart->Initialize(sensors);
	tabProcess->Initialize();

	DecoratorList *redDecorator=new DecoratorList();

	redDecorator->Add(new DecoratorRectFill(Color::Red, false));
	redDecorator->Add(new DecoratorColor(Color::Black));
	Environment::Get()->RegisterDecoratorsGroup(F("RedRectangle"), redDecorator);

	AHelper::LogFreeRam();
	delay(1000);
	out << F("End setup") << endln;
}

void loop() 
{
	//measure (if necessary -see delay parameter in sensor manager)
	if (measurementNode.Measure())
	{
		//following if is only for debugging purposes
		if (measurementNode.IsChanged())
		{
			//measurementNode.LogResults();
		}

	}
	//give window manager an opportunity to update display
	windowsManager.loop();
}
