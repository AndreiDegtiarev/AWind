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
// DEMO_SENSORS allows run of this sketch in DEMO mode without real sensor connections 
#define DEMO_SENSORS
#include <UTFT.h>
#include <UTouch.h>

#include "LinkedList.h"
#include "WindowsManager.h"
#include "GaugesWindow.h"
#include "FakeSensor.h"
#include "SensorManager.h"
#include "MeasurementNode.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);

//manager which is responsible for window updating process
WindowsManager<GaugesWindow> windowsManager(&myGLCD,&myTouch);

//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors);


void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out<<F("Setup")<<endl;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	//initialize window manager
	windowsManager.Initialize();

	//create sensor manager
	SensorManager *manager=new SensorManager(new FakeSensor(),15,40,1000*1);
	sensors.Add(manager); 
	manager->RegisterHasDataEventReceiver(windowsManager.MainWnd());
	manager->RegisterMeasuredEventReceiver(windowsManager.MainWnd());
	//in order to avoid pause in the touch interactions, windows manager is defined as critical process
	measurementNode.SetCriticalProcess(&windowsManager);

	delay(1000); 
	out<<F("End setup")<<endl;

}

void loop()
{
	//measure (if necessary -see delay parameter in sensor manager)
	if(measurementNode.measure())
	{
		//following if is only for debugging purposes
		if(measurementNode.IsChanged())
		{
			measurementNode.printSerial();
		}

	}
	//give window manager an opportunity to update display
	windowsManager.loop();
}