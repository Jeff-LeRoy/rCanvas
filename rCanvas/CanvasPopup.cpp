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

//---------------------------------------------------------------------------
//Canvas Popup
//---------------------------------------------------------------------------

CanvasPopup::CanvasPopup(wxWindow* parent)
	: wxPopupTransientWindow(parent, false)
{
	this->SetBackgroundColour(wxColor(150, 150, 150));

	//SetFocus();

	//Binding events
	//Bind(wxEVT_LEAVE_WINDOW, &CanvasPopup::ExitWindow, this);
	//Bind(wxEVT_KILL_FOCUS, &CanvasPopup::OnKillFocus, this);
	//Bind(wxEVT_SET_FOCUS, &CanvasPopup::OnSetFocus, this);
	//Bind(wxEVT_KEY_DOWN, &CanvasPopup::OnKeyDown, this);
}

//void CanvasPopup::OnKillFocus(wxFocusEvent& event)
//{
//	wxLogStatus("OnKillFocus");
//	//Dismiss();
//	event.Skip();
//}
//
//void CanvasPopup::OnSetFocus(wxFocusEvent& event)
//{
//	wxLogStatus("something has focus");
//	event.Skip();
//}
//
//void CanvasPopup::OnKeyDown(wxKeyEvent& event)
//{
//	wxLogStatus("OnKillFocus");
//	event.Skip();
//}

//---------------------------------------------------------------------------
//Canvas Dialog
//---------------------------------------------------------------------------

CanvasDialog::CanvasDialog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxPoint& canvasSize) 
	: wxDialog(parent, wxID_ANY, title, pos, wxDefaultSize)
{
	wxBoxSizer* sizerHorInput = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizerHorButtons = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizerVertical = new wxBoxSizer(wxVERTICAL);

	wxStaticText* text = new wxStaticText(this, wxID_ANY, "Enter desired Canvas size in pixels.");
	widthEntry = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%d"), canvasSize.x), wxDefaultPosition, wxDefaultSize);
	heigthEntry = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%d"), canvasSize.y), wxDefaultPosition, wxDefaultSize);
	wxButton* resizeButton = new wxButton(this, wxID_OK, "Resize");
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

	sizerHorInput->Add(widthEntry, 0, wxALL, 5);
	sizerHorInput->Add(heigthEntry, 0, wxALL, 5);
	sizerHorButtons->Add(resizeButton, 0, wxALL, 5);
	sizerHorButtons->Add(cancelButton, 0, wxALL, 5);

	sizerVertical->Add(text, 0, wxALL, 5);
	sizerVertical->Add(sizerHorInput, 0, wxALL, 5);
	sizerVertical->Add(sizerHorButtons, 0, wxALL, 5);

	this->SetBackgroundColour(wxColor(150, 150, 150));
	SetSizerAndFit(sizerVertical);
}

wxPoint CanvasDialog::GetCanvasSize()
{
	wxPoint canvasSize = { wxAtoi(widthEntry->GetLineText(0)), wxAtoi(heigthEntry->GetLineText(0)) };
	return canvasSize;
}
