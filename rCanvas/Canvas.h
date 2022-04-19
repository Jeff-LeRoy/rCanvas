#pragma once
#include <wx/xml/xml.h>
#include <wx/popupwin.h>
#include "CanvasPopup.h"

class ImageCanvas : public wxScrolledWindow
{
private:
    //Member Variables
    wxString m_canvasStatus{"No Canvas Loaded"};
    wxXmlDocument* m_XmlRcf = nullptr;
    wxPoint canvasMax{ 50000, 50000 };
    int m_subgridPixelSpacing{ 150 };
    wxPoint canvasMin{ 1000, 1000 };
    CanvasDialog* m_resizedialog{};
    bool m_loadingSaveFile{false};
    int m_dragMultiplier{ 1 };
    wxStatusBar* m_statusBar;//From mainFrame
    wxPoint m_virtualSize{};
    wxPoint m_startMousePos;
    wxPoint m_viewStart{};
    int m_border{ 50 };


    //Member Functions
    bool ShouldScrollToChildOnFocus(wxWindow* child);
    void LoadSavefile(wxXmlNode* node);
    void OnDraw(wxDC& dc) override;
    wxPoint GetClientMousePos();
    void Render(wxDC& dc);
    wxString GetImage();


    //Event Handlers
    wxPoint IncrimentScrollDirection(wxPoint pt, wxPoint start, wxMouseEvent& event);
    void OnCaptureLost(wxMouseCaptureLostEvent& event);
    void OnLeaveCanvasWindow(wxMouseEvent& event);
    //void mouseScrollWheel(wxMouseEvent& event);
    void RightIsDragging(wxMouseEvent& event);
    void HoverPrinting(wxMouseEvent& event);//Remove later
    void RightIsDown(wxMouseEvent& event);
    void RightIsUp(wxMouseEvent& event);
    void OnKey_Ctrl_S(wxKeyEvent& event);
    void OnKey_A(wxKeyEvent& event);
    void OnKey_O(wxKeyEvent& event);
    void OnKey_C(wxKeyEvent& event);
    void OnKey_X(wxKeyEvent& event);
    void OnKey_R(wxKeyEvent& event);


public:
    ImageCanvas(wxWindow* parent, wxWindowID id, wxStatusBar& statusBar);
    ~ImageCanvas();

    //Setters
    void SettViewStart(wxPoint vs) { m_viewStart = vs; }

    void CenterScrollbars();
    bool m_panCanvas{ false };//MOVE after removing ImageWidget object from myApp
};

