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
// Constructor / Destructor
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath, const bool& m_panCanvas)
    :wxPanel(parent, id, pos, size)
{
    this->SetBackgroundColour(wxColor(77, 38, 39));

    //Get info from Canvas
    m_imgPath = imgPath;
    m_isCanvasPanning = &m_panCanvas;

    //load image to heap
    m_bitmap = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);
    if(!m_bitmap->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    //Store original dimensions of image
    m_originalDimensions.x = m_bitmap->GetWidth();
    m_originalDimensions.y = m_bitmap->GetHeight();

    //If image is larger than client Y, Scale it
    wxPoint clientSize{}; 
    GetParent()->GetClientSize(&clientSize.x, &clientSize.y);
    if (m_bitmap->GetHeight() >= clientSize.y)
        rescaleImage(m_bitmap, clientSize.y);

    //Set size of widget
    m_scale.m_x = m_bitmap->GetWidth();
    m_scale.m_y = m_bitmap->GetHeight();
    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));

    Raise();

    //Bind Mouse Events
    Bind(wxEVT_MOUSEWHEEL, &ImageWidget::scrollWheelZoom, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageWidget::rightIsDown, this);
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::leftIsDown, this);
    Bind(wxEVT_ENTER_WINDOW, &ImageWidget::enterWindow, this);
    Bind(wxEVT_LEAVE_WINDOW, &ImageWidget::exitWindow, this);
    Bind(wxEVT_MOTION, &ImageWidget::hoverPrinting, this);//Remove later
    //Bind Paint
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);

}

ImageWidget::~ImageWidget()
{
    delete m_bitmap;
}

//---------------------------------------------------------------------------
// Paint / Render / Misc
//---------------------------------------------------------------------------

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
    dc.SetPen(wxPen(wxColor(245, 55, 55), 3));
    //dc.DrawLine(wxPoint(0, 0), wxPoint(m_scale.m_x, 0));
    //dc.DrawLine(wxPoint(0, 0), wxPoint(0, m_scale.m_y));
    //dc.DrawLine(wxPoint(m_scale.m_x, 0), wxPoint(m_scale.m_x, m_scale.m_y));
    //dc.DrawLine(wxPoint(0, m_scale.m_y), wxPoint(m_scale.m_x, m_scale.m_y));
 
    m_scalingImage = false;
}

void ImageWidget::calculateAspectRatio()
{
    //Retain aspect ratio and calculate new width then height
    //Rescale formula -> Width = Original Width * ( Percentage / 100 )
    //Aspect ratio formula -> (org. height / org. width) x new width = new height

    wxPoint2DDouble oldScale = m_scale;
    //Calculate width
    m_scale.m_x = oldScale.m_x * (m_scaleIncrimentor / 100.0);

    //Calculate new height
    m_aspectRatio = (oldScale.m_y / oldScale.m_x);
    m_scale.m_y = m_aspectRatio * m_scale.m_x;
}

void ImageWidget::calculateAspectRatio(int height)
{
    //Solve width while having new height
    //height * (old width / old Height)
    m_scale.m_y = (double)height - 20;

    double oldWidth = m_bitmap->GetWidth();
    double oldHeight = m_bitmap->GetHeight();
    double wh = (oldWidth / oldHeight);
    m_scale.m_x = ((double)height * wh);
}

void ImageWidget::rescaleImage(wxBitmap* bitmap, int max)
{
    //Convert bitmap to wxImage for scaling
    m_image = new wxImage();
    m_image->LoadFile(m_imgPath, wxBITMAP_TYPE_JPEG);

    calculateAspectRatio(max);

    //Convert wxImage to wxBitmap for drawing
    *m_bitmap = wxBitmap(m_image->Scale(m_scale.m_x, m_scale.m_y));
    delete m_image;
}

void ImageWidget::setGlobalScale()
{
    //baseOffset + (pivot * height) = finalPos
    m_scale.m_x = 256;
    m_scale.m_y = 256;
    this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));
}

//---------------------------------------------------------------------------
// Mouse / Keyboard Handlers
//---------------------------------------------------------------------------

void ImageWidget::hoverPrinting(wxMouseEvent& event)//Remove later
{
    //wxLogStatus(wxString::Format(wxT("%d"), *m_isCanvasPanning));

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

void ImageWidget::onKey_F(wxKeyEvent& event)
{
    //Rescales image to its original dimensions
    wxChar key = event.GetUnicodeKey();
    if (key == 'F')
    {
        m_scale.m_x = m_originalDimensions.x;
        m_scale.m_y = m_originalDimensions.y;
        this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));
        m_scalingImage = true;
        Refresh();
    }
    event.Skip();
}

void ImageWidget::onKey_D(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'D' && m_canDelete)
    {
        wxCloseEvent* close = new wxCloseEvent(wxEVT_CLOSE_WINDOW);
        close->SetCanVeto(true);

        if (close->CanVeto())
        {
            if (wxMessageBox("Delete this image ?",
                "Please confirm",
                wxICON_QUESTION | wxYES_NO) != wxYES)
            {
                close->Veto();
                return;
            }
        }
        Destroy();
    }
    else
        event.Skip();
}

void ImageWidget::scrollWheelZoom(wxMouseEvent& event)
{
    if (!m_widgetDragging && !*m_isCanvasPanning)
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

        //Set size of ImageWidget wxPanel
        this->SetSize(wxSize(m_scale.m_x, m_scale.m_y));
    }
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

void ImageWidget::enterWindow(wxMouseEvent& event)
{
    SetFocus();
    Bind(wxEVT_CHAR_HOOK, &ImageWidget::onKey_F, this);
    Bind(wxEVT_CHAR_HOOK, &ImageWidget::onKey_D, this);
    m_canDelete = true;
}

void ImageWidget::exitWindow(wxMouseEvent& event)
{
    Unbind(wxEVT_CHAR_HOOK, &ImageWidget::onKey_F, this);
    Unbind(wxEVT_CHAR_HOOK, &ImageWidget::onKey_D, this);
    m_canDelete = false;
}

void ImageWidget::OnCaptureLost(wxMouseCaptureLostEvent&)
{
    wxSetCursor(wxNullCursor);
    if (HasCapture())
    {
        ReleaseMouse();
    }
}
