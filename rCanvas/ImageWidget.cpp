#include <wx/wx.h>
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath)
    :wxPanel(parent, id, pos, size)
{
    //load image on heap
    m_image = new wxImage();
    m_image->LoadFile(imgPath, wxBITMAP_TYPE_JPEG);

    if(!m_image->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    //Set size of widget
    m_scale.x = m_image->GetWidth();
    m_scale.y = m_image->GetHeight();
    this->SetSize(wxSize(m_scale.x, m_scale.y));

    Raise();

    //Bind Shortcuts
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftDown, this);
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &ImageWidget::mouseScrolling, this);

}

ImageWidget::~ImageWidget()
{
    delete m_image;
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

    //wxLogStatus("X= " + wxString::Format(wxT("%d"), m_scale.x) + ' ' +
    //    "Y= " + wxString::Format(wxT("%d"), m_scale.y));
    wxLogStatus("X= " + wxString::Format(wxT("%lf"), percent));


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

    //SCALE
    //wxBitmap* m_bitmap = new wxBitmap(m_image->Scale(128,128));

    //Convert wxImage to wxBitmap for drawing
    wxBitmap* m_bitmap = new wxBitmap(m_image->Scale(m_scale.x, m_scale.y));

    dc.DrawBitmap(*m_bitmap, 0, 0, true);

    //Delete drawing bitmap 
    delete m_bitmap;
}

void ImageWidget::mouseScrolling(wxMouseEvent& event) 
{
    //Width = Original Width * ( Percentage / 100 )

    int rot = event.GetWheelRotation();
    int delta = event.GetWheelDelta();
    
    wxPoint oldScale = m_scale;

    //m_scale.x += percent * (rot / delta);
    //m_scale.y += percent * (rot / delta);
    
    percent = 100;
    percent +=  (rot / delta);

    m_scale.x = oldScale.x * (percent / 100.0);
    m_scale.y = oldScale.y * (percent / 100.0);


    this->SetSize(wxSize(m_scale.x, m_scale.y));
}


