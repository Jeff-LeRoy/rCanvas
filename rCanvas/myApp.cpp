/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           myApp.cpp
// Purpose:             Arrange photo references into a collage
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#include <wx/popupwin.h>
#include "CanvasPopup.h"
#include "ImageWidget.h"
#include "Canvas.h"
#include "myApp.h"

//---------------------------------------------------------------------------
// MainFrame
//---------------------------------------------------------------------------

MainFrame::MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
{
    Bind(wxEVT_CHAR_HOOK, &MainFrame::OnAbout, this);
}

void MainFrame::OnAbout(wxKeyEvent& event)
{
    wxChar key = event.GetKeyCode();

    if (key == WXK_F1)
    {
        delete helpPopup;
        wxBoxSizer* sizerVertical = new wxBoxSizer(wxVERTICAL);
        helpPopup = new CanvasPopup(this);
        helpPopup->SetSize(wxSize(200, 200));
        helpPopup->Position(ClientToScreen(wxPoint(0, 0)), wxSize(25, 25));

        wxStaticText* text = new wxStaticText(helpPopup, wxID_ANY,
            " \n"
            " GLOBAL SHORTCUTS\n"
            " ----------------------------------------------------------------------------\n"
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
        helpPopup->SetSizerAndFit(sizerVertical);
        helpPopup->Popup();
    }
    event.Skip();
}

//---------------------------------------------------------------------------
// Application
//---------------------------------------------------------------------------

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100, 100), wxSize(1280, 900));

    //Setup status bar
    wxStatusBar* statusBar = mainFrame->CreateStatusBar(2);
    int widths[2]{ -1, 300 };
    statusBar->SetStatusWidths(2, widths);

    ImageCanvas* canvas = new ImageCanvas(mainFrame, wxID_ANY, *statusBar);

    sizer->Add(canvas, 1, wxEXPAND);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);

    //ImageWidget* img = new ImageWidget(canvas, wxID_ANY, wxPoint(1000, 0), wxDefaultSize, "Images/image_grid.jpg", canvas->m_panCanvas, *statusBar);

    //Centers the scrollbars for startup
    canvas->CenterScrollbars();

    //Get view start for start up, this also needs to be done after CenterScrollbars or else it is 0,0
    //which is not accurate after the canvas is centered
    canvas->SettViewStart(canvas->GetViewStart());
    
    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);
