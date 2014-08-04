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

#include <math.h>
#include "Log.h"
#include "IDataBuffer.h"


class TimeSerieBuffer : public IDataBuffer
{
	int _reserved_size;
	int _size;
	int   *_data_y;
	float _time_step;
	float _factor_y;
public:
	TimeSerieBuffer(float time_step,float factor_y,int reserved_size,int size)
	{
		initialize(time_step,factor_y,reserved_size,size);
	}
	TimeSerieBuffer(float time_step,float factor_y,int size)
	{
		initialize(time_step,factor_y,size,size);

	}
protected:
	void initialize(float time_step,float factor_y,int reserved_size,int size)
	{
		_factor_y=factor_y;
		_size=size;
		_reserved_size=reserved_size;
		_time_step=time_step;
		_data_y=new int[reserved_size];
		for(int i=0;i<_size;i++)
			_data_y[i]=0;
	}
public:
	bool SetSize(int size)
	{
		if(size<=_reserved_size)
			_size=size;
		else
		{
			Serial.println("Error: buffer size is too big");
			return false;
		}
		return true;
	}
	float SetTimeStep(float time_step)
	{
		_time_step=time_step;
	}
	float SetFactorY(float factor_y)
	{
		_factor_y=factor_y;
	}
	void Set(unsigned int index,float value)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endl;
			return;
		}
		_data_y[index]=(value*_factor_y);
	}
	int *Y()
	{
		return _data_y;
	}
	void MinMax(float &min_x,float &max_x,float &min_y,float &max_y)
	{
 		min_x=0;
		max_x=_time_step*_size;
 		min_y=_data_y[_size-1];
		max_y=_data_y[_size-1];
		for(int i=0;i<_size;i++)
		{
			min_y=min(min_y,_data_y[i]);
			max_y=max(max_y,_data_y[i]);
		}
		min_y/=_factor_y;
		max_y/=_factor_y;
	}
	float X(unsigned int index)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endl;
			return 0;
		}
		return _time_step*index;
	}
	float Y(unsigned int index)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endl;
			return 0;
		}
		return _data_y[index]/_factor_y;
	}
	unsigned int StartIndex()
	{
		return 0;
	}
	unsigned int Size()
	{
		return _size;
	}
};