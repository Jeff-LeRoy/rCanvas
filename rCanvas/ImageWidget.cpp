#include <wx/wx.h>
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, const wxPoint pos, wxSize size, wxString imgPath)
    :wxPanel(parent, id, pos, size)
{
    this->SetBackgroundColour(wxColor(15, 68, 125));
    //ImageWidget::m_IwParent = parent;

    //load image on heap
    m_Image = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);

    //Set size of widget
    m_ImgWidth = m_Image->GetWidth();
    m_ImgHeight = m_Image->GetHeight();
    this->SetSize(wxSize(m_ImgWidth, m_ImgHeight));

    if (!m_Image->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftDown, this);
    Bind(wxEVT_LEFT_UP, &ImageWidget::leftUp, this);
    Bind(wxEVT_MOTION, &ImageWidget::mouseMoving, this);
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

ImageWidget::~ImageWidget()
{
    delete m_Image;
}

void ImageWidget::leftDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxSetCursor(wxCURSOR_HAND);

    //Get client mouse position
    m_MouseLocal_x = event.GetX();
    m_MouseLocal_y = event.GetY();
    m_MouseDragging = true;

    //Set Z order to top
    Raise();
}

void ImageWidget::leftUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }

    m_MouseDragging = false;
}

void ImageWidget::mouseMoving(wxMouseEvent& event)
{
    if (m_MouseDragging)
    {
        //Get screen space mouse pos
        wxPoint mouseScreen = wxGetMousePosition();

        //Subtract local mouse pos from screen pos
        int mousePosConverted_x = mouseScreen.x - m_MouseLocal_x;
        int mousePosConverted_y = mouseScreen.y - m_MouseLocal_y;

        //Move box to converted screen position
        //m_parent from window.h
        this->Move(m_parent->ScreenToClient(wxPoint(mousePosConverted_x, mousePosConverted_y)));

        //Need to do this otherwise dragging an ImageWidget leave artifacts
        GetParent()->ClearBackground();

        //wxLogStatus("mouseScreen X=" + wxString::Format(wxT("%d"), mouseScreen.x) + " " +
        //            "mouseScreen Y=" + wxString::Format(wxT("%d"), mouseScreen.y) + " " +
        //            "L X=" + wxString::Format(wxT("%d"), mouseLocal_x) + " " +
        //            "L Y=" + wxString::Format(wxT("%d"), mouseLocal_y)
        //            );
    }
}

void ImageWidget::OnCaptureLost(wxMouseCaptureLostEvent&)
{
    wxSetCursor(wxNullCursor);

    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void ImageWidget::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*m_Image, 0, 0, false);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
}
