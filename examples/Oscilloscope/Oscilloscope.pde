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


UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
extern uint8_t ArialNumFontPlus[];

const int display_width=320;
const int display_height=240;

WindowsManager windowsManager(&myGLCD,loopTouch,display_width,display_height);
TouchManager touchManager(&myTouch,&windowsManager);
VoltmeterSensor *voltmeter;
ChartWindow *chartWnd;
TextBoxNumber *txtTimeStep;
TextBoxNumber *txtBufSize;
TextBoxNumber *txtBuf;
TextBoxNumber *txtMinV;
TextBoxNumber *txtMaxV;

int time_step_mus=100;
const int reserved_buf_size=2000;
int buf_size=500;
//const int sample_ratio=(int)(1.0/(time_length/buf_size));

void setup()
{
	Serial.begin(57600);
	Serial.println(F("Setup"));

	myGLCD.InitLCD();
	myGLCD.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myGLCD.setFont(ArialNumFontPlus);
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	voltmeter=new VoltmeterSensor(A0,reserved_buf_size,buf_size);
	voltmeter->SetTimeStep(time_step_mus);
	//Log::Number("Sample ratio:",voltmeter->SampleRatio(),true);
	//Log::Number("ms:",(int)(1e6*1.0/sample_ratio),true);

	float minV=0;
	float maxV=4;

	int x=2;
	int y=0;
	int width=70;
	int height=30;

	TextBoxString<const __FlashStringHelper> *labelTimeSTep=new TextBoxString<const __FlashStringHelper>(x,y+10,width,height,F("Tstep us:"),Color::CornflowerBlue);
	initTextBox(labelTimeSTep,true);
	x+=width;
	txtTimeStep=new TextBoxNumber(x,y,width,height,0,Color::CornflowerBlue,true);
	txtTimeStep->SetNumber(time_step_mus);
	initTextBox(txtTimeStep,false);
	x+=width*1.2;
	TextBoxString<const __FlashStringHelper> *labelBufsize=new TextBoxString<const __FlashStringHelper>(x,y+10,width,height,F("Buf. size:"),Color::CornflowerBlue);
	initTextBox(labelBufsize,true);
	x+=width*1.1;
	txtBufSize=new TextBoxNumber(x,y,width,height,0,Color::CornflowerBlue,true);
	initTextBox(txtBufSize,false);
	txtBufSize->SetNumber(buf_size);
	x=0;
	y=txtBufSize->Top()+txtBufSize->Height()+1;
	chartWnd=new ChartWindow(x,y,windowsManager.MainWindow()->Width(),display_height-y-height*1.2);
	chartWnd->SetMinMaxY(minV,maxV);
	windowsManager.MainWindow()->AddChild(chartWnd);
	windowsManager.MainWindow()->SetBackColor(Color::Black);
	y=chartWnd->Top()+chartWnd->Height()+3;

	TextBoxString<const __FlashStringHelper> *labelMinV=new TextBoxString<const __FlashStringHelper>(x,y+10,width,height,F("Vmin/max:"),Color::CornflowerBlue);
	x+=width;
	txtMinV=new TextBoxNumber(x,y,width-10,height,1,Color::CornflowerBlue,true);
	txtMinV->SetNumber(minV);
	x+=width;
	txtMaxV=new TextBoxNumber(x,y,width-10,height,1,Color::CornflowerBlue,true);
	txtMaxV->SetNumber(maxV);
	x+=width;




	initTextBox(labelMinV,true);
	initTextBox(txtMinV,false);
	initTextBox(txtMaxV,false);

	delay(1000); 
	Serial.println("End setup");

}
void initTextBox(TextBox *textBox,bool isLabel)
{
	windowsManager.MainWindow()->AddChild(textBox);
	if(!isLabel)
	{
		textBox->SetOnChanged(settingsChanged);
		textBox->SetBorder(Color::CornflowerBlue);
		textBox->SetTextOffset(0,7);
		textBox->SetFont(BigFont);
		((TextBoxNumber *)textBox)->SetIsReadOnly(false);
	}
	else
		textBox->SetFont(SmallFont);
	textBox->SetColor(Color::CornflowerBlue);
	textBox->SetBackColor(Color::Black);

}
void settingsChanged(TextBox *textBox)
{
	if(textBox == txtMinV || textBox == txtMaxV)
	{
		chartWnd->SetMinMaxY(txtMinV->GetNumber(),txtMaxV->GetNumber());
		chartWnd->Invalidate();
	}
	else if(textBox == txtTimeStep)
	{
		//Log::Number("Target time step:",txtTimeStep->GetNumber(),true);
		voltmeter->SetTimeStep(txtTimeStep->GetNumber());
		//Log::Number("Sample ratio:",voltmeter->SampleRatio(),true);
	}
	else if(textBox == txtBufSize)
	{
		voltmeter->Buffer()->SetSize(txtBufSize->GetNumber());
		txtBufSize->SetNumber(voltmeter->Buffer()->Size());
	}
}
void loopTouch()
{
  touchManager.loop();
}
void loop()
{
	voltmeter->MeasureBuffer();
	chartWnd->SetBuffer(voltmeter->Buffer());
	loopTouch();
	windowsManager.loop();
}