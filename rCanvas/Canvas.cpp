/////////////////////////////////////////////////////////////////////////////
// Application Name:    rCanvas    
// File Name:           Canvas.cpp
// Purpose:             Pan canvas/images, create ImageWidgets, save/load
// Author:              Jeffrey C. LeRoy
// Created:             01/17/2022
// Copyright:           (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#include <wx/xml/xml.h>
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
    ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);

    GetVirtualSize(&m_virtualSize.x, &m_virtualSize.y);

    //Binding events
    Bind(wxEVT_MOTION, &ImageCanvas::RightIsDragging, this);
    Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::RightIsDown, this);
    Bind(wxEVT_MOTION, &ImageCanvas::HoverPrinting, this);//Remove later
    //Bind(wxEVT_MOUSEWHEEL, &ImageCanvas::mouseScrollWheel, this);

    //Global key bindings
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_O, this);
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_A, this);
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_C, this);
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_Ctrl_S, this);
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::OnKey_X, this);


    m_statusBar->SetStatusText("Press F1 for help!", 0);
    m_statusBar->SetStatusText(canvasStatus, 1);
}

ImageCanvas::~ImageCanvas() 
{
    delete m_XmlRcf;
}

wxString ImageCanvas::GetImage()
{
    wxFileDialog openFileDialog
    (this, _("Open Image"), "", "", 
"Any supported image (*.jpg; *.bmp; *.gif; *.png)|*.jpg; *.bmp; *.gif; *.png|\
JPG files (*.jpg)|*.jpg|\
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

wxPoint ImageCanvas::GetClientMousePos()
{
    //Get screen mouse pos
    const wxPoint pt = wxGetMousePosition();

    //Subtract corner of top left window to get client/local pos
    wxPoint mPos{ pt.x - this->GetScreenPosition().x, pt.y - this->GetScreenPosition().y };

    return mPos;
}

void ImageCanvas::LoadSavefile(wxXmlNode* node)
{
    wxPoint position{};
    int currentScaleY{};
    wxString imgPath{};
    wxPoint scrolledPos = GetViewStart();
    m_loadingSaveFile = true;

    //Have to do this before initializing ImageWidgets otherwise they wont be 
    //positioned correctly on canvas
    Scroll(0, 0);

    while (node)
    {
        //If imagewidget then get children
        if (node->GetAttribute("type") == "ImageWidget")
            node = node->GetChildren();

        //Assign data
        if (node->GetName() == "positionX")
            position.x = wxAtoi(node->GetNodeContent());
        else if (node->GetName() == "positionY")
            position.y = wxAtoi(node->GetNodeContent());
        else if (node->GetName() == "imgPath")
            imgPath = node->GetNodeContent();
        else if(node->GetName() == "currentScaleY")
            currentScaleY = wxAtoi(node->GetNodeContent());

        //No more children so move up in heirarchy
        if (node->GetNext() == NULL)
        {
            ImageWidget* imageWidget = new ImageWidget(this, 
                wxID_ANY, 
                position, 
                wxDefaultSize, 
                imgPath, m_panCanvas, 
                *m_statusBar, 
                m_viewStart, 
                m_loadingSaveFile, 
                currentScaleY);

            node = node->GetParent();
        }

        node = node->GetNext();
    }

    //Reset to where user was
    Scroll(scrolledPos);

    m_loadingSaveFile = false;
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

    //wxStaticText* text = new wxStaticText(this, wxID_ANY, "TEST", wxPoint(25, 25), wxSize(25, 12));
    //dc.SetTextForeground(*wxWHITE);
    //dc.DrawText("TEST", m_viewStart + (m_clientSize - wxPoint(100, 100)));
    //dc.DrawText("TEST", m_viewStart + wxPoint(10, 10));
}

//---------------------------------------------------------------------------
//Mouse / Keyboard Handlers
//---------------------------------------------------------------------------

void ImageCanvas::OnKey_A(wxKeyEvent& event)
{
    wxChar key = event.GetKeyCode();
    wxPoint mPos = GetClientMousePos();

    if (key == 'A')
    {
        wxString fileLocation = GetImage();

        if (fileLocation != wxEmptyString)
            ImageWidget* imageWidget = new ImageWidget(this, 
                wxID_ANY, 
                mPos, 
                wxDefaultSize, 
                fileLocation, 
                m_panCanvas, 
                *m_statusBar, 
                m_viewStart, 
                m_loadingSaveFile, 
                1);
    }
    event.Skip();
}

void ImageCanvas::OnKey_O(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();

    if (key == 'O')
    {
        //Get info from FileDialog and then load saved file
        //---------------------------------------------------------------------------
        wxFileDialog openFileDialog(this, _("Open Canvas file"), "", "",
            "rCanvas files (*.rcf)|*.rcf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return;
        else
        {
            this->DestroyChildren();
        }

        //Get path to XML location
        wxString fileLocation = (openFileDialog.GetPath());

        canvasStatus = (openFileDialog.GetFilename());
        m_statusBar->SetStatusText(canvasStatus, 1);

        m_XmlRcf = new wxXmlDocument(fileLocation);

        //Handle loading XML
        if (!m_XmlRcf->Load(fileLocation))
            m_statusBar->SetStatusText("Failed To Load canvas.", 1);
        //else
        //    m_statusBar->SetStatusText("Successfully Loaded canvas", 1);

        //Make sure it is a rCanvas XML
        if (m_XmlRcf->GetRoot()->GetName() != "rCanvasRoot")
            wxLogMessage("Not an rCanvas file");
        else
        {
            wxXmlNode* rootChild = m_XmlRcf->GetRoot()->GetChildren();
            LoadSavefile(rootChild);
        }
    }
    event.Skip();
}

void ImageCanvas::OnKey_X(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();

    if (key == 'X')
    {
        wxCloseEvent* close = new wxCloseEvent(wxEVT_CLOSE_WINDOW);
        close->SetCanVeto(true);

        if (close->CanVeto())
        {
            if (wxMessageBox("Clear all ImageWidgets from Canvas ?",
                "Please confirm",
                wxICON_QUESTION | wxYES_NO) != wxYES)
            {
                close->Veto();
                return;
            }
        }
        this->DestroyChildren();
        canvasStatus = "No Canvas Loaded";
        m_statusBar->SetStatusText(canvasStatus, 1);
    }
    else
        event.Skip();
}

void ImageCanvas::OnKey_Ctrl_S(wxKeyEvent& event)
{
    wxChar key = event.GetKeyCode();
  
    if (key == 'S' && event.ControlDown())
    {
        wxPoint savePosition{};
        int saveImageID{0};
        wxPoint saveCurrentScale{};
        wxString saveImgPath{};

        //Save all images on Canvas to xml doc
        //---------------------------------------------------------------------------

        wxXmlDocument* xmlSaveDoc = new wxXmlDocument();
        wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "rCanvasRoot");
        xmlSaveDoc->SetRoot(root);

        wxWindowList& children = GetChildren();
        for (wxWindowList::Node* node{ children.GetFirst() }; node; node = node->GetNext())
        {
            ImageWidget* current = (ImageWidget*)node->GetData();
            saveImgPath = current->GetImgPath();
            savePosition = current->GetPositionOnCanvas();
            saveCurrentScale = current->GetCurrentScale();

            wxXmlNode* currentImage = new wxXmlNode(root, wxXML_ELEMENT_NODE, "image_" + wxString::Format(wxT("%d"), saveImageID));
            currentImage->AddAttribute("type", "ImageWidget");

            wxXmlNode* imgPath = new wxXmlNode(currentImage, wxXML_ELEMENT_NODE, "imgPath");
            imgPath->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "", saveImgPath));

            wxXmlNode* currentScaleY = new wxXmlNode(currentImage, wxXML_ELEMENT_NODE, "currentScaleY");
            currentScaleY->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "", wxString::Format(wxT("%d"), saveCurrentScale.y)));

            wxXmlNode* positionY = new wxXmlNode(currentImage, wxXML_ELEMENT_NODE, "positionY");
            positionY->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "", wxString::Format(wxT("%d"), savePosition.y)));
            wxXmlNode* positionX = new wxXmlNode(currentImage, wxXML_ELEMENT_NODE, "positionX");
            positionX->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "", wxString::Format(wxT("%d"), savePosition.x)));

            saveImageID++;
        }

        //Get path and save file name from FileDialog then save
        //---------------------------------------------------------------------------

        wxFileDialog
            saveFileDialog(this, _("Save rCanvas file"), "", "",
                "RCF files (*.rcf)|*.rcf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (saveFileDialog.ShowModal() == wxID_CANCEL)
            return;     // the user changed idea...

        wxString savePath = saveFileDialog.GetPath();
        wxString saveName = saveFileDialog.GetName();
        wxString fileName = saveFileDialog.GetFilename();

        //Save the file to user PC
        xmlSaveDoc->Save(savePath + saveName);
        //xmlDoc.Save("testSaveFile.rcf");

        //Set statusBar
        canvasStatus = fileName;
        m_statusBar->SetStatusText(canvasStatus, 1);

        delete xmlSaveDoc;
    }
    event.Skip();
}

void ImageCanvas::OnKey_C(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'C')
    {
        CenterScrollbars();

        //needs to be updated so we can calculate where on Canvas images are when saving/loading
        m_viewStart = GetViewStart();
    }
    event.Skip();
}

void ImageCanvas::HoverPrinting(wxMouseEvent& event)//Remove later
{
    //const wxPoint pt = event.GetPosition();
    //wxPoint clientSize{}; GetClientSize(&clientSize.x, &clientSize.y);
    //wxPoint scrolledPosition{}; GetViewStart(&scrolledPosition.x, &scrolledPosition.y);
    //wxPoint windowSize{}; GetSize(&windowSize.x, &windowSize.y);
    //wxPoint mPos = GetClientMousePos();
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
    //    " mainScrnMPos=" + wxString::Format(wxT("%d"), mainScrnMPos.y) + ' ' +
    //    " vsX=" + wxString::Format(wxT("%d"), m_viewStart.x) + ' ' +
    //    " vsY=" + wxString::Format(wxT("%d"), m_viewStart.y)
    //);

     //wxLogStatus(
     //    " m_viewStartX=" + wxString::Format(wxT("%d"), m_viewStart.x) + ' ' +
     //    " m_viewStartY=" + wxString::Format(wxT("%d"), m_viewStart.y) + ' ' +
     //    " m_clientSizeX=" + wxString::Format(wxT("%d"), m_clientSize.x) + ' ' +
     //    " m_clientSizeY=" + wxString::Format(wxT("%d"), m_clientSize.y)

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

        //needs to be updated so we can calculate where on Canvas images are when saving/loading
        m_viewStart = GetViewStart();
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
