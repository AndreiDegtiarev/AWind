#pragma once
/*
  ASensor - Sensor library. Can be used standalone or in conjunction with AWind library
  Copyright (C)2015 Andrei Degtiarev. All right reserved
 
  You can always find the latest version of the library at 
  https://github.com/AndreiDegtiarev/ASensor

  This library is free software; you can redistribute it and/or
  modify it under the terms of the MIT license.
  Please see the included documents for further information.
*/
///LM35 Temp sensor. No any external libraries are neccesary. Details to member functions see ISensor class documentation
#include "ISensor.h"
class LM35Sensor : public ISensor
{
  int _port;
  float _temperatur;
public:
  LM35Sensor(int port)
  {
    _port=port;
    pinMode(_port,INPUT);

  }
  const __FlashStringHelper* Name()
  {
    return F("Temp");
  }
  float LowMeasurementLimit()
  {
    return -50;
  }
  float HighMeasurementLimit()
  {
    return 100;
  }
  int Precission()
  {
    return 1;
  }
  bool Measure(float &data)
  {

      _temperatur = (5.0 * analogRead(_port) / 10.24) - 273.15;; // using spec sheet curve

    data=_temperatur;
    return true;
  }
};
