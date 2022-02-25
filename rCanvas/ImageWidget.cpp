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

ImageWidget::ImageWidget(wxWindow* parent, 
                        wxWindowID id, 
                        wxPoint pos, 
                        wxSize size, 
                        wxString imgPath, 
                        const bool& m_panCanvas, 
                        wxStatusBar& statusBar)
:wxPanel(parent, id, pos, size)
{
    this->SetBackgroundColour(wxColor(77, 38, 39));

    //Get info from Canvas
    m_imgPath = imgPath;
    m_isCanvasPanning = &m_panCanvas;
    m_statusBar = &statusBar;

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

    m_scale.x = m_bitmap->GetWidth();
    m_scale.y = m_bitmap->GetHeight();
    //m_aspectRatio = ( (double)m_originalDimensions.y / m_originalDimensions.x);

    //Set size of widget
    this->SetSize(wxSize(m_scale.x, m_scale.y));

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
    *m_bitmap = wxBitmap(m_image->Scale(m_scale.x, m_scale.y));
    delete m_image;

    dc.DrawBitmap(*m_bitmap, 0, 0, true);

    m_scalingImage = false;
}

void ImageWidget::calculateAspectRatio()
{
    //Aspect ratio formula -> (org. height / org. width) x new width = new height
    //Solve for width - width / (original width / original Height);
    //Solve for height - height * (original width / original Height);

    wxPoint oldScale = m_scale;
    m_scale.x = oldScale.x * (m_scaleIncrimentor / 100.0);

    //Calculate new height
    //adding 0.5 causes the integer truncation to produce a rounding effect
    m_scale.y = ((double)m_scale.x * ((double)m_originalDimensions.y / (double)m_originalDimensions.x) + 0.5);

    //currentAspectRatio = ((double)m_scale.y / m_scale.x);
}

void ImageWidget::calculateAspectRatio(int clientSizeY)
{
    //Solve width while having new height
    m_scale.y = (double)clientSizeY - 20;

    double originalWidth = m_bitmap->GetWidth();
    double originalHeight = m_bitmap->GetHeight();
    m_scale.x = ((double)m_scale.y * ((double)originalWidth / (double)originalHeight) + 0.5);

    //currentAspectRatio = ((double)m_scale.y / m_scale.x);
}

void ImageWidget::rescaleImage(wxBitmap* bitmap, int max)
{
    //Convert bitmap to wxImage for scaling
    m_image = new wxImage();
    m_image->LoadFile(m_imgPath, wxBITMAP_TYPE_JPEG);

    calculateAspectRatio(max);

    //Convert wxImage to wxBitmap for drawing
    *m_bitmap = wxBitmap(m_image->Scale(m_scale.x, m_scale.y));
    delete m_image;
}

void ImageWidget::setGlobalScale()
{
    //baseOffset + (pivot * height) = finalPos
    m_scale.x = 256;
    m_scale.y = 256;
    this->SetSize(wxSize(m_scale.x, m_scale.y));
}

//---------------------------------------------------------------------------
// Mouse / Keyboard Handlers
//---------------------------------------------------------------------------

void ImageWidget::hoverPrinting(wxMouseEvent& event)//Remove later
{
//    m_statusBar->SetStatusText("Right click + drag to pan canvas \
//| Left click + drag to move image \
//| F - Restore original image size");
        


    wxPoint pos = wxGetMousePosition();

    int x = pos.x;
    int y = pos.y;

    //wxPoint scrn = event.GetPosition();
    wxPoint scrn = m_parent->ScreenToClient(wxPoint(x, y));
    wxPoint client = m_parent->ClientToScreen(wxPoint(x, y));


    wxLogStatus(
        " m_scaleX=" + wxString::Format(wxT("%d"), m_scale.x) + ' ' +
        " m_scaleY=" + wxString::Format(wxT("%d"), m_scale.y)
    );
}

void ImageWidget::onKey_F(wxKeyEvent& event)
{
    //Rescales image to its original dimensions
    wxChar key = event.GetUnicodeKey();
    if (key == 'F')
    {
        m_scale.x = m_originalDimensions.x;
        m_scale.y = m_originalDimensions.y;
        this->SetSize(wxSize(m_scale.x, m_scale.y));
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
    //GET PRE ZOOM POSITION
    mousePosPreZoom = event.GetPosition();
    sizeBeforeScale = m_scale;

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
        this->SetSize(wxSize(m_scale.x, m_scale.y));

    }
    mousePosPostZoom = event.GetPosition();
    sizeAfterScale = m_scale;

    wxPoint2DDouble blah = sizeBeforeScale / mousePosPreZoom;
    wxPoint2DDouble test = sizeAfterScale - sizeBeforeScale;

    m_offsetX = test.m_x / blah.m_x;
    m_offsetY = test.m_y / blah.m_y;

    //this->Move(wxPoint(m_offsetX, m_offsetY));
    wxPoint2DDouble screenMousePos = wxGetMousePosition();

    double TopLeftCorner_x = (screenMousePos.m_x - mousePosPreZoom.x) + m_offsetX;
    double TopLeftCorner_y = (screenMousePos.m_y - mousePosPreZoom.y) + m_offsetY;

    wxPoint2DDouble pos = this->GetPosition();

    this->Move((wxPoint(pos.m_x - (double)m_offsetX, pos.m_y - (double)m_offsetY)));



    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    wxPoint2DDouble posAfter = this->GetPosition();
    
    //wxLogStatus(
    //    " m_offsetX=" + wxString::Format(wxT("%lf"), m_offsetX) + ' ' +
    //    " m_offsetY=" + wxString::Format(wxT("%lf"), m_offsetY) + ' ' +
    //    " movedX=" + wxString::Format(wxT("%lf"), pos.m_x - posAfter.m_x) + ' ' +
    //    " screenMousePosX=" + wxString::Format(wxT("%lf"), screenMousePos.m_x) + ' ' +
    //    " screenMousePosY=" + wxString::Format(wxT("%lf"), screenMousePos.m_y)
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
    if(!event.AltDown())
        Raise();

    Bind(wxEVT_LEFT_UP, &ImageWidget::leftIsUp, this);
    Bind(wxEVT_MOTION, &ImageWidget::leftIsDragging, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::leftIsDragging(wxMouseEvent& event)
{
    if (m_widgetDragging)
    {
        moveWidget(0,0);
    }
    wxPoint2DDouble posAfter = this->GetPosition();

    wxLogStatus(
        " X=" + wxString::Format(wxT("%lf"), posAfter.m_x) + ' ' +
        " Y=" + wxString::Format(wxT("%lf"), posAfter.m_y)
    );
}

void ImageWidget::moveWidget(double offsetX, double offsetY)
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
    this->Move(m_parent->ScreenToClient(wxPoint(TopLeftCorner_x + (int)offsetX, TopLeftCorner_y + (int)offsetY)));

    //Need to do this otherwise dragging an ImageWidget leave artifacts
    //GetParent()->ClearBackground();
    Refresh();
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
