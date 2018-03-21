#pragma once
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
class DC;
///Base class for window decorators. This concept allows sharing of drawing setting among more than one window -> SRAM memory usage optimization 
class Decorator
{
public:
	///Estimates decorator left coordinate
	virtual int EstimateLeft(DC *dc)
	{
		return 0;
	}
	///Estimates decorator right coordinate
	virtual int EstimateRight(DC *dc)
	{
		return 0;
	}
	///Estimates decorator top coordinate
	virtual int EstimateTop(DC *dc)
	{
		return 0;
	}
	///Estimates decorator bottom coordinate
	virtual int EstimateBottom(DC *dc)
	{
		return 0;
	}
	///Drawing code implementation
	/**
	\param dc device context
	*/
	virtual void Draw(DC *dc,int left,int top,int width,int height)=0;
};