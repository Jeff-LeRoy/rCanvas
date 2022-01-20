#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "rCanvas.h"

ImagePanel::ImagePanel(wxWindow* parent) : wxPanel(parent)
{
    this->SetBackgroundColour(wxColor(15, 68, 125));
    image.LoadFile("image.jpg", wxBITMAP_TYPE_JPEG);
}

void ImagePanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    DrawMyImage(dc);
}

void ImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    DrawMyImage(dc);
}

void ImagePanel::DrawMyImage(wxDC& dc)
{
    dc.DrawBitmap(image, 0, 0, false);
}

//MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "rCanvas")
//{
//
//}

bool MyApp::OnInit()
{
    //Initialize all image handlers
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a frame and Panel
    wxFrame* mainFrame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(640,480));
    ImagePanel* imagePanel = new ImagePanel(mainFrame);

    //Add panel to sizer, fit frame to sizer
    sizer->Add(imagePanel, 1, wxEXPAND);
    mainFrame->SetSizerAndFit(sizer);
    mainFrame->Show(true);

    return true;
}

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