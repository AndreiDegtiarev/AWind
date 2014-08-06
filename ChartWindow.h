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
#include "IDataBuffer.h"
#include "TextBoxNumber.h"
#include "ChartDC.h"
#include "DecoratorPrimitives.h"


extern uint8_t SmallFont[];
class ChartWindow : public Window
{
	IDataBuffer *_buffer;
	unsigned long _last_buffer_change;
	ChartDC _dc;
	float _fix_MinY;
	float _fix_MaxY;
	DecoratorAxis *_xAxis;
	DecoratorAxis *_yAxis;
public:
	ChartWindow(DecoratorAxis *xAxis,DecoratorAxis *yAxis,int left,int top,int width,int hight):Window(F("chart"),left,top,width,hight)//,_textMinY(TextBoxFloat(left+1,top+1,0,Color::White))
	{
		_buffer = NULL;
		_fix_MinY=ChartDC::AutoMin;
		_fix_MaxY=ChartDC::AutoMax;
		_xAxis=xAxis;
		_yAxis=yAxis;
		if(_yAxis!=NULL)
		{
			_yAxis->GetMinMax(_fix_MinY,_fix_MaxY);
		}
	}
	void SetMinMaxY(float min,float max)
	{
		_fix_MinY=min;
		_fix_MaxY=max;
	}
	void SetBuffer(IDataBuffer *buffer)
	{
		_buffer=buffer;
		_last_buffer_change=0;
		//Invalidate();
	}
	void InvalidateOnlyChartArea()
	{
		if(_yAxis == NULL)
		{
			Invalidate();
		}
		else
		{
			DC dc;
			PrepareDC(&dc);
			dc.SetBackColor(Color::Black);
			dc.FillRect(_yAxis->EstimateWidth(&dc),2,Width()-2,Height()-2);
			OnDraw(&dc);
		}
	}
	void OnDraw(DC *dc)
	{ 
		if(_buffer!=NULL && _buffer->StartIndex()<_buffer->Size()-1)
		{
			unsigned int size=_buffer->Size();
			float min_x;
			float max_x;
			float min_y;
			float max_y;
			_buffer->MinMax(min_x,max_x,min_y,max_y);
			min_y=_fix_MinY==ChartDC::AutoMin?min_y:_fix_MinY;
			max_y=_fix_MaxY==ChartDC::AutoMax?max_y:_fix_MaxY;
			/*Log::Number(F("size: "),size);
			Log::Number(F(" min_x: "),min_x);
			Log::Number(F(" max_x: "),max_x);
			Log::Number(" min_y: ",min_y);
			Log::Number(" max_y: ",max_y,true);*/
			if(_last_buffer_change!=_buffer->X(size-1))
			{
				int xOffset=0;
				if(_yAxis!=NULL)
					xOffset=_yAxis->EstimateWidth(dc);
				_dc.setScalingX(xOffset,Width()-xOffset,min_x,max_x);
				_dc.setScalingY(Height(),min_y,max_y);
				_last_buffer_change=max_x;
				//_textMinY->SetNumber(_dc.MinY());
				//_textMaxY->SetNumber(_dc.MaxY());
			}
			dc->SetColor(Color::Yellow);
			_dc.MoveTo(dc,min_x,max(min_y,0));
			_dc.LineTo(dc,max_x,max(min_y,0));
			_dc.MoveTo(dc,max(min_x,0),min_y);
			_dc.LineTo(dc,max(min_x,0),max_y);
			dc->SetColor(Color::LightBlue);
			dc->SetFont(SmallFont);
			if(_yAxis == NULL)
			{
				dc->DrawNumber(_dc.MaxY(),1,1,1);
				dc->DrawNumber(_dc.MinY(),1,1,Height()-15);
			}
			_dc.MoveTo(dc,_buffer->X(_buffer->StartIndex()),_buffer->Y(_buffer->StartIndex()));
			for(int i=_buffer->StartIndex()+1;i<size;i++)
			{
				_dc.LineTo(dc,_buffer->X(i),_buffer->Y(i));
			}
		}
	}
};