#include <wx/wx.h>
#include "ImageWidget.h"
#include "rCanvas.h"

//---------------------------------------------------------------------------
// ImageCanvas
//---------------------------------------------------------------------------

ImageCanvas::ImageCanvas(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id)
{
    this->SetBackgroundColour(wxColor(34, 35, 37));
    this->SetDoubleBuffered(true);

    SetScrollbars(1, 1, 2000, 1200, 0, 0);
    //SetScrollRate(5, 5);

    Bind(wxEVT_MOTION, &ImageCanvas::rightDragging, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::rightDown, this);
    Bind(wxEVT_RIGHT_UP, &ImageCanvas::rightUp, this);

    //Global key bindings
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::onKeyOpen, this);


}

ImageCanvas::~ImageCanvas(){}

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

void ImageCanvas::rightDown(wxMouseEvent& event) 
{
    startMousePos = event.GetPosition();
    panCanvas = true;
    CaptureMouse();

    //wxLogStatus(wxString::Format(wxT("%d"), rightDownPos.x) + ' ' + 
    //            wxString::Format(wxT("%d"), rightDownPos.y));
}

wxPoint ImageCanvas::processDistance(wxPoint current, wxPoint start)
{
    //Distance in pixels mouse has traveled from start of right click
    int mouseTravelX = (current.x - start.x);
    int mouseTravelY = (current.y - start.y);

    startMousePos.x = current.x;
    startMousePos.y = current.y;

    return wxPoint(mouseTravelX, mouseTravelY);
}

void ImageCanvas::procesCanvasPan(wxPoint inProgressMousePos, wxPoint startMousePos)
{
    wxSetCursor(wxCURSOR_CROSS);

    //Get position of scrollbar in scroll units
    int currentScrolPosX{};
    int currentScrolPosY{};
    GetViewStart(&currentScrolPosX, &currentScrolPosY);

    //Get num pixels per scroll unit
    //int scrollUnitPxlsX{};
    //int scrollUnitPxlsY{};
    //GetScrollPixelsPerUnit(&scrollUnitPxlsX, &scrollUnitPxlsY);

    //currentScrolPosX = currentScrolPosX / scrollUnitPxlsX;
    //currentScrolPosY = currentScrolPosY / scrollUnitPxlsY;

    //Distance in pixels mouse has traveled from start of right click
    int mouseTravelX = (inProgressMousePos.x - startMousePos.x);
    int mouseTravelY = (inProgressMousePos.y - startMousePos.y);

    int pixelToScrollUnitX = mouseTravelX;
    int pixelToScrollUnitY = mouseTravelY;

    this->Scroll(inProgressMousePos.x, inProgressMousePos.y);

    wxLogStatus("PixelsX = " + wxString::Format(wxT("%d"), inProgressMousePos.x) + ' ' +
                "PixelsY = " + wxString::Format(wxT("%d"), inProgressMousePos.y) + ' ' +
                "currentScrolPosX= " + wxString::Format(wxT("%d"), currentScrolPosX) + ' ' +
                "currentScrolPosY = " + wxString::Format(wxT("%d"), currentScrolPosY)
    );
}

void ImageCanvas::rightDragging(wxMouseEvent& event)
{
    int virtualSizeX{};
    int virtualSizeY{};
    GetVirtualSize(&virtualSizeX, &virtualSizeY);
    int currentScrolPosX{};
    int currentScrolPosY{};
    GetViewStart(&currentScrolPosX, &currentScrolPosY);

    //wxLogStatus("virtualSizeX" + wxString::Format(wxT("%d"), virtualSizeX) + ' ' +
    //            "virtualSizeY" + wxString::Format(wxT("%d"), virtualSizeY) + ' ' +
    //            "currentScrolPosX" + wxString::Format(wxT("%d"), currentScrolPosX) + ' ' +
    //            "currentScrolPosY" + wxString::Format(wxT("%d"), currentScrolPosY)
    //);


    wxPoint direction{};
    if (panCanvas && event.RightIsDown())
    {
        //Get position of scrollbar in scroll units
        int currentScrolPosX{};
        int currentScrolPosY{};
        GetViewStart(&currentScrolPosX, &currentScrolPosY);

        wxPoint inProgressMousePos = event.GetPosition();
        direction = processDistance(event.GetPosition(), startMousePos);
        //ImageCanvas::procesCanvasPan(inProgressMousePos, startMousePos);
        this->Scroll(currentScrolPosX += direction.x, currentScrolPosY += direction.y);
        wxLogStatus("directionX" + wxString::Format(wxT("%d"), direction.x) + ' ' +
                    "directionY" + wxString::Format(wxT("%d"), direction.y)
        );
    }
}

void ImageCanvas::rightUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }

    panCanvas = false;
}

void ImageCanvas::OnDraw(wxDC& dc){}

//---------------------------------------------------------------------------
// MainFrame
//---------------------------------------------------------------------------

MainFrame::MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
{
    CreateStatusBar();
}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a mainFrame and mainImageCanvas
    MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(1280,720));
    ImageCanvas* mainImageCanvas = new ImageCanvas(mainFrame, wxID_ANY);

    ImageWidget* imageWidget02 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(500, 500), "image2.jpg");
    ImageWidget* imageWidget01 = new ImageWidget(mainImageCanvas, wxID_ANY, wxPoint(200, 200), wxSize(250, 250), "image3.jpg");

    //Add panel to sizer, fit frame to sizer
    sizer->Add(mainImageCanvas, 1, wxEXPAND | wxALL, 3);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);
    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);
