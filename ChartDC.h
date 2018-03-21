#pragma once
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
///Device context for chart window. Implement transformation of logic coordinates into window coordinates 
class ChartDC
{
	float _last_y;
	float _scale_x;
	float _scale_y;
	float _offset_x;
	float _offset_y;
	float _min_y;
	float _max_y;
	int   _dc_start_x;
	int   _dc_start_y;
public:
	static const float AutoMin;
	static const float AutoMax;
	///Constructor
	ChartDC()
	{
		_scale_x=1;
		_scale_y=1;
		_offset_x=0;
		_offset_y=0;
		_dc_start_x=0;
		_dc_start_y=0;
		_min_y=0;
		_max_y=0;
	}
	float calcScaling(int length,float &min_val,float &max_val)
	{
		if(min_val == max_val)
		{
			if(min_val == 0)
			{
				min_val=-1;
				max_val=1;
			}
			else if(min_val<0)
				max_val = 0;
			else if(min_val>0)
				min_val = 0;
		}
		return length/(max_val-min_val);
	}
	void setScalingX(int dc_offset,int length,float min_val,float max_val)
	{
		_scale_x=calcScaling(length,min_val,max_val);
		_offset_x=min_val;
		_dc_start_x=dc_offset;
		/*Log::Number("Set scale x scale: ",_scale_x);
		Log::Number(" offset: ",_offset_x);
		Log::Number(" min: ",min_val);
		Log::Number(" max: ",max_val,true);*/
	}
	void setScalingY(int dc_offset,int length,float min_val,float max_val)
	{
		_scale_y=calcScaling(length,min_val,max_val);
		_offset_y=min_val;
		/*Log::Number("Set scale y scale: ",_scale_y);
		Log::Number(" offset: ",_offset_y);
		Log::Number(" min: ",min_val);
		Log::Number(" max: ",max_val,true);*/
		_dc_start_y=dc_offset;
		_min_y=min_val;
		_max_y=max_val;
	}
	float MinY()
	{
		return _min_y;
	}
	float MaxY()
	{
		return _max_y;
	}
	///Converts logic x into window x
	float LCtoDC_x(float x)
	{
		return _dc_start_x+(x-_offset_x)*_scale_x;
	}
	///Converts logic y into window y
	float LCtoDC_y(float y)
	{
		return _dc_start_y-(y-_offset_y)*_scale_y;
	}
	///Moves actual position in logic coordinates
	void MoveTo(DC *dc,float x,float y)
	{
		_last_y=y;
		dc->MoveTo(LCtoDC_x(x),LCtoDC_y(y));
		//Log::Number("Move to x: ",x);
		//Log::Number(" y: ",y,true);
	}
	///Draws line from actual position to the new position
	void LineTo(DC *dc,float x,float y)
	{
		if(y<_min_y || y> _max_y || _last_y<_min_y || _last_y> _max_y)
			dc->MoveTo(LCtoDC_x(x),LCtoDC_y(y));
		else
			dc->LineTo(LCtoDC_x(x),LCtoDC_y(y));
		_last_y=y;
		//Log::Number(" min_y: ",_min_y);
		//Log::Number(" max_y: ",_max_y,true);

		/*Log::Number("Line: ",_last_x);
		Log::Number(" ",_last_y);
		Log::Number(" ",new_x);
		Log::Number(" ",new_y,"");
		Log::Number(" lc: ",x);
		Log::Number(",",y,true);*/
	}
};
const float ChartDC::AutoMin = 1e-8;
const float ChartDC::AutoMax = 1e8;
