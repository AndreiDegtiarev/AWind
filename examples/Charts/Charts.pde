/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  

  You can always find the latest version of the library at 
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

#include "OneWireSensor.h"
#include "WindowsManager.h"
#include "TouchManager.h"
#include "ViewModusWindow.h"

UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
extern uint8_t ArialNumFontPlus[];
//extern void loopTouch();

WindowsManager windowsManager(&myGLCD,loopTouch);
TouchManager touchManager(&myTouch,&windowsManager);


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

  windowsManager.loop();
}

