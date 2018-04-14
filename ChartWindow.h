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

#include "Window.h"
#include "IDataBuffer.h"
#include "TextBoxNumber.h"
#include "ChartDC.h"
#include "DecoratorPrimitives.h"

///Chart window implement XY plots
class ChartWindow : public Window
{
	IDataBuffer *_buffer;               //!< Container for chart data
	unsigned long _last_buffer_change; //!< Contains last x value if buffer contain different last value, chart scaling will be recalculated
	ChartDC _dc;                      //!< Chart specific device context. Performs recalculation between window and chart coordinate system
	DecoratorAxis *_xAxis;           //!< Not implemented yet
	DecoratorAxis *_yAxis;          //!< Decorator for Y axis. If it is not NULL y values range is received from thid decorator otherweise values range is obtained from data buffer
public:
	///Constructor
	/**
	\param xAxis not implemented yet
	\param yAxis decorator for Y axis. If it is not NULL y values range is received from third decorator otherweise values range is obtained from data buffer
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	ChartWindow(DecoratorAxis *xAxis,DecoratorAxis *yAxis,int left,int top,int width,int hight):Window(F("chart"),left,top,width,hight)
	{
		_buffer = NULL;
		_xAxis=xAxis;
		_yAxis=yAxis;
	}
	///Sets data buffer
	void SetBuffer(IDataBuffer *buffer)
	{
		_buffer=buffer;
		_last_buffer_change=0;
	}
	///Forces to repain only chart arey (not axises) - reduces flickering 
	void InvalidateOnlyChartArea()
	{
		if(_yAxis == NULL)
		{
			Invalidate();
		}
		else
		{
			if (IsVisible())
			{
				PrepareDC(globalLcd);
				OnDraw(globalLcd);
			}
		}
	}
	///Implements drawing code. Please note axises are plotted as decorators
	void OnDraw(DC *dc)
	{ 
		if(_buffer!=NULL && _buffer->StartIndex()<_buffer->Size()-1)
		{
			unsigned int size=_buffer->Size();
			float min_x;
			float max_x;
			float min_y;
			float max_y;
			int xOffset=1;
			_buffer->MinMax(min_x,max_x,min_y,max_y);
			if(_yAxis!=NULL)
			{
				xOffset=_yAxis->EstimateRight(dc);
				_yAxis->GetMinMax(min_y,max_y);
			}
			if(_last_buffer_change!=_buffer->X(size-1))
			{
				_dc.setScalingX(xOffset,Width()-xOffset-1,min_x,max_x);
				_dc.setScalingY(Height()-1,Height()-2,min_y,max_y);
				_last_buffer_change=max_x;
			}
			dc->SetBackColor(Color::Black);
			dc->FillRect(xOffset+1,2+1,Width()-2-1,Height()-2-1);
			dc->Rectangle3D(xOffset,2,Width()-2,Height()-2,Color::DarkGray,Color::White);

			dc->SetColor(Color::Yellow);
			_dc.MoveTo(dc,min_x,max(min_y,0.0f));
			_dc.LineTo(dc,max_x,max(min_y,0.0f));
			if(_yAxis==NULL)
			{
				_dc.MoveTo(dc,max(min_x,0.0f),min_y);
				_dc.LineTo(dc,max(min_x,0.0f),max_y);
			}
			dc->SetColor(Color::LightBlue);
			dc->SetFont(F("Small"));
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