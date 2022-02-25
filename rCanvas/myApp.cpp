/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           myApp.cpp
// Purpose:             Arrange photo references into a collage
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "ImageWidget.h"
#include "Canvas.h"
#include "myApp.h"

//---------------------------------------------------------------------------
// MainFrame
//---------------------------------------------------------------------------

MainFrame::MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
{
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

    canvas->centerScrollbars();
    
    //ImageWidget* img = new ImageWidget(canvas, wxID_ANY, wxPoint(200, 200), wxDefaultSize, "Images/image_grid.jpg", canvas->m_panCanvas, *statusBar);

    mainFrame->Center();

    return true;
}


wxIMPLEMENT_APP(MyApp);
