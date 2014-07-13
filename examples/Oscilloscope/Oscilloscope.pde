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

#include <UTFT.h>
#include <UTouch.h>

#include "LinkedList.h"
#include "WindowsManager.h"
#include "TouchManager.h"
#include "KeyboardWindow.h"
#include "ChartWindow.h"
#include "VoltmeterSensor.h"
#include "IEvent.h"


UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
extern uint8_t ArialNumFontPlus[];

const int display_width=320;
const int display_height=240;

WindowsManager windowsManager(&myGLCD,display_width,display_height);
TouchManager touchManager(&myTouch,&windowsManager);
VoltmeterSensor *voltmeter;
ChartWindow *chartWnd;
TextBoxNumber *txtTimeStep;
TextBoxNumber *txtBufSize;
TextBoxNumber *txtBuf;
TextBoxNumber *txtMinV;
TextBoxNumber *txtMaxV;

class ProcessTextBoxEvent : public IEvent<Window>
{
	void Notify(Window *textBox)
	{
		out<<F("Notified")<<endl;
		if(textBox == txtMinV || textBox == txtMaxV)
		{
			chartWnd->SetMinMaxY(txtMinV->GetNumber(),txtMaxV->GetNumber());
			chartWnd->Invalidate();
		}
		else if(textBox == txtTimeStep)
		{
			voltmeter->SetTimeStep(txtTimeStep->GetNumber());
		}
		else if(textBox == txtBufSize)
		{
			voltmeter->Buffer()->SetSize(txtBufSize->GetNumber());
			txtBufSize->SetNumber(voltmeter->Buffer()->Size());
		}
	}

};
ProcessTextBoxEvent textBoxEvent;


int time_step_mus=100;
const int reserved_buf_size=2000;
int buf_size=500;

void setup()
{
	out.begin(57600);
	out<<F("Setup");

	myGLCD.InitLCD();
	myGLCD.clrScr();

	windowsManager.Initialize();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myGLCD.setFont(ArialNumFontPlus);
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	voltmeter=new VoltmeterSensor(A0,reserved_buf_size,buf_size);
	voltmeter->SetTimeStep(time_step_mus);

	windowsManager.SetCriticalProcess(&touchManager);

	float minV=0;
	float maxV=4;

	int x=2;
	int y=0;
	int width=70;
	int height=30;

	TextBoxFString *labelTimeSTep=new TextBoxFString(x,y+10,width,height,F("Tstep us:"),Color::CornflowerBlue);
	initTextBox(labelTimeSTep,true);
	x+=width;
	txtTimeStep=new TextBoxNumber(x,y,width,height,0,Color::CornflowerBlue);
	txtTimeStep->SetNumber(time_step_mus);
	initTextBox(txtTimeStep,false);
	x+=width*1.2;
	TextBoxFString *labelBufsize=new TextBoxFString(x,y+10,width,height,F("Buf. size:"),Color::CornflowerBlue);
	initTextBox(labelBufsize,true);
	x+=width*1.1;
	txtBufSize=new TextBoxNumber(x,y,width,height,0,Color::CornflowerBlue);
	initTextBox(txtBufSize,false);
	txtBufSize->SetNumber(buf_size);
	x=0;
	y=txtBufSize->Top()+txtBufSize->Height()+1;
	chartWnd=new ChartWindow(x,y,windowsManager.MainWnd()->Width(),display_height-y-height*1.2);
	chartWnd->SetMinMaxY(minV,maxV);
	windowsManager.MainWnd()->AddChild(chartWnd);
	windowsManager.MainWnd()->SetBackColor(Color::Black);
	y=chartWnd->Top()+chartWnd->Height()+3;

	TextBoxFString *labelMinV=new TextBoxFString(x,y+10,width,height,F("Vmin/max:"),Color::CornflowerBlue);
	x+=width;
	txtMinV=new TextBoxNumber(x,y,width-10,height,1,Color::CornflowerBlue);
	txtMinV->SetNumber(minV);
	x+=width;
	txtMaxV=new TextBoxNumber(x,y,width-10,height,1,Color::CornflowerBlue);
	txtMaxV->SetNumber(maxV);
	x+=width;

	initTextBox(labelMinV,true);
	initTextBox(txtMinV,false);
	initTextBox(txtMaxV,false);

	delay(1000); 
	out<<F("End setup");

}

void initTextBox(TextBox *textBox,bool isLabel)
{
	windowsManager.MainWnd()->AddChild(textBox);
	if(!isLabel)
	{
		textBox->SetOnChanged(&textBoxEvent);
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
void loop()
{
	voltmeter->MeasureBuffer();
	chartWnd->SetBuffer(voltmeter->Buffer());
	windowsManager.loop();
}