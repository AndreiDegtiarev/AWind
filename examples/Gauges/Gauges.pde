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

// DEMO_SENSORS allows run of this sketch in DEMO mode without real sensor connections 
#define DEMO_SENSORS
//#define DEBUG_AWIND
#include "DC_UTFT.h"
#include "TouchUTFT.h"

#include "Log.h"
#include "GaugesWindow.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
#if defined _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32,25,26,27,28);
URTouch  myTouch(6,5,32,3,2);
#else
UTFT    myGLCD(ITDB32S,39,41,43,45);
URTouch  myTouch( 49, 51, 53, 50, 52);
#endif

DC_UTFT dc(&myGLCD);
TouchUTFT touch(&myTouch);

//manager which is responsible for window updating process
WindowsManager<GaugesWindow> windowsManager(&dc,&touch);


void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(9600);
	out<<F("Setup")<<endln;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	DC_UTFT::RegisterDefaultFonts();

	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	//pinMode(47,OUTPUT);
	//digitalWrite(47,HIGH);


	setupExample(windowsManager);

	delay(1000); 
	out<<F("End setup")<<endln;

}

void loop()
{
	loopExample(windowsManager);
}