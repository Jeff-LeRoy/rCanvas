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
#include "PopupWindow.h"

PopupWindow::PopupWindow(wxWindow* parent)
	: wxPopupTransientWindow(parent, false)
{
	//wxPanel* panel = new wxPanel(this);
	this->SetSize(wxSize(200, 200));
	this->SetBackgroundColour(wxColor(150, 150, 150));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	//sizer->Add(text, 1, wxEXPAND);
	//sizer->Add(btn);
	this->SetSizer(sizer);

	Bind(wxEVT_LEAVE_WINDOW, &PopupWindow::ExitWindow, this);
}

void PopupWindow::ExitWindow(wxMouseEvent& event)
{
	//Dismiss();
}