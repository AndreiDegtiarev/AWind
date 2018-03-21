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
#include "TouchUTFT.h"

//#define DEBUG_AWIND

#include "Log.h"
#include "LinkedList.h"
#include "WindowsManager.h"
#include "DefaultDecorators.h"
#include "Window1.h"
#include "Window2.h"
#include "Window3.h"

#include "TabControl.h"

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

//manager which is responsible for window updating process
WindowsManager<MainWindow> windowsManager(&dc, &touch);


void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(9600);
	out << F("Setup") << endln;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	//pinMode(47, OUTPUT);
	//digitalWrite(47, HIGH);

	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecorators::InitAll();

	//initialize window manager
	windowsManager.Initialize();
	TabControl *tabCtrl = new TabControl(F("TabControl"), 2, 2, windowsManager.MainWnd()->Width()-4, windowsManager.MainWnd()->Height()-4);
	windowsManager.MainWnd()->AddChild(tabCtrl);

	//create tabs
	Window1 *window1 = new Window1(F("Window1"), 0, 0, 0, 0);
	Window2 *window2 = new Window2(F("Window2"), 0, 0, 0, 0);
	Window3 *window3 = new Window3(F("Window3"), 0, 0, 0, 0);
	tabCtrl->AddTab(F("Tab 1"), window1);
	tabCtrl->AddTab(F("Tab 2"), window2);
	tabCtrl->AddTab(F("Tab 3"), window3);

	AHelper::LogFreeRam();
	delay(1000);
	out << F("End setup") << endln;

}

void loop()
{
	//give window manager an opportunity to update display
	windowsManager.loop();
}