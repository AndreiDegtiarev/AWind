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
#include "Window.h"
#include "ChartWindow.h"
#include "TextBoxNumber.h"
class Oscilloscope : public Window, IContentChangedEventReceiver
{
	VoltmeterSensor *_voltmeter;
	ChartWindow *_chartWnd;
	TextBoxNumber *_txtTimeStep;
	TextBoxNumber *_txtBufSize;
	TextBoxNumber *_txtBuf;
	TextBoxNumber *_txtMinV;
	TextBoxNumber *_txtMaxV;
public:
	Oscilloscope(VoltmeterSensor *voltmeter,int buf_size,float minV,float maxV,int wnd_width,int wnd_height):Window(F("Oscilloscope"),0,0,wnd_width,wnd_height)
	{
		_voltmeter=voltmeter;
		int x=2;
		int y=0;
		int width=70;
		int height=30;

		TextBoxFString *labelTimeSTep=new TextBoxFString(x,y+10,width,height,F("Tstep mus:"),Color::CornflowerBlue);
		initTextBox(labelTimeSTep,true);
		x+=width;
		_txtTimeStep=new TextBoxNumber(x,y,width,height,0,Color::CornflowerBlue);
		_txtTimeStep->SetNumber(voltmeter->TimeStep());
		initTextBox(_txtTimeStep,false);
		x+=width*1.2;
		TextBoxFString *labelBufsize=new TextBoxFString(x,y+10,width,height,F("Buf. size:"),Color::CornflowerBlue);
		initTextBox(labelBufsize,true);
		x+=width*1.1;
		_txtBufSize=new TextBoxNumber(x,y,width,height,0,Color::CornflowerBlue);
		initTextBox(_txtBufSize,false);
		_txtBufSize->SetNumber(buf_size);
		x=0;
		y=_txtBufSize->Top()+_txtBufSize->Height()+1;
		_chartWnd=new ChartWindow(x,y,wnd_width,wnd_height-y-height*1.2);
		_chartWnd->SetMinMaxY(minV,maxV);
		AddChild(_chartWnd);
		SetBackColor(Color::Black);
		y=_chartWnd->Top()+_chartWnd->Height()+3;

		TextBoxFString *labelMinV=new TextBoxFString(x,y+10,width,height,F("Vmin/max:"),Color::CornflowerBlue);
		x+=width;
		_txtMinV=new TextBoxNumber(x,y,width-10,height,1,Color::CornflowerBlue);
		_txtMinV->SetNumber(minV);
		x+=width;
		_txtMaxV=new TextBoxNumber(x,y,width-10,height,1,Color::CornflowerBlue);
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
			textBox->SetBorder(Color::CornflowerBlue);
			textBox->SetMargins(0,7);
			textBox->SetFont(BigFont);
			((TextBoxNumber *)textBox)->SetIsReadOnly(false);
		}
		else
			textBox->SetFont(SmallFont);
		textBox->SetColor(Color::CornflowerBlue);
		textBox->SetBackColor(Color::Black);

	}
	ChartWindow *ChartWnd()
	{
		return _chartWnd;
	}
	void NotifyContentChanged(Window *textBox)
	{
		//out<<F("Notified")<<endl;
		if(textBox == _txtMinV || textBox == _txtMaxV)
		{
			_chartWnd->SetMinMaxY(_txtMinV->GetNumber(),_txtMaxV->GetNumber());
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