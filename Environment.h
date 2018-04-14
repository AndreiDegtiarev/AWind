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
#include "LinkedList.h"
#include "AHelper.h"
#include "AFont.h"
#include "Decorator.h"

typedef LinkedList<Decorator> DecoratorList;
class DecoratorGroupEntry
{
public:
	const __FlashStringHelper *ID;
	DecoratorList *Decorators;
};

///This singltone class contains shared application resources like decorators.
class Environment
{
	static Environment *_singltone;
	LinkedList<DecoratorGroupEntry> _shared_decorators;
	LinkedList<AFont> _fonts;

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
	///Registers application font. For each DC type corresponding font class has to be derived
	void RegisterFont(AFont *font)
	{
		_fonts.Add(font);
	}
	///Finds registered font by the name
	AFont * FindFont(const __FlashStringHelper *name)
	{
		for (int i = 0; i<_fonts.Count(); i++)
		{
			if (AHelper::compare_F(name, _fonts[i]->Name))
				return _fonts[i];

		}
		return NULL;
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