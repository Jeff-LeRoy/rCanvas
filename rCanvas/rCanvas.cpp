#include <wx/wx.h>
#include "rCanvas.h"

//---------------------------------------------------------------------------
// ImageCanvas
//---------------------------------------------------------------------------

ImageCanvas::ImageCanvas(wxWindow* parent, wxWindowID id, wxString imgPath)
    : wxScrolledWindow(parent, id)
{
    this->SetBackgroundColour(wxColor(34, 35, 37));
    this->SetDoubleBuffered(true);

    SetScrollbars(1, 1, 1000, 1000, 0, 0);
    SetScrollRate(5, 5);

    Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::rightDown, this);
    Bind(wxEVT_RIGHT_UP, &ImageCanvas::rightUp, this);
    Bind(wxEVT_MOTION, &ImageCanvas::inMotion, this);
    Bind(wxEVT_LEFT_DOWN, &ImageCanvas::leftDown, this);
}

ImageCanvas::~ImageCanvas(){}

void ImageCanvas::rightDown(wxMouseEvent& event){}

void ImageCanvas::rightUp(wxMouseEvent& event){}

void ImageCanvas::leftDown(wxMouseEvent& event)
{
 /*   if (imageBoundingBox.Contains(event.GetPosition()) && event.LeftIsDown())
        wxLogStatus("Clicked Inside Box");
    else*/
        //wxLogStatus("clicked in canvas");
}

void ImageCanvas::inMotion(wxMouseEvent& event)
{
    //if (event.RightIsDown())
    //{
        //wxPoint pos = event.GetPosition();
        //wxLogStatus("X=" + wxString::Format(wxT("%d"), pos.x) + " " + "Y=" + wxString::Format(wxT("%d"), pos.y));
    //}
    //return;
}

void ImageCanvas::OnDraw(wxDC& dc)
{
    //dc.DrawRectangle(imageBoundingBox);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
    //wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
}

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, const wxPoint pos, wxSize size, wxString imgPath)
    :wxPanel(parent, id, pos, size)
{
    ImageWidget::m_parent = parent;

    //load image on heap
    image = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);
    m_imgWidth = image->GetWidth();
    m_imgHeight = image->GetHeight();
    //imageBoundingBox = wxRect(0, 0, m_imgWidth, m_imgHeight);

    if (!image->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    this->SetBackgroundColour(wxColor(15, 68, 125));

    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftDown, this);
    Bind(wxEVT_LEFT_UP, &ImageWidget::leftUp, this);
    Bind(wxEVT_MOTION, &ImageWidget::mouseMoving, this);
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);

}

ImageWidget::~ImageWidget()
{
    delete image;
}

void ImageWidget::leftDown(wxMouseEvent& event)
{
    CaptureMouse();

    //Get local/client mouse position
    mouseLocal_x = event.GetX();
    mouseLocal_y = event.GetY();

    m_mouseDragging = true;
}

void ImageWidget::leftUp(wxMouseEvent& event)
{
    ReleaseMouse();
    m_mouseDragging = false;
}

void ImageWidget::mouseMoving(wxMouseEvent& event)
{
    if (m_mouseDragging)
    {
        //Get screen space mouse pos
        wxPoint mouseScreen = wxGetMousePosition();

        //Subtract local mouse pos from screen pos
        int mousePosConverted_x = mouseScreen.x - mouseLocal_x;
        int mousePosConverted_y = mouseScreen.y - mouseLocal_y;

        //Move box to converted screen position
        this->Move(m_parent->ScreenToClient(wxPoint(mousePosConverted_x, mousePosConverted_y)));

        //Need to do this otherwise dragging image widgets leave artifacts
        GetParent()->ClearBackground();

        //wxLogStatus("mouseScreen X=" + wxString::Format(wxT("%d"), mouseScreen.x) + " " +
        //            "mouseScreen Y=" + wxString::Format(wxT("%d"), mouseScreen.y) + " " +
        //            "L X=" + wxString::Format(wxT("%d"), mouseLocal_x) + " " +
        //            "L Y=" + wxString::Format(wxT("%d"), mouseLocal_y)
        //            );
    }
}

void ImageWidget::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    //DrawMyDocument(dc);
    dc.DrawBitmap(*image, 0, 0, false);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
}

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a mainFrame and Canvas
    wxFrame* mainFrame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(1280,720));
    wxStatusBar* statusBar = mainFrame->CreateStatusBar();
    ImageCanvas* mainImageCanvas = new ImageCanvas(mainFrame, wxID_ANY, "image2.jpg");

    ImageWidget* imageWidget01 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(250, 250), "image2.jpg");
    ImageWidget* imageWidget02 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(250, 250), "image2.jpg");

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