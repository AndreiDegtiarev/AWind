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
#include "DataBufferBase.h"


template <class Tx,class Ty> class SensorDataBuffer : public DataBufferBase
{
	int _size;
	Tx   *_data_x;
	Ty   *_data_y;
	int _startIndex;
	float _factor_x;
	float _factor_y;
public:
	SensorDataBuffer(float factor_x,float factor_y,int size)
	{
		_factor_x=factor_x;
		_factor_y=factor_y;
		_size=size;

		_data_x=new Tx[_size];
		_data_y=new Ty[_size];
		for(int i=0;i<_size;i++)
		{
			_data_x[i]=0;
			_data_y[i]=0;
		}
		_startIndex = _size;

	}
	Tx *X()
	{
		return _data_x;
	}
	Ty *Y()
	{
		return _data_y;
	}
	void AddValue(float value_x,float value_y)
	{
		Tx buf_value_x=value_x*_factor_x;
		Ty buf_value_y=value_y*_factor_y;
		if(_startIndex!=0)
			_startIndex--;
		for(int i=0;i<_size-1;i++)
		{
			_data_x[i]=_data_x[i+1];
			_data_y[i]=_data_y[i+1];
		}
		_data_x[_size-1]=buf_value_x;
		_data_y[_size-1]=buf_value_y;
	}
	virtual void MinMax(float &min_x,float &max_x,float &min_y,float &max_y)
	{
 		min_x=_data_x[_size-1];
		max_x=_data_x[_size-1];
 		min_y=_data_y[_size-1];
		max_y=_data_y[_size-1];
		for(int i=_startIndex;i<_size-1;i++)
		{
			min_x=min(min_x,_data_x[i]);
			max_x=max(max_x,_data_x[i]);
			min_y=min(min_y,_data_y[i]);
			max_y=max(max_y,_data_y[i]);
		}
		min_x/=_factor_x;
		max_x/=_factor_x;
		min_y/=_factor_y;
		max_y/=_factor_y;
	}
	float X(unsigned int index)
	{
		if(index>=Size())
		{
			Log::Number("Error: index outside of array bounds: ",index,true);
			return 0;
		}
		return _data_x[index]/_factor_x;
	}
	float Y(unsigned int index)
	{
		if(index>=Size())
		{
			Log::Number("Error: index outside of array bounds: ",index,true);
			return 0;
		}
		return _data_y[index]/_factor_y;
	}
	virtual unsigned int StartIndex()
	{
		return _startIndex;
	}
	virtual unsigned int Size()
	{
		return _size;
	}
};
