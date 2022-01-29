#include <wx/wx.h>
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// ImageWidget
//---------------------------------------------------------------------------

//Need to catch capture lost event

ImageWidgetX::ImageWidgetX(wxWindow* parent, wxWindowID id, const wxPoint pos, wxSize size, wxString imgPath)
    :wxPanel(parent, id, pos, size)
{
    //ImageWidget::m_IwParent = parent;

    //load image on heap
    m_IwImage = new wxBitmap(imgPath, wxBITMAP_TYPE_JPEG);
    //m_IwImgWidth = m_IwImage->GetWidth();
    //m_IwImgHeight = m_IwImage->GetHeight();

    if (!m_IwImage->IsOk())
    {
        wxMessageBox("There was an error loading the image.");
        return;
    }

    this->SetBackgroundColour(wxColor(15, 68, 125));

    Bind(wxEVT_LEFT_DOWN, &ImageWidgetX::leftDown, this);
    Bind(wxEVT_LEFT_UP, &ImageWidgetX::leftUp, this);
    Bind(wxEVT_MOTION, &ImageWidgetX::mouseMoving, this);
    Bind(wxEVT_PAINT, &ImageWidgetX::OnPaint, this);

}

ImageWidgetX::~ImageWidgetX()
{
    delete m_IwImage;
}

void ImageWidgetX::leftDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxSetCursor(wxCURSOR_HAND);

    //Get local/client mouse position
    m_IwMouseLocal_x = event.GetX();
    m_IwMouseLocal_y = event.GetY();

    m_IwMouseDragging = true;

    //Set Z order to top
    Raise();
}

void ImageWidgetX::leftUp(wxMouseEvent& event)
{
    ReleaseMouse();
    m_IwMouseDragging = false;
    //SetFocus(0);
}

void ImageWidgetX::mouseMoving(wxMouseEvent& event)
{
    if (m_IwMouseDragging)
    {
        //Get screen space mouse pos
        wxPoint mouseScreen = wxGetMousePosition();

        //Subtract local mouse pos from screen pos
        int mousePosConverted_x = mouseScreen.x - m_IwMouseLocal_x;
        int mousePosConverted_y = mouseScreen.y - m_IwMouseLocal_y;

        //Move box to converted screen position
        //m_parent from window.h
        this->Move(m_parent->ScreenToClient(wxPoint(mousePosConverted_x, mousePosConverted_y)));

        //Need to do this otherwise dragging an ImageWidget leave artifacts
        GetParent()->ClearBackground();

        //wxLogStatus("mouseScreen X=" + wxString::Format(wxT("%d"), mouseScreen.x) + " " +
        //            "mouseScreen Y=" + wxString::Format(wxT("%d"), mouseScreen.y) + " " +
        //            "L X=" + wxString::Format(wxT("%d"), mouseLocal_x) + " " +
        //            "L Y=" + wxString::Format(wxT("%d"), mouseLocal_y)
        //            );
    }
}

void ImageWidgetX::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*m_IwImage, 0, 0, false);
    //dc.DrawLine(wxPoint(10, 10), wxPoint(100, 100));
}
