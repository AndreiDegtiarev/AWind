/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  

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
#pragma once

template <class T> class LinkedEntry
{
public:
	LinkedEntry* Next;
	T* Item;
	LinkedEntry(T* item)
	{
		Item = item;
		Next=NULL;
	}
};
template <class T> class LinkedList
{
	LinkedEntry<T> * Head;
	int _count;
public:
	LinkedList()
	{
		Head=NULL;
		_count=0;
	};
	void Add(T *item)
	{
		LinkedEntry<T> *new_entry=new LinkedEntry<T>(item);
		if(Head!=NULL)
		{
			LinkedEntry<T> *cur= Head;
			while(cur->Next!=NULL)
			{
				cur=cur->Next;
			}
			cur->Next = new_entry;
		}
		else
		{
			Head = new_entry;
		}
		_count++;
	}
	int Count()
	{
		return _count;
	}
	T* operator[](int pos)
	{
		if(pos>=_count)
		{
			Serial.print("Error: index is too big: ");
			Serial.println(pos);
			return NULL;
		}
		else
		{
			int index=0;
			LinkedEntry<T> *cur= Head;
			while(cur!=NULL)
			{
				if(index == pos)
					return cur->Item;
				cur=cur->Next;
				index++;
			}
		}
		return NULL;
	}
};