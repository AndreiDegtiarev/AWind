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

WindowsManager windowsManager(&myGLCD,loopTouch,320,240);
TouchManager touchManager(&myTouch,&windowsManager);
VoltmeterSensor *voltmeter;
ChartWindow *chartWnd;
TextBoxNumber *txtMinV;
TextBoxNumber *txtMaxV;

const float time_length=0.0001;
const int buf_size=1000;
const int sample_ratio=(int)(1.0/(time_length/buf_size));

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

	voltmeter=new VoltmeterSensor(A0,buf_size);
	Log::Number("Sample ratio:",sample_ratio,true);
	Log::Number("ms:",(int)(1e6*1.0/sample_ratio),true);

	float minV=0;
	float maxV=4;

	chartWnd=new ChartWindow(0,0,windowsManager.MainWindow()->Width(),windowsManager.MainWindow()->Height()-50);
	chartWnd->SetMinMaxY(minV,maxV);
	windowsManager.MainWindow()->AddChild(chartWnd);
	windowsManager.MainWindow()->SetBackColor(Color::Black);
	int x=0;
	int y=windowsManager.MainWindow()->Height()-40;
	int width=80;
	int height=30;
	TextBoxString<const __FlashStringHelper> *labelMinV=new TextBoxString<const __FlashStringHelper>(x,y+5,width,height,F("Vmin:"),Color::CornflowerBlue);
	x+=width;
	txtMinV=new TextBoxNumber(x,y,width-15,height,1,Color::CornflowerBlue,true);
	txtMinV->SetIsReadOnly(false);
	txtMinV->SetNumber(minV);
	x+=width;
	TextBoxString<const __FlashStringHelper> *labelMaxV=new TextBoxString<const __FlashStringHelper>(x,y+5,width,height,F("Vmax:"),Color::CornflowerBlue);
	x+=width;
	txtMaxV=new TextBoxNumber(x,y,width-15,height,1,Color::CornflowerBlue,true);
	txtMaxV->SetIsReadOnly(false);
	txtMaxV->SetNumber(maxV);

	initTextBox(labelMinV,true);
	initTextBox(txtMinV,false);
	initTextBox(labelMaxV,true);
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
	}
	textBox->SetFont(BigFont);
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
}
void loopTouch()
{
  touchManager.loop();
}
void loop()
{
	voltmeter->MeasureBuffer(sample_ratio);
	chartWnd->SetBuffer(voltmeter->Buffer());
	loopTouch();
	windowsManager.loop();
}