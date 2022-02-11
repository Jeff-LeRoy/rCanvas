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
    m_scale.m_x = m_image->GetWidth();
    m_scale.m_y = m_image->GetHeight();
    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));

    //Store original dimensions of image
    originalDimensions.x = m_image->GetWidth();
    originalDimensions.y = m_image->GetHeight();

    Raise();

    //Bind Shortcuts
    Bind(wxEVT_MOUSEWHEEL, &ImageWidget::scrollWheelZoom, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageWidget::rightIsDown, this);
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftIsDown, this);
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);
    Bind(wxEVT_MOTION, &ImageWidget::hoverPrinting, this);//Remove later
}

ImageWidget::~ImageWidget()
{
    delete m_image;
}

void ImageWidget::hoverPrinting(wxMouseEvent& event)//Remove later
{
    wxPoint pos = wxGetMousePosition();

    int x = pos.x;
    int y = pos.y;

    wxPoint scrn = m_parent->ScreenToClient(wxPoint(x, y));

    wxLogStatus(/*" X=" + wxString::Format(wxT("%lf"), m_scale.m_x) + ' ' +
                " Y=" + wxString::Format(wxT("%lf"), m_scale.m_y) + ' ' +
                " Percent=" + wxString::Format(wxT("%lf"), m_scaleIncrimentor) + ' ' +
                " aspect=" + wxString::Format(wxT("%lf"), aspect) + ' ' +
                " OrigX=" + wxString::Format(wxT("%d"), originalDimensions.x) + ' ' +
                " OrigY=" + wxString::Format(wxT("%d"), originalDimensions.y) + ' ' +*/
                " mainScrnMPosX=" + wxString::Format(wxT("%d"), pos.x) + ' ' +
                " mainScrnMPosY=" + wxString::Format(wxT("%d"), pos.y) + ' ' +
                " scrnX=" + wxString::Format(wxT("%d"), scrn.x) + ' ' +
                " scrnY=" + wxString::Format(wxT("%d"), scrn.y)
    );
}

void ImageWidget::leftIsDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxSetCursor(wxCURSOR_HAND);

    //Get client mouse position
    m_imageWidgetClickPos.x = event.GetX();
    m_imageWidgetClickPos.y = event.GetY();
    m_WidgetDragging = true;

    //Set Z order to top
    Raise();

    Bind(wxEVT_LEFT_UP, &ImageWidget::leftIsUp, this);
    Bind(wxEVT_MOTION, &ImageWidget::leftIsDragging, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::leftIsUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }

    m_WidgetDragging = false;

    Unbind(wxEVT_LEFT_UP, &ImageWidget::leftIsUp, this);
    Unbind(wxEVT_MOTION, &ImageWidget::leftIsDragging, this);
    Unbind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::leftIsDragging(wxMouseEvent& event)
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

    //Convert wxImage to wxBitmap for drawing
    wxBitmap* m_bitmap = new wxBitmap(m_image->Scale(m_scale.m_x, m_scale.m_y));

    dc.DrawBitmap(*m_bitmap, 0, 0, true);

    //Delete drawing bitmap 
    delete m_bitmap;
}

void ImageWidget::scrollWheelZoom(wxMouseEvent& event) 
{
    //Width = Original Width * ( Percentage / 100 )
    //Aspect ratio formula -> (org. height / org. width) x new width = new height

    int rot = event.GetWheelRotation();
    int delta = event.GetWheelDelta();
    
    //incriment scale
    m_scaleIncrimentor = 100;
    m_scaleIncrimentor += static_cast<wxDouble>(m_scaleMultiplier) * (rot / delta);

    //Retain aspect ratio and calculate new width then height
    wxPoint2DDouble oldScale = m_scale;
    //Calculate width
    m_scale.m_x = oldScale.m_x * (m_scaleIncrimentor / 100.0);
    //Calculate new height
    m_aspect = (oldScale.m_y / oldScale.m_x);
    m_scale.m_y = m_aspect * m_scale.m_x;

    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));

    Refresh();
}

void ImageWidget::rightIsDown(wxMouseEvent& event)
{
    //Get SCREEN mouse pos and convert to client
    wxPoint pos = wxGetMousePosition();
    wxPoint scrn = m_parent->ScreenToClient(wxPoint(pos.x, pos.y));

    //Assign converted position to mouse event otherwise it is coordinates 
    //relative to top corner of the ImageWidget
    event.m_x = scrn.x;
    event.m_y = scrn.y;

    //Pass event to the parent handler
    wxPostEvent(GetParent(), event);
}




