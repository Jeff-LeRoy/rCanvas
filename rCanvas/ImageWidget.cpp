#include <wx/wx.h>
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath)
    :wxPanel(parent, id, pos, size)
{
    //load image on heap
    m_Image = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);
    
    if (!m_Image->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    //Set size of widget
    m_ImgWidth = m_Image->GetWidth();
    m_ImgHeight = m_Image->GetHeight();
    this->SetSize(wxSize(m_ImgWidth, m_ImgHeight));

    Raise();

    //Bind Shortcuts
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftDown, this);
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);
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
    m_imageWidgetClickPos.x = event.GetX();
    m_imageWidgetClickPos.y = event.GetY();
    m_WidgetDragging = true;

    //Set Z order to top
    Raise();

    Bind(wxEVT_LEFT_UP, &ImageWidget::leftUp, this);
    Bind(wxEVT_MOTION, &ImageWidget::mouseMoving, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::leftUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }

    m_WidgetDragging = false;

    Unbind(wxEVT_LEFT_UP, &ImageWidget::leftUp, this);
    Unbind(wxEVT_MOTION, &ImageWidget::mouseMoving, this);
    Unbind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::mouseMoving(wxMouseEvent& event)
{
    if (m_WidgetDragging)
    {
        //When you click inside an ImageWidget LeftDown() will store a mouse position relative 
        //to the inside of that widget. Positioning an ImageWidget/wxPanel relies on its top left 
        //corner, so to get those coordinates we will get the SCREEN position of the mouse then  
        //subtract the offset from inside the widget, now we have the top left corner. Then we just
        //need to convert from SCREEN coordinates to client window coordinates

        wxPoint screenMousePos = wxGetMousePosition();

        int TopLeftCorner_x = screenMousePos.x - m_imageWidgetClickPos.x;
        int TopLeftCorner_y = screenMousePos.y - m_imageWidgetClickPos.y;

        //Move box to converted screen position (m_parent var is from window.h)
        this->Move(m_parent->ScreenToClient(wxPoint(TopLeftCorner_x, TopLeftCorner_y)));

        //Need to do this otherwise dragging an ImageWidget leave artifacts
        //GetParent()->ClearBackground();
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
}


