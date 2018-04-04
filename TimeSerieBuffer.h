/*
GUI library for Arduino TFT and OLED displays

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
#pragma once

#include <math.h>
#include "IDataBuffer.h"

///Buffer for sensor data. It is used by chart control.
class TimeSerieBuffer : public IDataBuffer
{
	int _reserved_size;
	int _size;
	int   *_data_y;
	float _time_step;
	float _factor_y;
	float _offset_y;
public:
	///Constructor
	/**
	\param time_step time interval between samples in buffer 
	\param factor_y scale factor for measurements. Data can be stored as volts measured directly on board pins and this factor transform them into phisical units
	\param reserved_size expected buffer size. This value allows buffer allocation only once. This is highly recomended because memory allocation is expencive operation. Mutiple allocation leads also to memory fragmentation.
	\param size actual buffer size
	*/
	TimeSerieBuffer(float time_step,float factor_y,int reserved_size,int size,float offset_y = 0)
	{
		initialize(time_step,factor_y,reserved_size,size, offset_y);
	}
	///Constructor
	/**
	\param time_step time interval between samples in buffer 
	\param factor_y scale factor for measurements. Data can be stored as volts measured directly on board pins and this factor transform them into phisical units
	\param size actual buffer size. reserved_size=size
	*/
	TimeSerieBuffer(float time_step,float factor_y,int size, float offset_y)
	{
		initialize(time_step,factor_y,size,size, offset_y);

	}
protected:
	///Initialize internal buffer
	void initialize(float time_step,float factor_y,int reserved_size,int size,float offset_y)
	{
		_factor_y=factor_y;
		_offset_y = offset_y;
		_size=size;
		_reserved_size=reserved_size;
		_time_step=time_step;
		_data_y=new int[reserved_size];
		for(int i=0;i<_size;i++)
			_data_y[i]=0;
	}
public:
	///Sets actual buffer size. It has to be less or equal to reserved_size  
	bool SetSize(int size)
	{
		if(size<=_reserved_size)
			_size=size;
		else
		{
			out<<F("Error: buffer size is too big")<<endln;
			return false;
		}
		return true;
	}
	///Sets time interval between samples in buffer 
	float SetTimeStep(float time_step)
	{
		_time_step=time_step;
	}
	///Sets scale factor for mesaurements
	float SetFactorY(float factor_y)
	{
		_factor_y=factor_y;
	}
	///Puts value into buffer
	/**
	\param index index in buffer. It has to be less than size
	\param value value to save in the buffer
	*/
	void Set(unsigned int index,float value)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endln;
			return;
		}
		_data_y[index]=(value*_factor_y) + _offset_y;
	}
	///Returns point to vaues array. It is used for direct access to internal buffer
	int *Y()
	{
		return _data_y;
	}
	///Returns min and max value for buffer data
	void MinMax(float &min_x,float &max_x,float &min_y,float &max_y)
	{
 		min_x=0;
		max_x=_time_step*_size;
 		min_y=_data_y[_size-1];
		max_y=_data_y[_size-1];
		for(int i=0;i<_size;i++)
		{
			min_y=min(min_y,(float)_data_y[i]);
			max_y=max(max_y,(float)_data_y[i]);
		}
		min_y= (min_y - _offset_y )/ _factor_y;
		max_y= (max_y - _offset_y) / _factor_y;
	}
	///Returns X that is calculated from index in buffer (see time_step parameter)
	float X(unsigned int index)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endln;
			return 0;
		}
		return _time_step*index;
	}
	///Returns  scaled value Y from buffer for specified index
	float Y(unsigned int index)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endln;
			return 0;
		}
		return (_data_y[index] - _offset_y)/_factor_y;
	}
	///Returns start index in buffer. For this class is always 0
	unsigned int StartIndex()
	{
		return 0;
	}
	///Returns buffer size
	unsigned int Size()
	{
		return _size;
	}
};