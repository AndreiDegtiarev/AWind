#pragma once
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
#include "LinkedList.h"
#include "AHelper.h"
#include "Decorator.h"

typedef LinkedList<Decorator> DecoratorList;
class DecoratorGroupEntry
{
public:
	const __FlashStringHelper *ID;
	DecoratorList *Decorators;
};
///This singltone class contains shared appcication resources like decorators dialogs and etc.
class Environment
{
	static Environment *_singltone;
	LinkedList<DecoratorGroupEntry> _shared_decorators;

	///Constructor initializes one static instance of environment
	Environment()
	{
		_singltone=this;
	}
public:
	///Returns singltone instance of environment
	static Environment *Get()
	{
		if(_singltone == NULL)
			_singltone = new Environment();
		return _singltone;
	}
	///Registers decorators group. It is like application resources that can be shred between windows of the same type (e.g. buttons, text box and etc.)
	void RegisterDecoratorsGroup(const __FlashStringHelper *id,DecoratorList * decorators)
	{
		DecoratorGroupEntry *entry=new DecoratorGroupEntry();
		entry->ID=id;
		entry->Decorators=decorators;
		_shared_decorators.Add(entry);
	}
	///Finds registered decorators by the name
	DecoratorList *FindDecorators(const __FlashStringHelper *id)
	{
		for(int i=0;i<_shared_decorators.Count();i++)
		{
			if(AHelper::compare_F(id, _shared_decorators[i]->ID))
				return _shared_decorators[i]->Decorators;

		}
		return NULL;
	}

};