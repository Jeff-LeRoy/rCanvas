/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           Canvas.cpp
// Purpose:             Panning canvas/images, instantiate ImageWidgets
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "ImageWidget.h"
#include "Canvas.h"

//---------------------------------------------------------------------------
// Constructor / Destructor
//---------------------------------------------------------------------------

ImageCanvas::ImageCanvas(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id)
{
    this->SetBackgroundColour(wxColor(37, 38, 39));
    this->SetDoubleBuffered(true);

    //Get users larger screen resolution (X or Y) and double it for canvas size 
    int resolution = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) > wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
        ? wxSystemSettings::GetMetric(wxSYS_SCREEN_X) : wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
    
    SetScrollbars(1, 1, resolution * 2, resolution * 2, 0, 0);
    ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);

    GetVirtualSize(&m_virtualSize.x, &m_virtualSize.y);

    //Binding events
    Bind(wxEVT_MOTION, &ImageCanvas::rightIsDragging, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::rightIsDown, this);
    Bind(wxEVT_MOTION, &ImageCanvas::hoverPrinting, this);//Remove later
    //Bind(wxEVT_MOUSEWHEEL, &ImageCanvas::mouseScrollWheel, this);

    //Global key bindings
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::onKey_O, this);
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::onKey_A, this);
}

ImageCanvas::~ImageCanvas() {}

wxString ImageCanvas::getImage()
{
    wxFileDialog openFileDialog
    (this, _("Open Image"), "", "", ".jpg files (*.jpg)|*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return wxEmptyString;

    //Get path to image
    wxString fileLocation = (openFileDialog.GetPath());
    //wxString fileName = openFileDialog.GetFilename();

    return fileLocation;
}

wxPoint ImageCanvas::incrimentScrollDirection(wxPoint current, wxPoint start, wxMouseEvent& event)
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

void ImageCanvas::centerScrollbars()
{
    wxPoint clientSize{};
    GetClientSize(&clientSize.x, &clientSize.y);
    Scroll(((m_virtualSize.x - clientSize.x) / 2), ((m_virtualSize.y - clientSize.y) / 2));
}

wxPoint ImageCanvas::getMousePos()
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
    render(dc);
}

void ImageCanvas::render(wxDC& dc)
{
    //Draw subgrid
    dc.SetPen(wxPen(wxColor(41, 41, 41), 2));

    //Horizontal - middle and down -Y (0,1750),(3500,0)
    for (int i{ m_virtualSize.y / 2 }; i < m_virtualSize.y; i += m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(0, i), wxPoint(m_virtualSize.x, i));
    }
    //Horizontal - middle and up +Y (0,1750),(3500,0)
    for (int i{ m_virtualSize.y / 2 }; i > 0; i -= m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(0, i), wxPoint(m_virtualSize.x, i));
    }
    //Vertical - middle and right to +X (1750,0),(0,3500)
    for (int i{ m_virtualSize.x / 2 }; i < m_virtualSize.x; i += m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(i, 0), wxPoint(i, m_virtualSize.y));
    }
    //Vertical - middle and left to -X (1750,0),(0,3500)
    for (int i{ m_virtualSize.x / 2 }; i > 0; i -= m_subgridPixelSpacing) {
        dc.DrawLine(wxPoint(i, 0), wxPoint(i, m_virtualSize.y));
    }

    //Draw axis lines X and Y
    dc.SetPen(wxPen(wxColor(45, 45, 45), 4));//45
    dc.DrawLine(wxPoint(0, m_virtualSize.y / 2), wxPoint(m_virtualSize.x, m_virtualSize.y / 2));//H
    dc.DrawLine(wxPoint(m_virtualSize.x / 2, 0), wxPoint(m_virtualSize.x / 2, m_virtualSize.y));//V

    //Draw origin crosshair
    dc.SetPen(wxPen(wxColor(95, 55, 55), 2));//45
    dc.DrawLine(wxPoint((m_virtualSize.x / 2) - 30, m_virtualSize.y / 2), wxPoint((m_virtualSize.x / 2) + 30, m_virtualSize.y / 2)); //H
    dc.DrawLine(wxPoint(m_virtualSize.x / 2, (m_virtualSize.y / 2) - 30), wxPoint(m_virtualSize.x / 2, (m_virtualSize.y / 2) + 30));//V
}

//---------------------------------------------------------------------------
//Shortcut key handlers
//---------------------------------------------------------------------------

void ImageCanvas::onKey_A(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'A')
    {
        centerScrollbars();
    }
    event.Skip();
}

void ImageCanvas::onKey_O(wxKeyEvent& event)
{
    wxChar key = event.GetKeyCode();
    wxPoint mPos = getMousePos();

    if (key == 'O')
    {
        wxString fileLocation = ImageCanvas::getImage();

        ImageWidget* imageWidget = new ImageWidget(this, wxID_ANY, mPos, wxDefaultSize, fileLocation, m_panCanvas);
    }
    event.Skip();
}

//---------------------------------------------------------------------------
//Mouse handlers
//---------------------------------------------------------------------------

void ImageCanvas::hoverPrinting(wxMouseEvent& event)//Remove later
{
    //const wxPoint pt = event.GetPosition();
    //wxPoint clientSize{}; GetClientSize(&clientSize.x, &clientSize.y);
    //wxPoint scrolledPosition{}; GetViewStart(&scrolledPosition.x, &scrolledPosition.y);
    //wxPoint windowSize{}; GetSize(&windowSize.x, &windowSize.y);
    //wxPoint mPos = getMousePos();
    //wxPoint screenToClient = ScreenToClient(mPos);
    //wxPoint clientToScreen = ClientToScreen(mPos);
    //wxPoint mainScrnMPos = wxGetMousePosition();

    //wxLogStatus(" clientX=" + wxString::Format(wxT("%d"), clientSize.x) + ' ' +
    //            " clientY=" + wxString::Format(wxT("%d"), clientSize.y) + ' ' +
    //            " virtX=" + wxString::Format(wxT("%d"), m_virtualSize.x) + ' ' +
    //            " virtY=" + wxString::Format(wxT("%d"), m_virtualSize.y) + ' ' +
    //            /*" evtMPosX=" + wxString::Format(wxT("%d"), pt.x) + ' ' +
    //            " evtMPosY=" + wxString::Format(wxT("%d"), pt.y) + ' ' +
    //            " scrollPosX=" + wxString::Format(wxT("%d"), scrolledPosition.x) + ' ' +
    //            " scrollPosY=" + wxString::Format(wxT("%d"), scrolledPosition.y) + ' ' +*/
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

void ImageCanvas::rightIsDown(wxMouseEvent& event)
{
    m_startMousePos = event.GetPosition();
    m_panCanvas = true;
    CaptureMouse();

    Bind(wxEVT_LEAVE_WINDOW, &ImageCanvas::onLeaveCanvasWindow, this);
    Bind(wxEVT_RIGHT_UP, &ImageCanvas::rightIsUp, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &ImageCanvas::onCaptureLost, this);
}

void ImageCanvas::rightIsDragging(wxMouseEvent& event)
{
    wxPoint direction{};
    if (m_panCanvas) {
        wxSetCursor(wxCURSOR_CROSS);

        //Get position of scrollbar in scroll units
        wxPoint scrolledPosition{};
        GetViewStart(&scrolledPosition.x, &scrolledPosition.y);

        wxPoint inProgressMousePos = event.GetPosition();
        direction = incrimentScrollDirection(event.GetPosition(), m_startMousePos, event);
        this->Scroll(scrolledPosition.x += direction.x, scrolledPosition.y += direction.y);
        //ScreenToClient(wxPoint(mousePosConverted_x, mousePosConverted_y));
    }
}

void ImageCanvas::rightIsUp(wxMouseEvent& event)
{
    if (HasCapture()) {
        ReleaseMouse();
    }

    m_panCanvas = false;

    Unbind(wxEVT_LEAVE_WINDOW, &ImageCanvas::onLeaveCanvasWindow, this);
    Unbind(wxEVT_RIGHT_UP, &ImageCanvas::rightIsUp, this);
    Unbind(wxEVT_MOUSE_CAPTURE_LOST, &ImageCanvas::onCaptureLost, this);
}

void ImageCanvas::onCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture()) {
        ReleaseMouse();
    }
    m_panCanvas = false;
}

void ImageCanvas::onLeaveCanvasWindow(wxMouseEvent& event)
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
