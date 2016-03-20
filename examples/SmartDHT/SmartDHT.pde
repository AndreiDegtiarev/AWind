/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  
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
#define DEMO_SENSORS

#include <UTFT.h>
#include <UTouch.h>
#include <DHT.h>

#include "AHelper.h"
#include "ISensor.h"

#include "DHTHumiditySensor.h"
#include "SensorManager.h"

#include "LinkedList.h"
#include "WindowsManager.h"
#include "SensorWindow.h"
#include "MeasurementNode.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32,25,26,27,28);
UTouch  myTouch(6,5,32,3,2);
#else
UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
#endif

//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors,NULL);


//manager which is responsible for window updating process
WindowsManager<ViewModusWindow> windowsManager(&myGLCD,&myTouch);


void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out<<F("Setup")<<endln;

	//initialize display
	myGLCD.InitLCD(PORTRAIT);
	//myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	//pinMode(47,OUTPUT);
	//digitalWrite(47,HIGH);

	//initialize window manager
	windowsManager.Initialize();

	//sensors
	int dht_pin=8;
	DHTTemperatureSensor *inTempr=new DHTTemperatureSensor(dht_pin, DHTTemperatureSensor::DHT11);
	DHTHumiditySensor *inHumidity=new DHTHumiditySensor(inTempr);
	sensors.Add(new SensorManager(inTempr,15,40,1000*10)); //0
	sensors.Add(new SensorManager(inHumidity,0,80,1000*10)); //1

	int screenWidth=windowsManager.MainWnd()->Width()-1;
	int screenHeight=windowsManager.MainWnd()->Height();
	windowsManager.MainWnd()->AddChild(new SensorWindow(F("In Temp"),sensors[0],1,1,screenWidth,screenHeight/2-5));
	windowsManager.MainWnd()->AddChild(new SensorWindow(F("In Humid"),sensors[1],1,screenHeight/2,screenWidth,screenHeight/2-5));

	windowsManager.MainWnd()->Initialize();
	windowsManager.MainWnd()->SetModus(ViewModusWindow::Day);

	//in order to avoid pause in the touch interactions, windows manager is defined as critical process
	measurementNode.SetCriticalProcess(&windowsManager);

	delay(1000); 
	out<<F("End setup")<<endln;

}

void loop()
{
	//measure (if necessary -see delay parameter in sensor manager)
	if(measurementNode.Measure())
	{
		//following if is only for debugging purposes
		if(measurementNode.IsChanged())
		{
			measurementNode.LogResults();
		}

	}
	//give window manager an opportunity to update display
	windowsManager.loop();
}