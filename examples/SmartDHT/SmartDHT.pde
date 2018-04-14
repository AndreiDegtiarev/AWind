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
#define DEMO_SENSORS

#include "DC_UTFT.h"
#include "TouchUTFT.h"
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
URTouch  myTouch(6,5,32,3,2);
#else
UTFT    myGLCD(ITDB32S,39,41,43,45);
URTouch  myTouch( 49, 51, 53, 50, 52);
#endif

DC_UTFT dc(&myGLCD);
TouchUTFT touch(&myTouch);

//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors,NULL);


//manager which is responsible for window updating process
WindowsManager<ViewModusWindow> windowsManager(&dc,&touch);

extern uint8_t ArialNumFontPlus[];

void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(9600);
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

	DC_UTFT::RegisterDefaultFonts();
	Environment::Get()->RegisterFont(new AFontUTFT(F("BigPlus"), ArialNumFontPlus));
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