#pragma once

#include "TextBoxString.h"
#include "TextBoxStrTouch.h"

class wnd_info : public MainWindow
{
  TextBoxStrTouch *edtBox;
public:
  wnd_info(int width,int height):MainWindow(width,height) 
  {
    int x=0, y=10, xOff=95, yOff=20, xW=width-xOff, tHgt=13;
    AddDecorator(new DecoratorRectFill(Color::Black));
    AddDecorator(new DecoratorColor(Color::SkyBlue));
    
    TextBoxFString *label=new TextBoxFString(x,y,104,tHgt,F("Enter text:"));
    AddChild(label);
    edtBox=new TextBoxStrTouch(xOff,y,xW,tHgt,"Some text to edit");
    initEditBox(edtBox);
    
    y += yOff;
    label=new TextBoxFString(x,y,104,tHgt,F("Short Text:"));
    AddChild(label);
    edtBox=new TextBoxStrTouch(xOff,y,xW,tHgt,"Short text");
    initEditBox(edtBox);
    
    y += yOff;
    label=new TextBoxFString(x,y,104,tHgt,F("Long Text:"));
    AddChild(label);
    edtBox=new TextBoxStrTouch(xOff,y,xW,tHgt,"Some long text that is greater than the edit box size will be around");
    initEditBox(edtBox);
    
    y += yOff;
    label=new TextBoxFString(x,y,104,tHgt,F("Long Text:"));
    AddChild(label);
    edtBox=new TextBoxStrTouch(xOff,y,xW,tHgt,"This will exceed the length of the edit area");
    initEditBox(edtBox);
  }
protected:
  void initEditBox(TextBoxStrTouch *edt)
  {
    edt->SetDecorators(GetDecorators());
    edt->SetMargins(4,0);
    AddChild(edt);
  }
  
public:
	void Create()
	{
	}
	void Notify(Window * wnd)
	{
	}
};
