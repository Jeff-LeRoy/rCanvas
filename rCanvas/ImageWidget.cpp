/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           ImageWidget.cpp
// Purpose:             Handle transforming and manipulating images on canvas
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// Constructor / Destructor
//---------------------------------------------------------------------------

ImageWidget::ImageWidget(wxWindow* parent, 
                        wxWindowID id, 
                        wxPoint pos, 
                        wxSize size, 
                        wxString imgPath, 
                        const bool& m_panCanvas, //Cant zoom when panning
                        wxStatusBar& statusBar,
                        const wxPoint& viewStart, //Calculat pos on Canvas
                        const bool& saving, //Know if instantiating from save file
                        int imageHeight) //Resize from save file
    :wxPanel(parent, id, pos, size)
{
    this->SetBackgroundColour(wxColor(77, 38, 39));

    //Get info from Canvas
    m_imgPath = imgPath;
    m_isCanvasPanning = &m_panCanvas;
    m_statusBar = &statusBar;
    m_viewStart = &viewStart;
    m_loadingSaveFile = &saving;

    //load image to heap
    m_bitmap = new wxBitmap(imgPath, wxBITMAP_TYPE_ANY);
    if(!m_bitmap->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    //Store original dimensions of image
    m_originalDimensions.x = m_bitmap->GetWidth();
    m_originalDimensions.y = m_bitmap->GetHeight();

    //If image is larger than client Y, scale it
    wxPoint clientSize{};
    GetParent()->GetClientSize(&clientSize.x, &clientSize.y);
    if (!*m_loadingSaveFile)
    {
        if (m_bitmap->GetHeight() >= clientSize.y)
            RescaleImage(m_bitmap, clientSize.y);

        m_scale.x = m_bitmap->GetWidth();
        m_scale.y = m_bitmap->GetHeight();
    }
    //Loading from a save file need to account for pixels removed in CalculateAspectRatio
    else
        RescaleImage(m_bitmap, imageHeight + 20);

    //Move center of ImageWidget to cursor 
    //this->Move(wxPoint(pos.x - (m_scale.x / 2), pos.y - (m_scale.y / 2)));

    //Set size of widget (wxPanel)
    this->SetSize(wxSize(m_scale.x, m_scale.y));

    Raise();

    m_timer.SetOwner(this);

    //Bind Mouse Events
    Bind(wxEVT_MOUSEWHEEL, &ImageWidget::ScrollWheelZoom, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageWidget::RightIsDown, this);
    Bind(wxEVT_LEFT_DOWN, &ImageWidget::LeftIsDown, this);
    Bind(wxEVT_ENTER_WINDOW, &ImageWidget::EnterWindow, this);
    Bind(wxEVT_LEAVE_WINDOW, &ImageWidget::ExitWindow, this);
    Bind(wxEVT_MOTION, &ImageWidget::HoverPrinting, this);//Remove later
    Bind(wxEVT_PAINT, &ImageWidget::OnPaint, this);
    Bind(wxEVT_TIMER, &ImageWidget::OnTimer, this);
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
        //wxLogStatus("Drawing wxImage");
        RenderScaled(dc);
    }
    else
    {
        //wxLogStatus("Drawing Bitmap");
        Render(dc);
    }
        
}

void ImageWidget::Render(wxDC& dc)
{
    dc.DrawBitmap(*m_bitmap, 0, 0, true);
}

void ImageWidget::RenderScaled(wxDC& dc)
{

    //Convert bitmap to wxImage for scaling
    m_image = new wxImage();
    m_image->LoadFile(m_imgPath, wxBITMAP_TYPE_ANY);

    //Convert wxImage to wxBitmap for drawing
    *m_bitmap = wxBitmap(m_image->Scale(m_scale.x, m_scale.y));
    delete m_image;

    dc.DrawBitmap(*m_bitmap, 0, 0, true);
}

void ImageWidget::CalculateAspectRatio()
{
    //Aspect ratio formula -> (org. height / org. width) x new width = new height
    //Solve for width - width / (original width / original Height);
    //Solve for height - height * (original width / original Height);

    wxPoint oldScale = m_scale;
    m_scale.x = oldScale.x * (m_scaleIncrimentor / 100.0);
    
    //Limiting scale of image
    if (m_scale.x < 32.0)
        m_scale.x = 32.0;
    else if (m_scale.x > m_originalDimensions.x * 4)
        m_scale.x = m_originalDimensions.x * 4;

    //Calculate new height
    //adding 0.5 causes the integer truncation to produce a rounding effect
    m_scale.y = ((double)m_scale.x * ((double)m_originalDimensions.y / (double)m_originalDimensions.x) + 0.5);

    //currentAspectRatio = ((double)m_scale.y / m_scale.x);
}

void ImageWidget::CalculateAspectRatio(int clientSizeY)
{
    //Solve width while having new height
    m_scale.y = (double)clientSizeY - 20;

    double originalWidth = m_bitmap->GetWidth();
    double originalHeight = m_bitmap->GetHeight();
    m_scale.x = ((double)m_scale.y * ((double)originalWidth / (double)originalHeight) + 0.5);

    //currentAspectRatio = ((double)m_scale.y / m_scale.x);
}

void ImageWidget::RescaleImage(wxBitmap* bitmap, int max)
{
    //Convert bitmap to wxImage for scaling
    m_image = new wxImage();
    m_image->LoadFile(m_imgPath, wxBITMAP_TYPE_ANY);

    CalculateAspectRatio(max);

    //Convert wxImage to wxBitmap for drawing
    *m_bitmap = wxBitmap(m_image->Scale(m_scale.x, m_scale.y));
    delete m_image;
}

void ImageWidget::SetGlobalScale()
{
    //baseOffset + (pivot * height) = finalPos
    m_scale.x = 256;
    m_scale.y = 256;
    this->SetSize(wxSize(m_scale.x, m_scale.y));
}

void ImageWidget::ZoomToCursor(wxPoint& mousePos, bool scalingUp,
    wxPoint2DDouble sizeAfterScale, wxPoint2DDouble sizeBeforeScale)
{
    //This will "round" the integer since they are truncated
    //Doing this because you may have a situation where it calls to move,
    //8.7 pixels, then 8.8, and 8.9 these will all be truncated to 8
    //BUT it will be more accurate if rounded to 9, round down when zooming out
    double rounding;

    rounding = (scalingUp) ? +0.5 : -0.5;

    //Get 0.0-1.0 decimal coordinates inside ImageWidget for X and Y
    //If the image is 512x512 and the mouse position is (256,256) this will give you (.5,.5)
    //aka we're at the middle of the image
    //0 will be far left or top of image, 1 is far right or bottom
    wxPoint2DDouble decCoordinates = mousePos / sizeBeforeScale;

    //Get how many pixels image size has changed
    wxPoint2DDouble changeInSize = sizeAfterScale - sizeBeforeScale;

    //Now we need to calculate how much to offset the image based on mousePos
    //if old size was 512,512 and new size is 528x528 the change in size is 16
    //so, 16 * .5 = 8, same for y because image is square, now we know to move the image 
    //up and to the left 8 pixels each
    int m_offsetX = (changeInSize.m_x * decCoordinates.m_x) + rounding;
    int m_offsetY = (changeInSize.m_y * decCoordinates.m_y) + rounding;

    wxPoint2DDouble pos = this->GetPosition();
    this->Move((wxPoint(pos.m_x - m_offsetX, pos.m_y - m_offsetY)));
}

wxPoint ImageWidget::CalcPositionOnCanvas()
{
    //This will return the position of the ImageWidget on the canvas
    //0,0 is top left corner
    return *m_viewStart + GetPosition();
}

//---------------------------------------------------------------------------
// Mouse / Keyboard Handlers
//---------------------------------------------------------------------------

void ImageWidget::HoverPrinting(wxMouseEvent& event)//Remove later
{
//    m_statusBar->SetStatusText("Right click + drag to pan canvas \
//| Left click + drag to move image \
//| F - Restore original image size");

    //wxPoint pos = event.GetPosition();
    //int x = pos.x;
    //int y = pos.y;
    ////wxPoint scrn = event.GetPosition();
    //wxPoint scrn = m_parent->ScreenToClient(wxPoint(x, y));
    //wxPoint client = m_parent->ClientToScreen(wxPoint(x, y));

    //----------------------------------------
    //wxPoint iWPos = GetPosition();
    ////wxPoint viewStart = GetViewStart();
    //

    //wxLogStatus(
    //    " posX" + wxString::Format(wxT("%d"), m_viewStart->x) + ' ' +
    //    " posY" + wxString::Format(wxT("%d"), m_viewStart->y)
    //);
}

void ImageWidget::OnKey_F(wxKeyEvent& event)
{
    //Pass to zoom
    wxPoint2DDouble sizeBeforeScale = m_scale;
    wxPoint mousePos = event.GetPosition();

    //Rescales image to its original dimensions
    wxChar key = event.GetUnicodeKey();
    if (key == 'F' && m_scale.x != m_originalDimensions.x)
    {
        m_scale.x = m_originalDimensions.x;
        m_scale.y = m_originalDimensions.y;
        this->SetSize(wxSize(m_scale.x, m_scale.y));
        m_scalingImage = true;
        Refresh();
    }

    bool scalingUp;
    (sizeBeforeScale.m_x < m_scale.x) ? scalingUp = true : scalingUp = false;

    ZoomToCursor(mousePos, scalingUp, m_scale, sizeBeforeScale);

    event.Skip();
}

void ImageWidget::OnKey_D(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'D' && m_canDelete)
    {
        wxCloseEvent* close = new wxCloseEvent(wxEVT_CLOSE_WINDOW);
        close->SetCanVeto(true);

        if (close->CanVeto())
        {
            if (wxMessageBox("Delete this ImageWidget ?",
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

void ImageWidget::ScrollWheelZoom(wxMouseEvent& event)
{
    //This will auto turn m_scalingImage to false
    m_timer.Start(1000);

    wxPoint mousePosPreZoom = event.GetPosition();
    wxPoint2DDouble sizeBeforeScale = m_scale;

    int rot = event.GetWheelRotation();
    int delta = event.GetWheelDelta();

    if (!m_widgetDragging && !*m_isCanvasPanning)
    {
        m_scalingImage = true;

        //Speed up scaling if CTRL is down
        if (event.ControlDown())
            m_scaleMultiplier = 8;
        else
            m_scaleMultiplier = 2;

        //incriment scale
        m_scaleIncrimentor = 100;
        m_scaleIncrimentor += (double)m_scaleMultiplier * (rot / delta);

        CalculateAspectRatio();

        //Set size of ImageWidget wxPanel
        this->SetSize(wxSize(m_scale.x, m_scale.y));
    }

    wxPoint2DDouble sizeAfterScale = m_scale;
    
    bool scalingUp;
    if (rot > 1)
        scalingUp = true;
    else
        scalingUp = false;;
    
    ZoomToCursor(mousePosPreZoom, scalingUp, m_scale, sizeBeforeScale);

    //wxLogStatus(
    //    " rot=" + wxString::Format(wxT("%d"), rot) 
    //    //" MoveY=" + wxString::Format(wxT("%lf"), posBefore.m_y - posAfter.m_y) + ' ' +
    //    //" decCoordinates=" + wxString::Format(wxT("%lf"), decCoordinates.m_x) + ' ' +
    //    //" decCoordinates=" + wxString::Format(wxT("%lf"), decCoordinates.m_y) + ' ' +
    //    //" m_offsetX=" + wxString::Format(wxT("%d"), m_offsetX) + ' ' +
    //    //" m_offsetY=" + wxString::Format(wxT("%d"), m_offsetY) + ' ' +
    //    //" m_scaleIncrimentor=" + wxString::Format(wxT("%lf"), m_scaleIncrimentor)
    //);
}

void ImageWidget::LeftIsDown(wxMouseEvent& event)
{
    m_scalingImage = false;

    CaptureMouse();
    wxSetCursor(wxCURSOR_HAND);

    //Get client mouse position
    m_imageWidgetClickPos.x = event.GetX();
    m_imageWidgetClickPos.y = event.GetY();
    m_widgetDragging = true;

    //Set Z order to top
    if(!event.AltDown())
        Raise();

    Bind(wxEVT_LEFT_UP, &ImageWidget::LeftIsUp, this);
    Bind(wxEVT_MOTION, &ImageWidget::LeftIsDragging, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::LeftIsDragging(wxMouseEvent& event)
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
        Refresh();
    }
}

void ImageWidget::LeftIsUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }

    m_widgetDragging = false;

    Unbind(wxEVT_LEFT_UP, &ImageWidget::LeftIsUp, this);
    Unbind(wxEVT_MOTION, &ImageWidget::LeftIsDragging, this);
    Unbind(wxEVT_MOUSE_CAPTURE_LOST, &ImageWidget::OnCaptureLost, this);
}

void ImageWidget::EnterWindow(wxMouseEvent& event)
{
    SetFocus();
    Bind(wxEVT_CHAR_HOOK, &ImageWidget::OnKey_F, this);
    Bind(wxEVT_CHAR_HOOK, &ImageWidget::OnKey_D, this);
    m_canDelete = true;
}

void ImageWidget::ExitWindow(wxMouseEvent& event)
{
    m_scalingImage = false;

    Unbind(wxEVT_CHAR_HOOK, &ImageWidget::OnKey_F, this);
    Unbind(wxEVT_CHAR_HOOK, &ImageWidget::OnKey_D, this);
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

void ImageWidget::RightIsDown(wxMouseEvent& event)
{
    m_scalingImage = false;

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

void ImageWidget::OnTimer(wxTimerEvent& event)
{
    m_scalingImage = false;
}
