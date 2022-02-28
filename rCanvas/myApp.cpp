/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           myApp.cpp
// Purpose:             Arrange photo references into a collage
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/popupwin.h>
#include "PopupWindow.h"
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
        helpPopup = new PopupWindow(this);
        helpPopup->SetSize(wxSize(390, 280));
        //wxPoint mPos = wxGetMousePosition();
        //wxPoint pos = ScreenToClient(mPos);
        helpPopup->Position(ClientToScreen(wxPoint(0, 0)), wxSize(390, 280));

        wxStaticText* text = new wxStaticText(helpPopup, wxID_ANY,
            " \n"
            " GLOBAL SHORTCUTS\n"
            " ----------------------------------------------------------------------------\n"
            " Pan canvas \t\t\t| Right Mouse + Drag\n"
            " Accelerate pan speed \t\t| CTRL + Right Mouse + Drag\n"
            " Center canvas \t\t\t| a\n"
            " Open a new image \t\t| o\n"
            " \n"
            " \n"
            " IMAGE-WIDGET SHORTCUTS\n"
            " (When mouse is hovering over an image)\n"
            " ----------------------------------------------------------------------------\n"
            " Scale image up / down \t\t| Mouse Scrollwheel\n"
            " Accelerate Scaling speed \t\t| CTRL + Mouse Scrollwheel\n"
            " Delete an ImageWidget \t\t| d\n"
            " Move image widget \t\t| Left Mouse + Drag\n"
            " Move without changing z-order \t| Alt + Left Mouse + Drag\n"
            " Set image to original size \t\t| f\n"
        );
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
    wxStatusBar* statusBar = mainFrame->CreateStatusBar();
    ImageCanvas* canvas = new ImageCanvas(mainFrame, wxID_ANY, *statusBar);

    sizer->Add(canvas, 1, wxEXPAND);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);

    canvas->CenterScrollbars();
    
    //ImageWidget* img = new ImageWidget(canvas, wxID_ANY, wxPoint(0, 0), wxDefaultSize, "Images/image_grid.jpg", canvas->m_panCanvas, *statusBar);

    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);
