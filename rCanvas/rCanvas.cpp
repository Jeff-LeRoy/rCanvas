#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "rCanvas.h"


ImagePanel::ImagePanel(wxWindow* parent, wxWindowID id) 
    : wxScrolledWindow(parent, id)
{
    //this->SetBackgroundColour(wxColor(15, 68, 125));

    image.LoadFile("image.jpg", wxBITMAP_TYPE_JPEG);
    if (!image.IsOk())
    {
        wxMessageBox("there was an error loading the image");
        return;
    }
    w = image.GetWidth();
    h = image.GetHeight();

    SetScrollbars(1, 1, w, h, 0, 0);
}

void ImagePanel::OnRender(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
    dc.DrawBitmap(image, 0, 0, false);
}

//void ImagePanel::OnDraw(wxDC& dc)
//{
//    dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
//    //dc.DrawBitmap(image, 0, 0, false);
//}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a frame and Panel
    wxFrame* mainFrame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(854,480));
    ImagePanel* imagePanel = new ImagePanel(mainFrame, wxID_ANY);
    mainFrame->CreateStatusBar();

    //Add panel to sizer, fit frame to sizer
    sizer->Add(imagePanel, 1, wxEXPAND);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);

    return true;
}

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::OnRender)
END_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

//void MyFrame::OnImport(wxCommandEvent& event)
//{
//    wxFileDialog openFileDialog
//    (this, _("Open Image"), "", "", ".jpg files (*.jpg)|*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
//
//    if (openFileDialog.ShowModal() == wxID_CANCEL)
//        return;
//
//    //Get path to image
//    wxString fileLocation = (openFileDialog.GetPath());
//    wxString fileName = openFileDialog.GetFilename();
//}