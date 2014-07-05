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

#define DEMO_SENSORS

#ifndef DEMO_SENSORS
#include <OneWire.h>
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>
#endif

#include <UTFT.h>
#include <UTouch.h>

#include "ISensor.h"
#include "SensorManager.h"
#include "DS18B20Sensor.h"
#include "DHTTemperatureSensor.h"
#include "DHTHumiditySensor.h"
#include "BMP085Sensor.h"
#include "MeasurementNode.h"

#include "WindowsManager.h"
#include "TouchManager.h"
#include "ViewModusWindow.h"
#include "MeasurementNode.h"

UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
extern uint8_t ArialNumFontPlus[];

int temperature_port=10;

LinkedList<SensorManager> sensors;
MeasurementNode measurementNode(sensors,loopTouch); //,num_sensors);

WindowsManager windowsManager(&myGLCD,loopTouch);
TouchManager touchManager(&myTouch,&windowsManager);

LinkedList<SensorWindow> vis_sensors;


unsigned long timer;

void setup()
{
	Serial.begin(57600);
	Serial.println(F("Setup"));

	myGLCD.InitLCD();
	myGLCD.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myGLCD.setFont(ArialNumFontPlus);
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	DHTTemperatureSensor *inTempr=new DHTTemperatureSensor(temperature_port+2);
	DHTHumiditySensor *inHumidity=new DHTHumiditySensor(inTempr);
	DHTTemperatureSensor *alkovenTempr=new DHTTemperatureSensor(temperature_port-2);
	DHTHumiditySensor *alkovenHumidity=new DHTHumiditySensor(alkovenTempr);
	BMP085Sensor *pressure=new BMP085Sensor();
	DS18B20Sensor *fridgeTempr=new DS18B20Sensor(temperature_port,1);
	DS18B20Sensor *outTempr=new DS18B20Sensor(temperature_port,2);

	sensors.Add(new SensorManager(inTempr,15,40,1000*10));
	sensors.Add(new SensorManager(inHumidity,0,80,1000*10));
	sensors.Add(new SensorManager(alkovenTempr,15,40,1000*10));
	sensors.Add(new SensorManager(alkovenHumidity,0,80,1000*10));
	sensors.Add(new SensorManager(pressure,700,1300,1000*10));
	sensors.Add(new SensorManager(fridgeTempr,-5,16,1000*10));
	sensors.Add(new SensorManager(outTempr,-30,40,1000*10));


	int second_column = SensorWindow::BigWindowWidth+SensorWindow::Margin/2;
	int second_row=SensorWindow::BigWindowHeight+SensorWindow::Margin/2;
	vis_sensors.Add(new SensorWindow(F("In Temp"),sensors[0],0,0));
	vis_sensors.Add(new SensorWindow(F("In Humid"),sensors[1],second_column,0));
	vis_sensors.Add(new SensorWindow(F("Out"),sensors[6],0,second_row));
	int third_column = second_column + SensorWindow::SmallWindowWidth+SensorWindow::Margin/2;
	int third_row = second_row + SensorWindow::SmallWindowHeight+SensorWindow::Margin/2;
	vis_sensors.Add(new SensorWindow(F("Pressure"),sensors[4],second_column,second_row,SensorWindow::Small));
	vis_sensors.Add(new SensorWindow(F("Fridge"),sensors[5],third_column,second_row,SensorWindow::Small));
	vis_sensors.Add(new SensorWindow(F("Alk Temp"),sensors[2],second_column,third_row,SensorWindow::Small));
	vis_sensors.Add(new SensorWindow(F("Alk Humid"),sensors[3],third_column,third_row,SensorWindow::Small));
	for(int i=0;i<vis_sensors.Count();i++)
		windowsManager.MainWindow()->AddChild(vis_sensors[i]);
	windowsManager.MainWindow()->AddChild(new ViewModusWindow(vis_sensors,windowsManager.MainWindow()->Width()-100,windowsManager.MainWindow()->Height()-45,95,35));


	delay(1000); 
	Serial.println("End setup");

}
void loopTouch()
{
  touchManager.loop();
}
void loop()
{
  loopTouch();
  if(measurementNode.measure())
  {
	  if(measurementNode.IsChanged())
	  {
		  measurementNode.printSerial();
		  for(int i=0;i<vis_sensors.Count();i++)
			  vis_sensors[i]->OnUpdate();
	  }

  }
  windowsManager.loop();
}

