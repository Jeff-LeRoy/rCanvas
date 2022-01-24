#include <wx/wx.h>
#include "rCanvas.h"


ImagePanel::ImagePanel(wxWindow* parent, wxWindowID id, wxString imgPath)
    : wxScrolledWindow(parent, id)
{
    //this->SetBackgroundColour(wxColor(15, 68, 125));

    //load image on heap
    image = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);
    m_imgWidth = image->GetWidth();
    m_imgHeight = image->GetHeight();

    if (!image->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    SetScrollbars(1, 1, m_imgWidth, m_imgHeight, 0, 0);
    SetScrollRate(5, 5);

    Bind(wxEVT_RIGHT_DOWN, &ImagePanel::rightDown, this);
    Bind(wxEVT_RIGHT_UP, &ImagePanel::rightUp, this);
    Bind(wxEVT_MOTION, &ImagePanel::inMotion, this);
}

ImagePanel::~ImagePanel()
{
    delete image;
}

void ImagePanel::rightDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    //CaptureMouse();
    wxLogStatus("X=" + wxString::Format(wxT("%d"), pos.x) + " " + "Y=" + wxString::Format(wxT("%d"), pos.y));
    return;
}

void ImagePanel::rightUp(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    wxLogStatus("X=" + wxString::Format(wxT("%d"), pos.x) + " " + "Y=" + wxString::Format(wxT("%d"), pos.y));
    return;
}

void ImagePanel::inMotion(wxMouseEvent& event)
{
    if (event.RightIsDown())
    {
        wxPoint pos = event.GetPosition();
        wxLogStatus(wxString::Format(wxT("%d"), pos.x) + " " + wxString::Format(wxT("%d"), pos.y));
    }
    return;
}

void ImagePanel::OnDraw(wxDC& dc)
{
    dc.DrawBitmap(*image, 0, 0, false);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
    dc.SetLogicalScale(0.5, 0.5);
}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a frame and Panel
    wxFrame* mainFrame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(854,480));
    wxStatusBar* statusBar = mainFrame->CreateStatusBar();
    ImagePanel* imagePanel = new ImagePanel(mainFrame, wxID_ANY, "image2.jpg");

    //Add panel to sizer, fit frame to sizer
    sizer->Add(imagePanel, 1, wxEXPAND | wxALL, 5);
    mainFrame->SetSizer(sizer);
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