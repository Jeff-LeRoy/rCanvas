#pragma once

class ImageCanvas : public wxScrolledWindow
{
private:
    //Member Variables
    int m_subgridPixelSpacing{ 350 };
    int m_dragMultiplier{ 1 };
    wxStatusBar* m_statusBar;//From mainFrame
    wxPoint m_virtualSize{};
    wxPoint m_startMousePos;
    int m_border{ 50 };

    //Member Functions
    bool ShouldScrollToChildOnFocus(wxWindow* child);
    void OnDraw(wxDC& dc) override;
    void Render(wxDC& dc);
    wxPoint GetMousePos();
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
    void OnKey_A(wxKeyEvent& event);
    void OnKey_O(wxKeyEvent& event);

public:
    ImageCanvas(wxWindow* parent, wxWindowID id, wxStatusBar& statusBar);
    ~ImageCanvas();

    void CenterScrollbars();
    bool m_panCanvas{ false };//MOVE after removing ImageWidget object from myApp
};

