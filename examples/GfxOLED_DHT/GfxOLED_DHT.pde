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

#include <DHT.h>

#include "ISensor.h"
#include "DHTHumiditySensor.h"
#include "SensorManager.h"
#include "MeasurementNode.h"

#include "DC_GfxOLED.h""
#include "Adafruit_SSD1306.h"

#include "WindowsManager.h"
#include "Log.h"
#include "GfxOLED_DHT_Window.h"
#include "DefaultDecoratorsOLED.h"


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void displayWrp() { display.display(); }

DC_GfxOLED dc(&display, displayWrp);


//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors, NULL);

//Windows manager: container for GUI elements 
WindowsManager<GfxOLED_DHT_Window> windowsManager(&dc,NULL);

int dht_pin = 8;
DHTTemperatureSensor inTempr(dht_pin, DHTTemperatureSensor::DHT11);
DHTHumiditySensor inHumidity(&inTempr);
SensorManager inTemprManager(&inTempr, 15, 40, 1000 * 1);
SensorManager inHumidityManager(&inHumidity, 0, 80, 1000 * 1);

void setup()
{

	out.begin(9600);
	out<<(F("Setup"))<<endln;

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.display();
	display.clearDisplay();

	DC_GfxOLED::RegisterDefaultFonts();
	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecoratorsOLED::InitAll();

	sensors.Add(&inTemprManager); //0
	sensors.Add(&inHumidityManager); //1
	inTemprManager.initSecondsBuffer(100);

	//initialize window manager
	windowsManager.Initialize();
	windowsManager.MainWnd()->Initialize(&inTemprManager);
	inTemprManager.RegisterHasDataEventReceiver(windowsManager.MainWnd());
	inHumidityManager.RegisterHasDataEventReceiver(windowsManager.MainWnd());


	out<<F("End setup")<<endln;

}

void loop()
{
	//measure (if necessary -see delay parameter in sensor manager)
	if (measurementNode.Measure())
	{
		//following if is only for debugging purposes
		if (measurementNode.IsChanged())
		{
			measurementNode.LogResults();
		}

	}
	//give window manager an opportunity to update display
	windowsManager.loop();
}

