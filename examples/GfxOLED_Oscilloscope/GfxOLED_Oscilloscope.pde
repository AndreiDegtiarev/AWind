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

#include "Log.h"
#include "ISensor.h"
#include "VoltmeterSensor.h"
#include "SensorManager.h"
#include "MeasurementNode.h"

#include "DC_GfxOLED.h""

#include "WindowsManager.h"
#include "GfxOLED_Oscilloscope.h"
#include "DefaultDecoratorsOLED.h"


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

DC_GfxOLED dc(&display);


//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors, NULL);

//Windows manager: container for GUI elements 
WindowsManager<GfxOLED_Oscilloscope> windowsManager(&dc, NULL);

VoltmeterSensor voltmeter(A0, 2000, 200);
SensorManager voltmeterManager(&voltmeter, 15, 40, 1000 * 1);
uint8_t SmallOledFont[] = { 1 };
uint8_t BigOledFont[] = { 2 };

void setup()
{
	out.begin(9600);
	out << (F("Setup")) << endln;

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.display();
	display.clearDisplay();

	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecoratorsOLED::InitAll();

	//initialize window manager
	windowsManager.Initialize();

	//create voltmeter sensor that measures analog pin A0
	voltmeter.SetTimeStep(100);

	windowsManager.MainWnd()->Initialize(&voltmeterManager);
	voltmeterManager.RegisterHasDataEventReceiver(windowsManager.MainWnd());
	AHelper::LogFreeRam();

	out<<F("End setup")<<endln;

}

void loop()
{
	//measure data
	voltmeter.MeasureBuffer();
	//initialize chart window with measured data 
	windowsManager.MainWnd()->ChartWnd()->SetBuffer(voltmeter.Buffer());
	windowsManager.MainWnd()->ChartWnd()->Invalidate();
	//give window manager an opportunity to update display
	windowsManager.loop();
}