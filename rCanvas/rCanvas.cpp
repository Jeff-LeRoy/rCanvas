#include <wx/wx.h>
#include "rCanvas.h"

//---------------------------------------------------------------------------
// ImageCanvas
//---------------------------------------------------------------------------

ImageCanvas::ImageCanvas(wxWindow* parent, wxWindowID id, wxString imgPath)
    : wxScrolledWindow(parent, id)
{
    //this->SetBackgroundColour(wxColor(15, 68, 125));

    //load image on heap
    //image = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);
    //m_imgWidth = image->GetWidth();
    //m_imgHeight = image->GetHeight();
    //imageBoundingBox = wxRect(0, 0, m_imgWidth, m_imgHeight);

    //if (!image->IsOk())
    //{
    //    wxMessageBox("There was an error loading the image.");
    //    return;
    //}

    //SetScrollbars(1, 1, m_imgWidth, m_imgHeight, 0, 0);
    //SetScrollRate(5, 5);

    //Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::rightDown, this);
    //Bind(wxEVT_RIGHT_UP, &ImageCanvas::rightUp, this);
    Bind(wxEVT_MOTION, &ImageCanvas::inMotion, this);
    //Bind(wxEVT_LEFT_DOWN, &ImageCanvas::leftDown, this);

}

ImageCanvas::~ImageCanvas()
{
    delete image;
}

void ImageCanvas::rightDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();

    //CaptureMouse();
    //wxLogStatus("X=" + wxString::Format(wxT("%d"), pos.x) + " " + "Y=" + wxString::Format(wxT("%d"), pos.y));
    return;
}

void ImageCanvas::rightUp(wxMouseEvent& event)
{
    //wxPoint pos = event.GetPosition();
    //wxLogStatus("X=" + wxString::Format(wxT("%d"), pos.x) + " " + "Y=" + wxString::Format(wxT("%d"), pos.y));
    wxLogStatus("up");
    return;
}

void ImageCanvas::leftDown(wxMouseEvent& event)
{
    if (imageBoundingBox.Contains(event.GetPosition()) && event.LeftIsDown())
        wxLogStatus("Clicked Inside Box");
    else
        wxLogStatus("");
}

void ImageCanvas::inMotion(wxMouseEvent& event)
{
    //if (event.RightIsDown())
    //{
        wxPoint pos = event.GetPosition();
        wxLogStatus(wxString::Format(wxT("%d"), pos.x) + " " + wxString::Format(wxT("%d"), pos.y));
    //}
    return;
}

void ImageCanvas::OnDraw(wxDC& dc)
{
    //IsDoubleBuffered();
    //dc.DrawBitmap(*image, 0, 0, false);
    //dc.DrawRectangle(imageBoundingBox);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
    //wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
}

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, const wxPoint pos, wxSize size)
    :wxPanel(parent, id, pos, size)
{
    parent = ImageWidget::parent;

    this->SetBackgroundColour(wxColor(15, 68, 125));

    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftDown, this);
    Bind(wxEVT_LEFT_UP, &ImageWidget::leftUp, this);
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftDown, this);
    Bind(wxEVT_MOTION, &ImageWidget::mouseMoving, this);
}

void ImageWidget::leftDown(wxMouseEvent& event)
{
    CaptureMouse();
    //wxPoint pos = event.GetPosition();
    m_x = event.GetX();
    m_y = event.GetY();
    m_mouseDragging = true;
    //this->Move((wxPoint(x += 5, y += 5)));
    //wxLogStatus("X=" + wxString::Format(wxT("%d"), pos.x) + " " + "Y=" + wxString::Format(wxT("%d"), pos.y));
}

void ImageWidget::leftUp(wxMouseEvent& event)
{
    ReleaseMouse();
    m_mouseDragging = false;
    //wxLogStatus("L up");
}

void ImageWidget::mouseMoving(wxMouseEvent& event)
{
    //if (m_mouseDragging & event.Dragging())
    //    wxLogStatus("Dragging");
    //else
    //    wxLogStatus("Not Dragging");


    if (m_mouseDragging)
    {
        wxPoint mouseOnScreen = wxGetMousePosition();
        int newx = mouseOnScreen.x - m_x;
        int newy = mouseOnScreen.y - m_y;
        //this->Move(ImageWidget::parent->ScreenToClient(wxPoint(newx, newy)));
        this->Move((wxPoint(newx, newy)));
    }

}

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a frame and Panel
    wxFrame* mainFrame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(854,480));
    wxStatusBar* statusBar = mainFrame->CreateStatusBar();
    ImageCanvas* mainImageCanvas = new ImageCanvas(mainFrame, wxID_ANY, "image.jpg");
    ImageWidget* imageWidget01 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(100, 100));

    //Add panel to sizer, fit frame to sizer
    sizer->Add(mainImageCanvas, 1, wxEXPAND | wxALL, 5);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);
    mainFrame->Center();

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