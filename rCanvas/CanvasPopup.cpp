/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           PopupWindow.cpp
// Purpose:             Handle popups and dialog windows
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
	wxBoxSizer* sizerVertical = new wxBoxSizer(wxVERTICAL);
	wxStaticText* text = new wxStaticText(this, wxID_ANY,
		" \n"
		" GLOBAL SHORTCUTS\n"
		" ----------------------------------------------------------------------------\n"
		" Help Menu \t\t\t| F1\n"
		" Toggle full screen \t\t| F11\n"
		" Pan canvas \t\t\t| Right Mouse + Drag\n"
		" Accelerate pan speed \t\t| CTRL + Right Mouse + Drag\n"
		" Center canvas \t\t\t| C\n"
		" Add a new image \t\t| A\n"
		" Open an existing canvas \t\t| O\n"
		" Save currently loaded Canvas \t| CTRL + S\n"
		" Clear Canvas \t\t\t| X\n"
		" Resize Canvas \t\t\t| R\n"
		" \n"
		" \n"
		" IMAGE-WIDGET SHORTCUTS\n"
		" (When mouse is hovering over an image)\n"
		" ----------------------------------------------------------------------------\n"
		" Scale image up / down \t\t| Mouse Scrollwheel\n"
		" Accelerate Scaling speed \t\t| CTRL + Mouse Scrollwheel\n"
		" Delete an ImageWidget \t\t| D\n"
		" Move image widget \t\t| Left Mouse + Drag\n"
		" Move without changing z-order \t| Alt + Left Mouse + Drag\n"
		" Set image to full size \t\t| F\n"
	);

	sizerVertical->Add(text);
	SetSizerAndFit(sizerVertical);
	Popup();
}

//---------------------------------------------------------------------------
//Canvas Dialog
//---------------------------------------------------------------------------

CanvasDialog::CanvasDialog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxPoint& canvasSize) 
	: wxDialog(parent, wxID_ANY, title, pos, wxDefaultSize)
{
	wxBoxSizer* sizerHorInput = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizerHorButtons = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizerVertical = new wxBoxSizer(wxVERTICAL);

	wxStaticText* text = new wxStaticText(this, wxID_ANY, 
		"Enter desired Canvas size in pixels.\n"
		"Minimum size limited to 1000.\n"
		"Maximum size limited to 50000.\n"
		"\n"
		"Anything outside of this range will be\n"
		"automatically resized.\n"
	);
	wxStaticText* textX = new wxStaticText(this, wxID_ANY, "X" );
	wxStaticText* textY = new wxStaticText(this, wxID_ANY, "Y");

	m_widthEntry = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%d"), canvasSize.x), wxDefaultPosition, wxDefaultSize);
	m_heigthEntry = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%d"), canvasSize.y), wxDefaultPosition, wxDefaultSize);
	wxButton* resizeButton = new wxButton(this, wxID_OK, "Resize");
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

	sizerHorInput->Add(textX, 0, wxALL, 5);
	sizerHorInput->Add(m_widthEntry, 0, wxALL, 5);
	sizerHorInput->Add(textY, 0, wxALL, 5);
	sizerHorInput->Add(m_heigthEntry, 0, wxALL, 5);
	sizerHorButtons->Add(resizeButton, 0, wxALL, 5);
	sizerHorButtons->Add(cancelButton, 0, wxALL, 5);

	sizerVertical->Add(text, 0, wxALL, 5);
	sizerVertical->Add(sizerHorInput, 0, wxALL, 5);
	sizerVertical->Add(sizerHorButtons, 0, wxALL, 5);

	this->SetBackgroundColour(wxColor(150, 150, 150));
	SetSizerAndFit(sizerVertical);
}

wxPoint CanvasDialog::GetCanvasSizeEntered()
{
	wxPoint canvasSize = { wxAtoi(m_widthEntry->GetLineText(0)), wxAtoi(m_heigthEntry->GetLineText(0)) };
	return canvasSize;
}
