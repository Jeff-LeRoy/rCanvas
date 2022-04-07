#pragma once
#include <wx/xml/xml.h>

class ImageCanvas : public wxScrolledWindow
{
private:
    //Member Variables
    int m_subgridPixelSpacing{ 350 };
    bool m_loadingSaveFile{false};
    int m_dragMultiplier{ 1 };
    wxStatusBar* m_statusBar;//From mainFrame
    wxPoint m_virtualSize{};
    wxPoint m_startMousePos;
    wxPoint m_viewStart{};
    int m_border{ 50 };
    wxXmlDocument doc;

    //Member Functions
    bool ShouldScrollToChildOnFocus(wxWindow* child);
    void ProcessSavefile(wxXmlNode* node);
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
    void OnSave(wxKeyEvent& event);
    void OnKey_A(wxKeyEvent& event);
    void OnKey_O(wxKeyEvent& event);
    void OnKey_C(wxKeyEvent& event);


public:
    ImageCanvas(wxWindow* parent, wxWindowID id, wxStatusBar& statusBar);
    ~ImageCanvas();

    //Setters
    void SettViewStart(wxPoint vs) { m_viewStart = vs; }

    void CenterScrollbars();
    bool m_panCanvas{ false };//MOVE after removing ImageWidget object from myApp
};

