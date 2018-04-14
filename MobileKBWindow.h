#pragma once
#include "TextBoxString.h"
#include "TextBoxEditString.h"
#include "TextBoxChar.h"
#include "IDialogClosedEventReceiver.h"
#include "DecoratorPrimitives.h"
#include "Dialog.h"

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

static const char mkbCaps[31] PROGMEM = "QWERTYUIOPASDFGHJKL\"ZXCVBNM;,.";
static const char mkbSm[31] PROGMEM = "qwertyuiopasdfghjkl\"zxcvbnm;,.";
static const char mkbNumSym[31] PROGMEM = "1234567890@#$%&-+_=/[]{}()<>,.";

class MobileKBWindow :  public Dialog
{
	TextBoxEditString * _editField;
	TextBoxChar * _btnWins[30];
	//TextBoxFString * _btnOK;
	//TextBoxFString * _btnCancel;
	TextBoxFString * _btnBckSpc;
	TextBoxFString * _btnCaps;
	TextBoxFString * _btnNumSym;
	TextBoxFString * _btnSpaceBar;
	TextBoxFString * _btnClear;

	char let[31];
	int curLetters;
	const static int _butSize=22;
	const static int _butSpace=4;
	const static int _butSizeSpace=_butSize+_butSpace;
	const static int _textOffset=3;
	const static int _btn2W = _butSize*2 + _butSpace;
	const static int _btn10W = 10*_butSizeSpace+_butSpace;
public:
	MobileKBWindow(int left,int top):Dialog(left,top,12*_butSizeSpace+_butSpace,5*_butSizeSpace+_butSpace)
	{
		int  i,k, len=30, p=0, x=_butSpace, y=_butSpace, tSmOff=5;
		char c;
		
		GetLetters(mkbCaps, 30);
		curLetters = 0;

		_editField=new TextBoxEditString(x,y,Width()-2*_butSpace,_butSize,"");
		initTextBox(_editField);
		_editField->SetDecorators(Environment::Get()->FindDecorators(F("EditTextBoxReadOnly")));
		_editField->SetFont(Environment::Get()->FindFont(F("Big")));
		
		DecoratorList * btnDecorators=Environment::Get()->FindDecorators(F("Button"));
		y+=_butSize+_butSpace;
		for(k=0;k<3;k++)
		{
			for(i=0;i<10;i++)
			{
				x=i*(_butSize+_butSpace);
				c = let[p];
				_btnWins[p]=new TextBoxChar(x+_butSpace,y,_butSize,_butSize,c);
				_btnWins[p]->SetDecorators(btnDecorators);
				initTextBox(_btnWins[p]);
				p++;
			}
			y+=_butSize+_butSpace;
		}
		
		y=_butSizeSpace+_butSpace;
		_btnBckSpc=new Button(_btn10W,y,_btn2W,_butSize,F("BckSpc"));
		initTextBox(_btnBckSpc);
		_btnBckSpc->SetFont(Environment::Get()->FindFont(F("Small")));
		_btnBckSpc->SetMargins(1,tSmOff);
		
		y+=_butSize+_butSpace;
		_btnOK=new Button(_btn10W,y,_btn2W,_butSize,F("Enter"));
		initTextBox(_btnOK);
		_btnOK->SetFont(Environment::Get()->FindFont(F("Small")));
		_btnOK->SetMargins(4,tSmOff);

		y+=2*(_butSize+_butSpace);
		_btnCaps=new Button(_butSpace,y,_btn2W,_butSize,F("aA"));
		initTextBox(_btnCaps);
		_btnCaps->SetMargins(7,2);

		_btnNumSym=new Button(2*_butSizeSpace+_butSpace,y,_btn2W,_butSize,F("1&"));
		initTextBox(_btnNumSym);
		_btnNumSym->SetMargins(7,2);

		_btnSpaceBar=new Button(4*_butSizeSpace+_butSpace,y,_btn2W*2+_butSpace,_butSize,F("Space"));
		initTextBox(_btnSpaceBar);
		_btnSpaceBar->SetFont(Environment::Get()->FindFont(F("Small")));
		_btnSpaceBar->SetMargins(32,tSmOff);

		_btnClear=new Button(8*_butSizeSpace+_butSpace,y,_btn2W,_butSize,F("Clear"));
		initTextBox(_btnClear);
		_btnClear->SetFont(Environment::Get()->FindFont(F("Small")));
		_btnClear->SetMargins(4,tSmOff);

		_btnCancel=new Button(_btn10W,y,_btn2W,_butSize,F("Cancel"));
		initTextBox(_btnCancel);
		_btnCancel->SetFont(Environment::Get()->FindFont(F("Small")));
		_btnCancel->SetMargins(1,tSmOff);

	}
protected:
	void initTextBox(TextBox *text)
	{
		text->SetFont(Environment::Get()->FindFont(F("Big")));
		text->SetMargins(_textOffset,_textOffset);
		text->RegisterTouchEventReceiver(this);
		AddChild(text);
	}
private:
	void SetBtns()
	{
		char c;
		int i;
		for(i=0;i<28;i++) {
			c = let[i];
			_btnWins[i]->SetChar(c);
		}
	}
	void GetLetters(const char* buf, int len=28)
	{
		int i;
		for(i=0;i<len;i++) {
			let[i] = pgm_read_byte_near(buf + i);
		}
	}

public:
	void Initialize(char *txt)
	{
		_editField->SetText(txt);
		_editField->Invalidate();
	}
	void Finalization()
	{
		_editField->Closed();
	}
	char * GetText()
	{
		return _editField->GetText();
	}
	void DoControlMessage(Window *window)
	{
		/*if((window == _btnOK) || (window == _btnCancel))
		{
			if(_dialogClosedEventReceiver!=NULL) {
				_dialogClosedEventReceiver->NotifyDialogClosed(this,window == _btnOK?IDialogClosedEventReceiver::OK : IDialogClosedEventReceiver::Cancel);
				_editField->Closed();
			}
		}
		else*/
			if ((window == _btnCaps) || (window == _btnNumSym))
			{
				if (window == _btnCaps)
				{
					if (*let == *mkbNumSym)
						(curLetters == 1)? GetLetters(mkbSm) : GetLetters(mkbCaps);
					else
					{
						(*let == *mkbCaps)? GetLetters(mkbSm) : GetLetters(mkbCaps);
						curLetters = !curLetters;
					}
				}
				else
				{
					if (*let == *mkbNumSym)
						return;
					GetLetters(mkbNumSym);
				}
				SetBtns();
			}
			else
			{
				byte b;
				if (window == _btnBckSpc) b = 8; else
				if (window == _btnClear) b = 13; else
				if (window == _btnSpaceBar) b = (byte)' '; else
				{
					for(int i=0;i<30;i++)
						if (window == _btnWins[i])
						{
							b = (byte)_btnWins[i]->GetChar();
							break;
						}
					}
			_editField->UpdateEdit(b);
		}
	}
};