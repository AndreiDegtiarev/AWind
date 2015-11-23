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

#include <UTFT.h>
#include <UTouch.h>

#include "Log.h"
#include "AHelper.h"
#include "LinkedList.h"
#include "WindowsManager.h"
#include "DecoratorPrimitives.h"
#include "Window1.h"
#include "Window2.h"
#include "Window3.h"
#include "Window4.h"
#include "DefaultDecorators.h"
#include "WindowSelector.h"
#include "SensorManager.h"
#include "MeasurementNode.h"
#include "LM35Sensor.h"


// Setup TFT display + touch (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32,25,26,27,28);
UTouch  myTouch(6,5,32,3,2);
#else
UTFT         myGLCD(SSD1289,38,39,40,41); //ITDB32S TFT01_32  SSD1289
UTouch       myTouch(6,5,4,3,2);
#endif


//manager which is responsible for window updating process
WindowsManager<WindowSelector> windowsManager(&myGLCD,&myTouch);
//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
DefaultDecorators defDecoratos(DefaultDecorators::all);
//list where all sensors are collected
LinkedList<SensorManager> sensors;

//manager which controls the measurement process
MeasurementNode measurementNode(sensors,NULL);

const char wind1Name[] PROGMEM = "Window1";
const char wind2Name[] PROGMEM = "Window2";
const char wind3Name[] PROGMEM = "Window3";
const char wind4Name[] PROGMEM = "Window4";
Window1 window1((const __FlashStringHelper *)wind1Name,0,0,0,0);
Window2 window2((const __FlashStringHelper *)wind2Name,0,0,0,0);
Window3 window3((const __FlashStringHelper *)wind3Name,0,0,0,0);
Window4 window4((const __FlashStringHelper *)wind4Name,0,0,0,0);
	//Sensors
LM35Sensor DC1Temp1(temperature_port);
LM35Sensor DC1Temp2(temperature_port+1);
LM35Sensor DC1Temp3(temperature_port+2);

SensorManager sensorManagerDC1Temp1(&DC1Temp1,5,100,1000*10);
SensorManager sensorManagerDC1Temp2(&DC1Temp2,5,100,1000*10);
SensorManager sensorManagerDC1Temp3(&DC1Temp3,5,100,1000*10);

void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out<<F("Setup")<<endln;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	//initialize window manager
	windowsManager.Initialize();

	//create tabs
	windowsManager.MainWnd()->Initialize(); 
	windowsManager.MainWnd()->AddTab(F("Main"),&window1);
	windowsManager.MainWnd()->AddTab(F("Temp"),&window2);
	windowsManager.MainWnd()->AddTab(F("Volt"),&window3);
	windowsManager.MainWnd()->AddTab(F("SWR"),&window4);

	//sensor managers. Manager defines measurement limits and measurement delays
	sensors.Add(&sensorManagerDC1Temp1);
	window2.AttachSensor(&sensorManagerDC1Temp1);
	sensors.Add(&sensorManagerDC1Temp2);
	window2.AttachSensor(&sensorManagerDC1Temp2);
	sensors.Add(&sensorManagerDC1Temp3);
	window2.AttachSensor(&sensorManagerDC1Temp3);
	//in order to avoid pause in the touch interactions, windows manager is defined as critical process
	measurementNode.SetCriticalProcess(&windowsManager);

	delay(1000); 

	AHelper::LogFreeRam();
	out<<F("End setup")<<endln;

}

void loop()
{
	//give window manager an opportunity to update display
	windowsManager.loop();
 
  //measure (if necessary -see delay parameter in sensor manager)
  if(measurementNode.Measure())
  {
    if(measurementNode.IsChanged())
    {
      measurementNode.LogResults();
    }
  } 
}

