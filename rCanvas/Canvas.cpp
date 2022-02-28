/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           Canvas.cpp
// Purpose:             Pan canvas/images, instance ImageWidgets
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "ImageWidget.h"
#include "Canvas.h"


class ImageWidget;

//---------------------------------------------------------------------------
// Constructor / Destructor
//---------------------------------------------------------------------------

ImageCanvas::ImageCanvas(wxWindow* parent, wxWindowID id, wxStatusBar& statusBar)
    : wxScrolledWindow(parent, id)
{
    m_statusBar = &statusBar;

    this->SetBackgroundColour(wxColor(37, 38, 39));
    this->SetDoubleBuffered(true);

    //Get users larger screen resolution (X or Y) and double it for canvas size 
    int resolution = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) > wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
        ? wxSystemSettings::GetMetric(wxSYS_SCREEN_X) : wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
    
    SetScrollbars(1, 1, resolution * 3, resolution * 3, 0, 0);
    //ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);

    GetVirtualSize(&m_virtualSize.x, &m_virtualSize.y);

    //Binding events
    Bind(wxEVT_MOTION, &ImageCanvas::RightIsDragging, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::RightIsDown, this);
    Bind(wxEVT_MOTION, &ImageCanvas::HoverPrinting, this);//Remove later
    //Bind(wxEVT_MOUSEWHEEL, &ImageCanvas::mouseScrollWheel, this);

    //Global key bindings
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_O, this);
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_A, this);

    m_statusBar->SetStatusText("Press F1 for help!");
}

ImageCanvas::~ImageCanvas() {}

wxString ImageCanvas::GetImage()
{
    wxFileDialog openFileDialog
    (this, _("Open Image"), "", "", 
"Any supported image (*.jpg; *.bmp; *.gif; *.png)|*.jpg; *.bmp; *.gif; *.png|\
.jpg files (*.jpg)|*.jpg|\
BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|\
PNG files (*.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return wxEmptyString;

    //Get path to image
    wxString fileLocation = (openFileDialog.GetPath());
    //wxString fileName = openFileDialog.GetFilename();

    return fileLocation;
}

wxPoint ImageCanvas::IncrimentScrollDirection(wxPoint current, wxPoint start, wxMouseEvent& event)
{
    //This should return a 1 or -1 in x and y when dragging
    //the mouse, the pan should feel like a 1:1 ratio for pixels
    //traveled on screen and pixels the canvas has moved/scrolled
    wxPoint mouseTravel{};

    //Get x/y direction mouse has traveled
    mouseTravel.x = (current.x - start.x);
    mouseTravel.y = (current.y - start.y);

    //Reset starting position so movement is 1:1
    m_startMousePos.x = current.x;
    m_startMousePos.y = current.y;

    //Speed up dragging if CTRL is down
    if (event.ControlDown())
        m_dragMultiplier = 4;
    else
        m_dragMultiplier = 1;

    //Returning negative causes canvas to drag with mouse direction
    return -mouseTravel * m_dragMultiplier;
}

void ImageCanvas::CenterScrollbars()
{
    wxPoint clientSize{};
    GetClientSize(&clientSize.x, &clientSize.y);
    Scroll(((m_virtualSize.x - clientSize.x) / 2), ((m_virtualSize.y - clientSize.y) / 2));
}

wxPoint ImageCanvas::GetMousePos()
{
    //Get screen mouse pos
    const wxPoint pt = wxGetMousePosition();

    //Subtract corner of top left window to get client/local pos
    wxPoint mPos{ pt.x - this->GetScreenPosition().x, pt.y - this->GetScreenPosition().y };

    return mPos;
}

//---------------------------------------------------------------------------
//Override functions
//---------------------------------------------------------------------------

bool ImageCanvas::ShouldScrollToChildOnFocus(wxWindow* child)
{
    //Overriding this member function because we dont want the canvas
    //to auto scroll to the currently focused child. if we dont override this 
    //when adding a ImageWidget while the curently focused child is off screen
    //the canvas will auto scroll to focus that ImageWidget THEN add the new
    //ImageWidget we created, which will cause it to be placed incorrectly on
    //the canvas.
    return false;
}

void ImageCanvas::OnDraw(wxDC& dc)
{
    Render(dc);
    //dc.DrawText(wxString("UNLOCKED"), wxPoint(0, 0));
}

void ImageCanvas::Render(wxDC& dc)
{
    //Draw subgrid
    dc.SetPen(wxPen(wxColor(41, 41, 41), 2));

    //Horizontal - middle and down -Y (0,1750),(3500,0)
    for (int i{ m_virtualSize.y / 2 }; i < m_virtualSize.y; i += m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(0 + m_border, i), wxPoint(m_virtualSize.x - m_border, i));
    }
    //Horizontal - middle and up +Y (0,1750),(3500,0)
    for (int i{ m_virtualSize.y / 2 }; i > 0; i -= m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(0 + m_border, i), wxPoint(m_virtualSize.x - m_border, i));
    }
    //Vertical - middle and right to +X (1750,0),(0,3500)
    for (int i{ m_virtualSize.x / 2 }; i < m_virtualSize.x; i += m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(i, 0 + m_border), wxPoint(i, m_virtualSize.y - m_border));
    }
    //Vertical - middle and left to -X (1750,0),(0,3500)
    for (int i{ m_virtualSize.x / 2 }; i > 0; i -= m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(i, 0 + m_border), wxPoint(i, m_virtualSize.y - m_border));
    }

    //Draw axis lines X and Y
    dc.SetPen(wxPen(wxColor(45, 45, 45), 4));
    dc.DrawLine(wxPoint(0 + m_border, m_virtualSize.y / 2), wxPoint(m_virtualSize.x - m_border, m_virtualSize.y / 2));//H
    dc.DrawLine(wxPoint(m_virtualSize.x / 2, 0 + m_border), wxPoint(m_virtualSize.x / 2, m_virtualSize.y - m_border));//V

    //Draw origin crosshair
    dc.SetPen(wxPen(wxColor(95, 55, 55), 2));
    dc.DrawLine(wxPoint((m_virtualSize.x / 2) - 30, m_virtualSize.y / 2), wxPoint((m_virtualSize.x / 2) + 30, m_virtualSize.y / 2)); //H
    dc.DrawLine(wxPoint(m_virtualSize.x / 2, (m_virtualSize.y / 2) - 30), wxPoint(m_virtualSize.x / 2, (m_virtualSize.y / 2) + 30));//V

    //Draw canvas boundry
    dc.SetPen(wxPen(wxColor(95, 55, 55), 3));
    dc.DrawLine(wxPoint(m_border, m_border), wxPoint(m_virtualSize.x - m_border, m_border)); //H-top
    dc.DrawLine(wxPoint(m_virtualSize.x - m_border, m_border), wxPoint(m_virtualSize.x - m_border, m_virtualSize.y - m_border)); //V-right
    dc.DrawLine(wxPoint(m_border, m_virtualSize.y - m_border), wxPoint(m_virtualSize.x - m_border, m_virtualSize.y - m_border)); //H-bottom
    dc.DrawLine(wxPoint(m_border, m_border), wxPoint(m_border, m_virtualSize.y - m_border)); //V-left
}

//---------------------------------------------------------------------------
//Shortcut key handlers
//---------------------------------------------------------------------------

void ImageCanvas::OnKey_A(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'A')
    {
        CenterScrollbars();
    }
    event.Skip();
}

void ImageCanvas::OnKey_O(wxKeyEvent& event)
{
    wxChar key = event.GetKeyCode();
    wxPoint mPos = GetMousePos();

    if (key == 'O')
    {
        wxString fileLocation = ImageCanvas::GetImage();

        if(fileLocation != wxEmptyString)
            ImageWidget* imageWidget = new ImageWidget(this, wxID_ANY, mPos, wxDefaultSize, fileLocation, m_panCanvas, *m_statusBar);
    }
    event.Skip();
}

//---------------------------------------------------------------------------
//Mouse handlers
//---------------------------------------------------------------------------

void ImageCanvas::HoverPrinting(wxMouseEvent& event)//Remove later
{
    //const wxPoint pt = event.GetPosition();
    //wxPoint clientSize{}; GetClientSize(&clientSize.x, &clientSize.y);
    //wxPoint scrolledPosition{}; GetViewStart(&scrolledPosition.x, &scrolledPosition.y);
    //wxPoint windowSize{}; GetSize(&windowSize.x, &windowSize.y);
    //wxPoint mPos = wxGetMousePosition();
    //wxPoint screenToClient = ScreenToClient(mPos);
    //wxPoint clientToScreen = ClientToScreen(mPos);
    //wxPoint mainScrnMPos = wxGetMousePosition();

    //wxLogStatus(
    //    " mPosX=" + wxString::Format(wxT("%d"), mPos.x) + ' ' +
    //    " mPosY=" + wxString::Format(wxT("%d"), mPos.y) + ' ' +
    //    " screenToClientX=" + wxString::Format(wxT("%d"), screenToClient.x) + ' ' +
    //    " screenToClientY=" + wxString::Format(wxT("%d"), screenToClient.y) + ' ' +
    //    " clientToScreenX=" + wxString::Format(wxT("%d"), clientToScreen.x) + ' ' +
    //    " clientToScreenY=" + wxString::Format(wxT("%d"), clientToScreen.y) + ' ' +
    //    " mainScrnMPos=" + wxString::Format(wxT("%d"), mainScrnMPos.x) + ' ' +
    //    " mainScrnMPos=" + wxString::Format(wxT("%d"), mainScrnMPos.y)
    //);
    event.Skip();
}

void ImageCanvas::RightIsDown(wxMouseEvent& event)
{
    m_startMousePos = event.GetPosition();
    m_panCanvas = true;
    CaptureMouse();

    Bind(wxEVT_LEAVE_WINDOW, &ImageCanvas::OnLeaveCanvasWindow, this);
    Bind(wxEVT_RIGHT_UP, &ImageCanvas::RightIsUp, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageCanvas::OnCaptureLost, this);
}

void ImageCanvas::RightIsDragging(wxMouseEvent& event)
{
    wxPoint direction{};
    if (m_panCanvas) {
        wxSetCursor(wxCURSOR_CROSS);

        //Get position of scrollbar in scroll units
        wxPoint scrolledPosition{};
        GetViewStart(&scrolledPosition.x, &scrolledPosition.y);

        wxPoint inProgressMousePos = event.GetPosition();
        direction = IncrimentScrollDirection(event.GetPosition(), m_startMousePos, event);
        this->Scroll(scrolledPosition.x += direction.x, scrolledPosition.y += direction.y);
        //ScreenToClient(wxPoint(mousePosConverted_x, mousePosConverted_y));
    }
}

void ImageCanvas::RightIsUp(wxMouseEvent& event)
{
    if (HasCapture()) {
        ReleaseMouse();
    }

    m_panCanvas = false;

    Unbind(wxEVT_LEAVE_WINDOW, &ImageCanvas::OnLeaveCanvasWindow, this);
    Unbind(wxEVT_RIGHT_UP, &ImageCanvas::RightIsUp, this);
    Unbind(wxEVT_MOUSE_CAPTURE_LOST, &ImageCanvas::OnCaptureLost, this);
}

void ImageCanvas::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture()) {
        ReleaseMouse();
    }
    m_panCanvas = false;
}

void ImageCanvas::OnLeaveCanvasWindow(wxMouseEvent& event)
{
    //By just having this event handler here, when panning and the mouse 
    //the leaves program window it eliminates jitters in the canvas movement
}

//void ImageCanvas::mouseScrollWheel(wxMouseEvent& event)
//{
//    wxLogStatus("mousescrolling");
//
//
//    //wxWindowList& children = GetChildren();
//    //for (wxWindowList::Node* node = children.GetFirst(); node; node = node->GetNext())
//    //{
//    //    ImageWidget* current = (ImageWidget*)node->GetData();
//    //    current->setGlobalScale();
//    //}
//
//
//    //List children of Canvas
//    //wxChar key = event.GetUnicodeKey();
//    //if (key == 'A')
//    // {
//        //wxWindowList children = GetChildren();
//        //for (auto itr{ children.begin() }; itr != children.end(); itr++)
//        //{
//        //    wxLogMessage(GetName());
//        //}
//     //}
//}
