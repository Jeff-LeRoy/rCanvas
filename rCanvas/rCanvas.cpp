#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "rCanvas.h"


ImagePanel::ImagePanel(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id)
{
    //this->SetBackgroundColour(wxColor(15, 68, 125));

    //load image on heap
    image = new wxBitmap("image.jpg", wxBITMAP_TYPE_JPEG);
    m_imgWidth = image->GetWidth();
    m_imgHeight = image->GetHeight();

    if (!image->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }
    SetScrollbars(1, 1, m_imgWidth, m_imgHeight, 0, 0);
    SetScrollRate(5, 5);
}

ImagePanel::~ImagePanel()
{
    delete image;
}

void ImagePanel::rightClick(wxMouseEvent& event)
{
    wxMessageBox("Clicked right mouse button");
}

void ImagePanel::OnDraw(wxDC& dc)
{
    dc.DrawBitmap(*image, 0, 0, false);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a frame and Panel
    wxFrame* mainFrame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(854,480));
    wxStatusBar* statusBar = mainFrame->CreateStatusBar();
    ImagePanel* imagePanel = new ImagePanel(mainFrame, wxID_ANY);

    //Add panel to sizer, fit frame to sizer
    sizer->Add(imagePanel, 1, wxEXPAND | wxALL, 5);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);

    return true;
}

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_RIGHT_DOWN(ImagePanel::rightClick)
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