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
#include "CurrentmeterSensor.h"
#include "DS18B20Sensor.h"
#include "SensorManager.h"
#include "MeasurementNode.h"

#include "DC_GfxOLED.h""

#include "WindowsManager.h"
#include "ChargerOverviewWindow.h"
#include "ChargerVoltageChartWindow.h"
#include "ChargerTemperatureChartWindow.h"
#include "ChargerMainWindow.h"
#include "DefaultDecoratorsOLED.h"


#define OLED_RESET 4
#define VOLTAGE_PROBE_ONE_PIN  A0
#define VOLTAGE_PROBE_TWO_PIN  A1
#define MOSFET_PIN 9
#define BUTTON_PIN 2
#define CUTOFF_TIME 13


Adafruit_SSD1306 display(OLED_RESET);

DC_GfxOLED dc(&display);


//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors, NULL);

//Windows manager: container for GUI elements 
WindowsManager<ChargerMainWindow> windowsManager(&dc, NULL);

SensorManager voltageProbeTwo(new VoltmeterSensor(VOLTAGE_PROBE_TWO_PIN,-5.0f,5.0f), 0, 2, 1000 * 10);
SensorManager temperature(new DS18B20Sensor(8, 1), 0, 60, 1000 * 10);
SensorManager currentMeter(new CurrentmeterSensor(VOLTAGE_PROBE_ONE_PIN, VOLTAGE_PROBE_TWO_PIN, 10,5), 0, 1000, 1000 * 10);


ChargerOverviewWindow *overviewWindow;
ChargerVoltageChartWindow *voltageWindow;
ChargerTemperatureChartWindow *temperatureWindow;


ChargerController controller(&currentMeter,&voltageProbeTwo, &temperature, MOSFET_PIN,
							 200, //start current
	                         2500/7, //chrging rate
	                         2,     //cutoff voltgae
	                         35,     //cutoff temperature
					         CUTOFF_TIME);   //cuttof time, hours

uint8_t SmallOledFont[] = { 1 };
uint8_t BigOledFont[] = { 2 };

volatile bool btnState = false;
volatile unsigned long lastStateChange = 0;


void setup()
{
	out.begin(9600);
	out << (F("Setup")) << endln;

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.display();
	display.clearDisplay();

	pinMode(MOSFET_PIN, OUTPUT);

	sensors.Add(&temperature);
	sensors.Add(&voltageProbeTwo);
	sensors.Add(&currentMeter);

	voltageProbeTwo.initMinutesBuffer(CUTOFF_TIME*60*0.7);
	temperature.initMinutesBuffer(CUTOFF_TIME*60/3*0.7);


	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecoratorsOLED::InitAll();

	overviewWindow = new ChargerOverviewWindow(dc.DeviceWidth(), &currentMeter, &voltageProbeTwo, &temperature);
	voltageWindow= new ChargerVoltageChartWindow(dc.DeviceWidth(), dc.DeviceHeight(), &voltageProbeTwo);
	temperatureWindow = new ChargerTemperatureChartWindow(dc.DeviceWidth(), dc.DeviceHeight(), &temperature);


	//initialize window manager
	windowsManager.Initialize();

	windowsManager.MainWnd()->AddTab(overviewWindow);
	windowsManager.MainWnd()->AddTab(voltageWindow);
	windowsManager.MainWnd()->AddTab(temperatureWindow);

	voltageWindow->SetVisible(false);
	temperatureWindow->SetVisible(false);

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, LOW);

	AHelper::LogFreeRam();

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
	if (btnState)
	{
		out << "Stage changed" << endln;
		if (overviewWindow->IsVisible())
		{
			overviewWindow->SetVisible(false);
			voltageWindow->SetVisible(true);
			temperatureWindow->SetVisible(false);
		}
		else if(voltageWindow->IsVisible())
		{
			overviewWindow->SetVisible(false);
			voltageWindow->SetVisible(false);
			temperatureWindow->SetVisible(true);
		}
		else if (temperatureWindow->IsVisible())
		{
			overviewWindow->SetVisible(true);
			voltageWindow->SetVisible(false);
			temperatureWindow->SetVisible(false);
		}
		windowsManager.MainWnd()->Invalidate();
		btnState = false;
	}
	//give window manager an opportunity to update display
	windowsManager.loop();
}
void buttonPressed() 
{
	if (millis() - lastStateChange > 500)
	{
		lastStateChange = millis();
		btnState = true;
	}
}