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


extern uint8_t SmallFont[];
class ChartWindow : public Window
{
	IDataBuffer *_buffer;
	unsigned long _last_buffer_change;
	ChartDC _dc;
	//TextBoxNumber *_textMinY;
	//TextBoxNumber *_textMaxY;
	float _fix_MinY;
	float _fix_MaxY;
public:
	ChartWindow(int left,int top,int width,int hight):Window(F("chart"),left,top,width,hight)//,_textMinY(TextBoxFloat(left+1,top+1,0,Color::White))
	{
		this->SetBackColor(Color::Black);
		_buffer = NULL;
		//_textMaxY = new TextBoxNumber(1,1,width/2,hight/2,1,Color::White,true);
		//_textMaxY->SetFont(SmallFont);
		//_textMinY = new TextBoxNumber(1,hight-15,width/2,hight/2,1,Color::White,true);
		//_textMinY->SetFont(SmallFont);
		//this->AddChild(_textMinY);
		//this->AddChild(_textMaxY);
		_fix_MinY=ChartDC::AutoMin;
		_fix_MaxY=ChartDC::AutoMax;
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
			min_y=_fix_MinY==ChartDC::AutoMin?min_y:_fix_MinY;
			max_y=_fix_MaxY==ChartDC::AutoMax?max_y:_fix_MaxY;
			/*Log::Number("size: ",size);
			Log::Number(" min_x: ",min_x);
			Log::Number(" max_x: ",max_x);*/
			//Log::Number(" min_y: ",min_y);
			//Log::Number(" max_y: ",max_y,true);
			if(_last_buffer_change!=_buffer->X(size-1))
			{
				_dc.setScalingX(Width(),min_x,max_x);
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
			dc->DrawNumber(_dc.MaxY(),1,1,1);
			dc->DrawNumber(_dc.MinY(),1,1,Height()-15);
			_dc.MoveTo(dc,_buffer->X(_buffer->StartIndex()),_buffer->Y(_buffer->StartIndex()));
			for(int i=_buffer->StartIndex()+1;i<size;i++)
			{
				_dc.LineTo(dc,_buffer->X(i),_buffer->Y(i));
			}
		}
	}
};