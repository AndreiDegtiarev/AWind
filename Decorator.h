#pragma once
/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  

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