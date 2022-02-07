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

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, "panScrollbarWithMouseDrag", wxPoint(100, 100), wxSize(1280, 720));
    ImageCanvas* canvas = new ImageCanvas(mainFrame, wxID_ANY);

    ImageWidget* imageWidget02 = new ImageWidget(canvas, wxID_ANY, wxDefaultPosition, wxSize(500, 500), "image2.jpg");
    ImageWidget* imageWidget01 = new ImageWidget(canvas, wxID_ANY, wxPoint(200, 200), wxSize(250, 250), "image3.jpg");

    sizer->Add(canvas, 1, wxEXPAND);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);

    canvas->centerScrollbars();
    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);
