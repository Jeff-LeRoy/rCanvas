/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           ImageWidget.cpp
// Purpose:             Handle transforming and manipulating images on canvas
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath)
    :wxPanel(parent, id, pos, size)
{
    this->SetBackgroundColour(wxColor(77, 38, 39));

    m_imgPath = imgPath;

    //load image to heap
    m_bitmap = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);

    if(!m_bitmap->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    //Set size of widget
    m_scale.m_x = m_bitmap->GetWidth();
    m_scale.m_y = m_bitmap->GetHeight();
    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));

    //Store original dimensions of image
    m_originalDimensions.x = m_bitmap->GetWidth();
    m_originalDimensions.y = m_bitmap->GetHeight();

    Raise();

    //Bind Shortcuts
    Bind(wxEVT_MOUSEWHEEL, &ImageWidget::scrollWheelZoom, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageWidget::rightIsDown, this);
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftIsDown, this);
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);
    Bind(wxEVT_MOTION, &ImageWidget::hoverPrinting, this);//Remove later
    Bind(wxEVT_CHAR_HOOK, &ImageWidget::onKey_T, this);//For testing
    Bind(wxEVT_LEFT_DCLICK, &ImageWidget::onRightDClick, this);
}

ImageWidget::~ImageWidget()
{
    delete m_bitmap;
}

void ImageWidget::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    if (m_scalingImage)
    {
        renderScaled(dc);
    }
    else
        render(dc);

}

void ImageWidget::render(wxDC& dc)
{
    //wxLogStatus("render");
    dc.DrawBitmap(*m_bitmap, 0, 0, true);
}

void ImageWidget::renderScaled(wxDC& dc)
{
    //Convert bitmap to wxImage for scaling
    m_image = new wxImage();
    m_image->LoadFile(m_imgPath, wxBITMAP_TYPE_JPEG);

    //Convert wxImage to wxBitmap for drawing
    *m_bitmap = wxBitmap(m_image->Scale(m_scale.m_x, m_scale.m_y));
    delete m_image;

    dc.DrawBitmap(*m_bitmap, 0, 0, true);

    //Draw border around image
    //dc.SetPen(wxPen(wxColor(245, 55, 55), 5));
    //dc.DrawLine(wxPoint(0, 0), wxPoint(m_scale.m_x, 0));
    //dc.DrawLine(wxPoint(0, 0), wxPoint(0, m_scale.m_y));
    //dc.DrawLine(wxPoint(m_scale.m_x, 0), wxPoint(m_scale.m_x, m_scale.m_y));
    //dc.DrawLine(wxPoint(0, m_scale.m_y), wxPoint(m_scale.m_x, m_scale.m_y));
 
    m_scalingImage = false;

    //Refresh();
}

void ImageWidget::calculateAspectRatio()
{
    //Rescale formula -> Width = Original Width * ( Percentage / 100 )
    //Aspect ratio formula -> (org. height / org. width) x new width = new height

    //Retain aspect ratio and calculate new width then height
    wxPoint2DDouble oldScale = m_scale;
    //Calculate width
    m_scale.m_x = oldScale.m_x * (m_scaleIncrimentor / 100.0);

    //Calculate new height
    m_aspectRatio = (oldScale.m_y / oldScale.m_x);
    m_scale.m_y = m_aspectRatio * m_scale.m_x;
}

void ImageWidget::setGlobalScale()
{
    //baseOffset + (pivot * height) = finalPos

    m_scale.m_x = 256;
    m_scale.m_y = 256;
    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));
}

//---------------------------------------------------------------------------
// Mouse Handlers
//---------------------------------------------------------------------------

void ImageWidget::onKey_T(wxKeyEvent& event)
{
    //wxChar key = event.GetUnicodeKey();
    //if (key == 'T')
    //{
    //    wxLogStatus("fjfjfjfjfj");
    //    m_scale.m_x = 256;
    //    m_scale.m_y = 256;
    //    m_scalingImage = true;
    //    Refresh();
    //    //m_image->GetSubImage(wxRect(wxPoint(10,10),wxSize(512,512)));
    //}
    //event.Skip();
}

void ImageWidget::scrollWheelZoom(wxMouseEvent& event)
{
    m_scalingImage = true;

    //Speed up scaling if CTRL is down
    if (event.ControlDown())
        m_scaleMultiplier = 8;
    else
        m_scaleMultiplier = 1;

    int rot = event.GetWheelRotation();
    int delta = event.GetWheelDelta();

    //incriment scale
    m_scaleIncrimentor = 100;
    m_scaleIncrimentor += (double)m_scaleMultiplier * (rot / delta);

    calculateAspectRatio();

    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));
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

void ImageWidget::hoverPrinting(wxMouseEvent& event)//Remove later
{
    //wxPoint pos = wxGetMousePosition();

    //int x = pos.x;
    //int y = pos.y;

    //wxPoint scrn = m_parent->ScreenToClient(wxPoint(x, y));

    //wxLogStatus(/*" X=" + wxString::Format(wxT("%lf"), m_scale.m_x) + ' ' +
    //            " Y=" + wxString::Format(wxT("%lf"), m_scale.m_y) + ' ' +
    //            " Percent=" + wxString::Format(wxT("%lf"), m_scaleIncrimentor) + ' ' +
    //            " OrigX=" + wxString::Format(wxT("%d"), originalDimensions.x) + ' ' +
    //            " OrigY=" + wxString::Format(wxT("%d"), originalDimensions.y) + ' ' +*/
    //    " aspect=" + wxString::Format(wxT("%lf"), m_aspectRatio) + ' ' +
    //    " mainScrnMPosX=" + wxString::Format(wxT("%d"), pos.x) + ' ' +
    //    " mainScrnMPosY=" + wxString::Format(wxT("%d"), pos.y) + ' ' +
    //    " scrnX=" + wxString::Format(wxT("%d"), scrn.x) + ' ' +
    //    " scrnY=" + wxString::Format(wxT("%d"), scrn.y)
    //);
}

void ImageWidget::leftIsDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxSetCursor(wxCURSOR_HAND);

    //Get client mouse position
    m_imageWidgetClickPos.x = event.GetX();
    m_imageWidgetClickPos.y = event.GetY();
    m_widgetDragging = true;

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

    m_widgetDragging = false;

    Unbind(wxEVT_LEFT_UP, &ImageWidget::leftIsUp, this);
    Unbind(wxEVT_MOTION, &ImageWidget::leftIsDragging, this);
    Unbind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::leftIsDragging(wxMouseEvent& event)
{
    if (m_widgetDragging)
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

void ImageWidget::onRightDClick(wxMouseEvent& event)
{
    SetFocus();
    m_scalingImage = true;
}





