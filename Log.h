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
#pragma once

class Endl {};
class Log
{
	bool _is_initialized;
public:
	Log()
	{
		_is_initialized=false;
	}
	void Init()
	{
		_is_initialized=true;
	}
	bool IsInitialized()
	{
		return _is_initialized;
	}
	template<class T1,class T2>
	static void Number(T1 *prefix,T2 &value,bool newLine=false)
	{
		if(prefix!=NULL)
			Serial.print(prefix);
		Serial.print(value);
		if(newLine)
			Line();
	}
	static void Line()
	{
		Serial.println("");
	}
	template<class T>
	static void Line(T value)
	{
		Serial.println(value);
	}
	friend Log& operator<<(Log &out,Endl &value)
	{
		if(out.IsInitialized())
			Serial.println();
	}
	/*template<class T>
	friend Log& operator<<(Log &out,T &value)
	{
		if(out.IsInitialized())
			Serial.print(value);
	}*/
	template<class T>
	friend Log& operator<<(Log &out,T value)
	{
		if(out.IsInitialized())
			Serial.print(value);
	}
};
extern Log out;
extern Endl endl;
