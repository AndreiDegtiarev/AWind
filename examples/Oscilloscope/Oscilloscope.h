/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
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
#include "MainWindow.h"
#include "ChartWindow.h"
#include "TextBoxNumber.h"
#include "DefaultDecorators.h"
#include "Label.h"
///Osciloscope main window
class Oscilloscope : public MainWindow, IContentChangedEventReceiver
{
	VoltmeterSensor *_voltmeter;
	ChartWindow *_chartWnd;
	TextBoxNumber *_txtTimeStep;
	TextBoxNumber *_txtBufSize;
	TextBoxNumber *_txtBuf;
	TextBoxNumber *_txtMinV;
	TextBoxNumber *_txtMaxV;
	DecoratorAxis *_chartYAxis;
public:
	Oscilloscope(int wnd_width,int wnd_height):MainWindow(wnd_width,wnd_height)
	{
	}
	void Initialize(VoltmeterSensor *voltmeter,int buf_size,float minV,float maxV)
	{
		SetDecorators(*Environment::Get()->FindDecorators(F("Window")));
		int wnd_width=Width();
		int wnd_height=Height();
		_voltmeter=voltmeter;
		int x=2;
		int y=2;
		int width=70;
		int height=30;

		LinkedList<Decorator> *txtDecorators= Environment::Get()->FindDecorators(F("EditTextBox"));

		Label *labelTimeSTep=new Label(x,y+10,width,height,F("Tstep mus:"));
		initTextBox(labelTimeSTep,true);
		x+=width;
		_txtTimeStep=new TextBoxNumber(x,y,width,height,0);
		_txtTimeStep->SetNumber(voltmeter->TimeStep());
		initTextBox(_txtTimeStep,false);
		x+=width*1.2;
		Label *labelBufsize=new Label(x,y+10,width,height,F("Buf. size:"));
		initTextBox(labelBufsize,true);
		x+=width*1.1;
		_txtBufSize=new TextBoxNumber(x,y,width,height,0);
		initTextBox(_txtBufSize,false);
		_txtBufSize->SetNumber(buf_size);
		x=1;
		y=_txtBufSize->Top()+_txtBufSize->Height()+1;
		int cy=wnd_height-y-height*1.2;
		int axis_y_margins=2;
		_chartYAxis=new DecoratorAxis(DecoratorAxis::VerticalLeft,SmallFont,cy-axis_y_margins*2,minV,maxV,5);
		_chartYAxis->SetOffset(4,axis_y_margins);
		_chartWnd=new ChartWindow(NULL,_chartYAxis,x,y,wnd_width-2,cy);
		//_chartWnd->SetMinMaxY(minV,maxV);
		_chartWnd->AddDecorator(new Decorator3DRect(Color::White,Color::Gray));
		_chartWnd->AddDecorator(new DecoratorColor(Color::Black));
		_chartWnd->AddDecorator(_chartYAxis);
		AddChild(_chartWnd);
		y=_chartWnd->Top()+_chartWnd->Height()+3;

		Label *labelMinV=new Label(x,y+10,width,height,F("Vmin/max:"));
		x+=width;
		_txtMinV=new TextBoxNumber(x,y,width-10,height,1);
		_txtMinV->SetNumber(minV);
		x+=width;
		_txtMaxV=new TextBoxNumber(x,y,width-10,height,1);
		_txtMaxV->SetNumber(maxV);
		x+=width;

		initTextBox(labelMinV,true);
		initTextBox(_txtMinV,false);
		initTextBox(_txtMaxV,false);
	}
	void initTextBox(TextBox *textBox,bool isLabel)
	{
		AddChild(textBox);
		if(!isLabel)
		{
			textBox->RegisterContentChangedReceiver(this);
			textBox->SetMargins(0,7);
			textBox->SetFont(BigFont);
			((TextBoxNumber *)textBox)->SetIsReadOnly(false);
		}

	}
	ChartWindow *ChartWnd()
	{
		return _chartWnd;
	}
	void NotifyContentChanged(Window *textBox)
	{
		if(textBox == _txtMinV || textBox == _txtMaxV)
		{
			_chartYAxis->SetMinMax(_txtMinV->GetNumber(),_txtMaxV->GetNumber());
			_chartWnd->Invalidate();
		}
		else if(textBox == _txtTimeStep)
		{
			_voltmeter->SetTimeStep(_txtTimeStep->GetNumber());
		}
		else if(textBox == _txtBufSize)
		{
			_voltmeter->Buffer()->SetSize(_txtBufSize->GetNumber());
			_txtBufSize->SetNumber(_voltmeter->Buffer()->Size());
		}
	}
};