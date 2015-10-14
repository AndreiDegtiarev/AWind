#include "UTFT.h"
#include "TextBoxStrTouch.h"
#include "MobileKBWindow.h"

bool TextBoxStrTouch::OnTouch(int x,int y)
{
	bool retCode=TextBox::OnTouch(x,y);
	MobileKBWindow * mobKBWnd=(MobileKBWindow *)FindDialog(F("MobileKeyboard"));
	if(mobKBWnd == NULL)
	{
		mobKBWnd=new MobileKBWindow(2,100);
		RegisterDialog(F("MobileKeyboard"),mobKBWnd); 
	}
	if(mobKBWnd!=NULL)
	{
		mobKBWnd->Initialize(GetText());
		if(DoDialog(mobKBWnd) == IDialogClosedEventReceiver::OK)
			SetText(mobKBWnd->GetText());
		mobKBWnd->Finalization();
	}
	return true;
}
