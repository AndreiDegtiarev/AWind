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
#ifndef DEMO_SENSORS
#include <OneWire.h>
#endif

#include "Log.h"
#include "ISensor.h"
#include "VoltmeterSensor.h"
#include "CurrentmeterSensor.h"
#include "DS18B20Sensor.h"
#include "SensorManager.h"
#include "MeasurementNode.h"

#include "BatteryVoltmeterSensor.h"

#include "DC_GfxOLED.h"
#include "Adafruit_SSD1306.h"


#include "WindowsManager.h"
#include "ChargerOverviewWindow.h"
#include "ChargerVoltageChartWindow.h"
#include "ChargerTemperatureChartWindow.h"
#include "ChargerMainWindow.h"
#include "DefaultDecoratorsOLED.h"

#ifdef ESP32
#define MAX_VOLTAGE 3.3
#else
#define MAX_VOLTAGE 5.0
#endif


#define OLED_RESET 4
#ifdef ESP32
#include <driver/adc.h>
#include <pins_arduino.h>
#define VOLTAGE_PROBE_ONE_PIN ADC1_CHANNEL_0
#define VOLTAGE_PROBE_TWO_PIN ADC1_CHANNEL_3
#define MAX_VOLTAGE (3.3*2)
#define MOSFET_PIN 32
#define MOSFET_CHANNEL 0
#define BUTTON_PIN 25
#define REF_VOLTAGE 5.0f
#else
#define VOLTAGE_PROBE_ONE_PIN A0
#define VOLTAGE_PROBE_TWO_PIN A1
#define MAX_VOLTAGE 5.0
#define MOSFET_PIN 9
#define MOSFET_CHANNEL MOSFET_PIN
#define BUTTON_PIN 2
#define REF_VOLTAGE 5.84f
#endif
#define CUTOFF_TIME 13


Adafruit_SSD1306 display(OLED_RESET);

void displayWrp() { display.display(); }

DC_GfxOLED dc(&display, displayWrp);


//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors, NULL);

//Windows manager: container for GUI elements 
WindowsManager<ChargerMainWindow> windowsManager(&dc, NULL);

VoltmeterSensor probeTwoSensor(VOLTAGE_PROBE_TWO_PIN, -MAX_VOLTAGE, REF_VOLTAGE); // 5.84f);
CurrentmeterSensor currentSensor(VOLTAGE_PROBE_ONE_PIN, VOLTAGE_PROBE_TWO_PIN, 10, MAX_VOLTAGE);

DS18B20Sensor temperatureSensor(8, 1);

SensorManager voltageProbeTwo(&probeTwoSensor, 0, 10, 1000 * 10,false);
SensorManager temperature(&temperatureSensor, 0, 60, 1000 * 10,false);
SensorManager currentMeter(&currentSensor, 0, 1000, 1000 * 10, false);


ChargerOverviewWindow *overviewWindow;
ChargerVoltageChartWindow *voltageWindow;
ChargerTemperatureChartWindow *temperatureWindow;


ChargerController controller(&currentMeter,&voltageProbeTwo, &temperature, 
#ifdef ESP32
	MOSFET_CHANNEL,
#else
	MOSFET_PIN,
#endif
							 200, //start current
	                         390, //charging rate
	                         2,     //cutoff voltgae
	                         35,     //cutoff temperature
					         CUTOFF_TIME);   //cuttof time, hours

BatteryVoltmeterSensor batteryVoltsSensor(&controller, VOLTAGE_PROBE_TWO_PIN, -MAX_VOLTAGE, REF_VOLTAGE); // 5.84);
SensorManager batteryVoltage(&batteryVoltsSensor, 0, 2, 1000 * 25,false);

volatile bool btnState = false;
volatile unsigned long lastStateChange = 0;


void setup()
{
	out.begin(9600);
	out << (F("Setup")) << endln;

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.display();
	display.clearDisplay();

	sensors.Add(&temperature);
	sensors.Add(&voltageProbeTwo);
	sensors.Add(&currentMeter);
	sensors.Add(&batteryVoltage);

	batteryVoltage.initMinutesBuffer(CUTOFF_TIME*60*0.69);
	temperature.initMinutesBuffer(CUTOFF_TIME*60*0.69);

	DC_GfxOLED::RegisterDefaultFonts();
	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecoratorsOLED::InitAll();

	overviewWindow = new ChargerOverviewWindow(dc.DeviceWidth(), &currentMeter, &batteryVoltage, &temperature);
	voltageWindow= new ChargerVoltageChartWindow(dc.DeviceWidth(), dc.DeviceHeight(), &batteryVoltage);
	temperatureWindow = new ChargerTemperatureChartWindow(dc.DeviceWidth(), dc.DeviceHeight(), &temperature);


	//initialize window manager
	windowsManager.Initialize();

	windowsManager.MainWnd()->AddTab(overviewWindow);
	windowsManager.MainWnd()->AddTab(voltageWindow);
	windowsManager.MainWnd()->AddTab(temperatureWindow);

	voltageWindow->SetVisible(false);
	temperatureWindow->SetVisible(false);

	//pinMode(MOSFET_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);

#ifdef ESP32
	ledcSetup(MOSFET_CHANNEL, 12000, 8);
	ledcAttachPin(MOSFET_PIN, MOSFET_CHANNEL);
#endif

	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, HIGH);

#ifndef ESP32
	AHelper::LogFreeRam();
#endif

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
		out << "State changed" << endln;
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