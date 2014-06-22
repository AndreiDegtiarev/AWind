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

#include "Window.h"
#include "DataBufferBase.h"


class ChartDC
{
	int _last_x;
	int _last_y;
	float _scale_x;
	float _scale_y;
	float _offset_x;
	float _offset_y;
	float _min_y;
	float _max_y;
	int   _dc_start_x;
	int   _dc_start_y;
public:
	ChartDC()
	{
		_last_x=0;
		_last_y=0;
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
	void setScalingX(int length,float min_val,float max_val,int dc_start_x)
	{
		_scale_x=calcScaling(length,min_val,max_val);
		_offset_x=min_val;
		/*Log::Number("Set scale x scale: ",_scale_x);
		Log::Number(" offset: ",_offset_x);
		Log::Number(" min: ",min_val);
		Log::Number(" max: ",max_val,true);*/
		_dc_start_x=dc_start_x;
	}
	void setScalingY(int length,float min_val,float max_val,int dc_start_y)
	{
		_scale_y=calcScaling(length,min_val,max_val);
		_offset_y=min_val;
		/*Log::Number("Set scale y scale: ",_scale_y);
		Log::Number(" offset: ",_offset_y);
		Log::Number(" min: ",min_val);
		Log::Number(" max: ",max_val,true);*/
		_dc_start_y=dc_start_y;
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
	float LCtoDC_x(float x)
	{
		return _dc_start_x+(x-_offset_x)*_scale_x;
	}
	float LCtoDC_y(float y)
	{
		return _dc_start_y-(y-_offset_y)*_scale_y;
	}
	void MoveTo(DC *dc,float x,float y)
	{
		dc->MoveTo(LCtoDC_x(x),LCtoDC_y(y));
		//Log::Number("Move to x: ",x);
		//Log::Number(" y: ",y,true);
	}
	void LineTo(DC *dc,float x,float y)
	{
		dc->LineTo(LCtoDC_x(x),LCtoDC_y(y));

		/*Log::Number("Line: ",_last_x);
		Log::Number(" ",_last_y);
		Log::Number(" ",new_x);
		Log::Number(" ",new_y,"");
		Log::Number(" lc: ",x);
		Log::Number(",",y,true);*/
	}
};
extern uint8_t SmallFont[];
class ChartWindow : public Window
{
	DataBufferBase *_buffer;
	unsigned long _last_buffer_change;
	ChartDC _dc;
	TextBoxNumber *_textMinY;
	TextBoxNumber *_textMaxY;
public:
	ChartWindow(int left,int top,int width,int hight):Window(F("chart"),left,top,width,hight)//,_textMinY(TextBoxFloat(left+1,top+1,0,Color::White))
	{
		this->SetBackColor(Color::Black);
		_buffer = NULL;
		_textMaxY = new TextBoxNumber(1,1,width/2,hight/2,1,Color::White,true);
		_textMaxY->SetFont(SmallFont);
		_textMinY = new TextBoxNumber(1,hight-15,width/2,hight/2,1,Color::White,true);
		_textMinY->SetFont(SmallFont);
		this->AddChild(_textMinY);
		this->AddChild(_textMaxY);
	}
	void SetBuffer(DataBufferBase *buffer)
	{
		_buffer=buffer;
		_last_buffer_change=0;
		Invalidate();
	}
	void OnDraw(DC *dc)
	{ 
		Window::OnDraw(dc);
		if(_buffer!=NULL && _buffer->StartIndex()<_buffer->Size()-1)
		{
			unsigned int size=_buffer->Size();
			float min_x;
			float max_x;
			float min_y;
			float max_y;
			_buffer->MinMax(min_x,max_x,min_y,max_y);
			Log::Number("size: ",size);
			Log::Number(" min_x: ",min_x);
			Log::Number(" max_x: ",max_x);
			Log::Number(" min_y: ",min_y);
			Log::Number(" max_y: ",max_y,true);
			if(_last_buffer_change!=_buffer->X(size-1))
			{
				_dc.setScalingX(Width(),min_x,max_x,Left());
				_dc.setScalingY(Height(),min_y,max_y,Top()+Height());
				_last_buffer_change=max_x;
				_textMinY->SetNumber(_dc.MinY());
				_textMaxY->SetNumber(_dc.MaxY());
			}
			dc->SetColor(Color::Yellow);
			_dc.MoveTo(dc,min_x,max(min_y,0));
			_dc.LineTo(dc,max_x,max(min_y,0));
			_dc.MoveTo(dc,max(min_x,0),min_y);
			_dc.LineTo(dc,max(min_x,0),max_y);
			dc->SetColor(Color::LightBlue);
			_dc.MoveTo(dc,_buffer->X(_buffer->StartIndex()),_buffer->Y(_buffer->StartIndex()));
			for(int i=_buffer->StartIndex()+1;i<size;i++)
			{
				_dc.LineTo(dc,_buffer->X(i),_buffer->Y(i));
			}
		}
	}
};