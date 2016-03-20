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
#include <UTFT.h>
#include <UTouch.h>

// DEMO_SENSORS allows run of this sketch in DEMO mode without real sensor connections 
#define DEMO_SENSORS

#include "Log.h"
#include "LinkedList.h"
#include "WindowsManager.h"
#include "DefaultDecorators.h"

#ifndef DEMO_SENSORS
#include <DRV8825.h>
#endif

#include "TabVacuum.h"
#include "TabManual.h"
#include "TabTemperature.h"
#include "TabControl.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32, 25, 26, 27, 28);
UTouch  myTouch(6, 5, 32, 3, 2);
#else
UTFT    myGLCD(ITDB32S, 39, 41, 43, 45);
UTouch  myTouch(49, 51, 53, 50, 52);
#endif

//manager which is responsible for window updating process
WindowsManager<MainWindow> windowsManager(&myGLCD, &myTouch);

PumpController pumpController;


void setup() {

	pumpController.stopPump();

	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out << F("Setup") << endln;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecorators::InitAll();

	//initialize window manager
	windowsManager.Initialize();
	TabControl *tabCtrl = new TabControl(F("TabControl"), 2, 2, windowsManager.MainWnd()->Width() - 4, windowsManager.MainWnd()->Height() - 4);
	windowsManager.MainWnd()->AddChild(tabCtrl);

	//create tabs
	TabVacuum *window1 = new TabVacuum(&pumpController,F("Window1"), 0, 0, 0, 0);
	TabManual *window2 = new TabManual(&pumpController, F("Window2"), 0, 0, 0, 0);
	TabTemperature *window3 = new TabTemperature(F("Window3"), 0, 0, 0, 0);
	tabCtrl->AddTab(F("Vacuum"), window1);
	tabCtrl->AddTab(F("Manual"), window2);
	tabCtrl->AddTab(F("Tempr."), window3);
	window1->Initialize();
	window3->Initialize();

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
	//give window manager an opportunity to update display
	windowsManager.loop();
}
