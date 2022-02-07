#include <wx/wx.h>
#include "ImageWidget.h"
#include "ImageCanvas.h"
#include "rCanvas.h"

//---------------------------------------------------------------------------
// MainFrame
//---------------------------------------------------------------------------

MainFrame::MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
{
    CreateStatusBar();
}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a mainFrame and mainImageCanvas
    MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(1280,720));
    ImageCanvas* mainImageCanvas = new ImageCanvas(mainFrame, wxID_ANY);

    ImageWidget* imageWidget02 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(500, 500), "image2.jpg");
    ImageWidget* imageWidget01 = new ImageWidget(mainImageCanvas, wxID_ANY, wxPoint(200, 200), wxSize(250, 250), "image3.jpg");

    //Add panel to sizer, fit frame to sizer
    sizer->Add(mainImageCanvas, 1, wxEXPAND | wxALL, 3);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);
    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);
