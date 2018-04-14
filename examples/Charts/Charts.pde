/*
This file is part of AWind library

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

#include "DC_UTFT.h"

#include "Log.h"
#include "WindowsManager.h"
#include "ChartWindow.h"
#include "TimeSerieBuffer.h"
#include "TextBoxString.h"
#include "TextBoxNumber.h"
#include "DecoratorPrimitives.h"

// Setup TFT display (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32,25,26,27,28);
#else
UTFT    myGLCD(ITDB32S,39,41,43,45);
#endif

DC_UTFT dc(&myGLCD);

//manager which is responsible for window updating process 
WindowsManager<> windowsManager(&dc,NULL);

//Container that keeps data for further visualization 
TimeSerieBuffer	*dataBuffer;
ChartWindow *chartWnd;
TextBoxNumber *textNumber;

//Number of chart points
int buf_size=1000;
//Time step in buffer. If this parameter is defined it is no more need to store x-date separatly
float time_step=1.0/buf_size;

void setup()
{
	out.begin(9600);
	out<<(F("Setup"))<<endln;
	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	//pinMode(47,OUTPUT);
	//digitalWrite(47,HIGH);
	
	DC_UTFT::RegisterDefaultFonts();
	//initialize window manager
	windowsManager.Initialize();

	dataBuffer=new TimeSerieBuffer(time_step,100,1000,1000);
	//Setup main window background
	windowsManager.MainWnd()->AddDecorator(new DecoratorRectFill(Color::LightGray));

	int x=0;
	int y=2;
	int cy=25;
	TextBoxFString *textBox=new TextBoxFString(x,y,windowsManager.GetDC()->DeviceWidth()/2,cy,F("Scaling factor: "));
	//Setup transparent background and green text color
	textBox->AddDecorator(new DecoratorColor(Color::Green));
	textBox->SetFont(F("Big"));
	x=windowsManager.GetDC()->DeviceWidth()*3.0/4;
	textNumber=new TextBoxNumber(x,y,windowsManager.GetDC()->DeviceWidth()-x-2,cy,0);
	//Setup background + 3D-look of text box
	textNumber->AddDecorator(new DecoratorRectFill(Color::Red));
	textNumber->AddDecorator(new Decorator3DRect(Color::DarkGray,Color::White));
	textNumber->AddDecorator(new DecoratorColor(Color::SkyBlue));
	textNumber->SetFont(F("Big"));
	textNumber->SetMargins(20,2);
	x=1;
	y+=cy+5;
	cy=windowsManager.GetDC()->DeviceHeight()-y-2;
	//We create axis decorator separetly because chart need this decorator as well
	int axis_y_margins=2;
	DecoratorAxis *chartYAxis=new DecoratorAxis(DecoratorAxis::VerticalLeft, Environment::Get()->FindFont(F("Small")),cy-axis_y_margins*2,-1,1,5);
	chartYAxis->SetOffset(4,axis_y_margins);
	chartWnd=new ChartWindow(NULL,chartYAxis,x,y,windowsManager.GetDC()->DeviceWidth()-2,cy);
	//Chart decorators
	chartWnd->AddDecorator(new Decorator3DRect(Color::White,Color::Gray));
	chartWnd->AddDecorator(new DecoratorColor(Color::Black));
	chartWnd->AddDecorator(chartYAxis);
	chartWnd->SetBuffer(dataBuffer);
	///Attach crated window to the main window
	windowsManager.MainWnd()->AddChild(textBox);
	windowsManager.MainWnd()->AddChild(textNumber);
	windowsManager.MainWnd()->AddChild(chartWnd);

	out<<F("End setup")<<endln;

}
int num_index=100;
void loop()
{
	if(num_index<1)
		num_index=100;
	dataBuffer->SetFactorY(num_index);
	for(unsigned int i=0;i<1000;i++)
	{
		dataBuffer->Set(i,sin(2*3.14*(time_step*i)));
	}
	textNumber->SetNumber(num_index);
	//In order to reduce flickering only chart area is updated
	chartWnd->InvalidateOnlyChartArea();
	num_index/=2;
	//give window manager an opportunity to update display
	windowsManager.loop();
	delay(1000);
}

