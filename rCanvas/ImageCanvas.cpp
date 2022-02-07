#include <wx/wx.h>
#include "ImageWidget.h"
#include "ImageCanvas.h"

//---------------------------------------------------------------------------
// ImageCanvas
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

    GetVirtualSize(&m_virtualSize.x, &m_virtualSize.y);

    //Binding events
    Bind(wxEVT_MOTION, &ImageCanvas::rightIsDragging, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::rightIsDown, this);
    Bind(wxEVT_KEY_DOWN, &ImageCanvas::onKeyA, this);
    //Bind(wxEVT_MOUSEWHEEL, &ImageCanvas::mouseScrolling, this);

    //Global key bindings
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::onKeyOpen, this);
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

void ImageCanvas::onKeyOpen(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'O')
    {
        wxString fileLocation = ImageCanvas::getImage();

        wxLogStatus(fileLocation);
        ImageWidget* imageWidget = new ImageWidget(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, fileLocation);
    }
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
    /////////////////
    const wxPoint pt = event.GetPosition();

    wxPoint clientSize{}; GetClientSize(&clientSize.x, &clientSize.y);
    wxPoint scrolledPosition{}; GetViewStart(&scrolledPosition.x, &scrolledPosition.y);
    wxPoint windowSize{}; GetSize(&windowSize.x, &windowSize.y);

    wxLogStatus(" clientX=" + wxString::Format(wxT("%d"), clientSize.x) + ' ' +
        " clientY=" + wxString::Format(wxT("%d"), clientSize.y) + ' ' +
        " virtX=" + wxString::Format(wxT("%d"), m_virtualSize.x) + ' ' +
        " virtY=" + wxString::Format(wxT("%d"), m_virtualSize.y) + ' ' +
        " mousePos=" + wxString::Format(wxT("%d"), pt.x) + ' ' +
        " mousePos=" + wxString::Format(wxT("%d"), pt.y) + ' ' +
        " scrollPosX=" + wxString::Format(wxT("%d"), scrolledPosition.x) + ' ' +
        " scrollPosY=" + wxString::Format(wxT("%d"), scrolledPosition.y) + ' ' +
        " winSizeX=" + wxString::Format(wxT("%d"), windowSize.x) + ' ' +
        " winSizeY=" + wxString::Format(wxT("%d"), windowSize.y)
    );
    /////////////////

    wxPoint direction{};
    if (m_panCanvas) {
        wxSetCursor(wxCURSOR_CROSS);

        //Get position of scrollbar in scroll units
        wxPoint scrolledPosition{};
        GetViewStart(&scrolledPosition.x, &scrolledPosition.y);

        wxPoint inProgressMousePos = event.GetPosition();
        direction = incrimentScrollDirection(event.GetPosition(), m_startMousePos);
        this->Scroll(scrolledPosition.x += direction.x, scrolledPosition.y += direction.y);
        //ScreenToClient(wxPoint(mousePosConverted_x, mousePosConverted_y));
    }
}

wxPoint ImageCanvas::incrimentScrollDirection(wxPoint current, wxPoint start)
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

    //this->GetParent()->ScreenToClient(mouseTravel);

    return -mouseTravel;
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

void ImageCanvas::OnDraw(wxDC& dc)
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

void ImageCanvas::onCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture()) {
        ReleaseMouse();
    }

    m_panCanvas = false;
}

void ImageCanvas::onKeyA(wxKeyEvent& event)
{
    if (event.GetUnicodeKey() == 'A')
    {
        centerScrollbars();
    }
}

void ImageCanvas::onLeaveCanvasWindow(wxMouseEvent& event)
{
    //By just having this event handler here, when panning and the mouse 
    //the leaves program window it eliminates jitters in the canvas movement

    //wxLogStatus("Left the window!");
}

void ImageCanvas::centerScrollbars()
{
    wxPoint clientSize{};
    GetClientSize(&clientSize.x, &clientSize.y);
    Scroll(((m_virtualSize.x - clientSize.x) / 2), ((m_virtualSize.y - clientSize.y) / 2));
}

//void Canvas::mouseScrolling(wxMouseEvent& event){}
