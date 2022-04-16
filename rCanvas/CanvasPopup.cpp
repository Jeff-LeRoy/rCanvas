/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           PopupWindow.cpp
// Purpose:             Handle popup windows
// Author:              Jeffrey C. LeRoy
// Created:             02/27/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/popupwin.h>
#include "CanvasPopup.h"

PopupWindow::PopupWindow(wxWindow* parent)
	: wxPopupTransientWindow(parent, false)
{
	this->SetBackgroundColour(wxColor(150, 150, 150));

	Bind(wxEVT_LEAVE_WINDOW, &PopupWindow::ExitWindow, this);
}

void PopupWindow::ExitWindow(wxMouseEvent& event)
{
	//Dismiss();
}