#pragma once

class ImageCanvas : public wxScrolledWindow
{
private:
    //wxVector<ImageWidget> vecImageWidgets{};
    int m_subgridPixelSpacing{ 200 };
    bool m_panCanvas{ false };
    wxPoint m_virtualSize{};
    wxPoint m_startMousePos;

    wxPoint incrimentScrollDirection(wxPoint pt, wxPoint start);
    void onCaptureLost(wxMouseCaptureLostEvent& event);
    bool ShouldScrollToChildOnFocus(wxWindow* child);
    void onLeaveCanvasWindow(wxMouseEvent& event);
    void rightIsDragging(wxMouseEvent& event);
    void rightIsDown(wxMouseEvent& event);
    void rightIsUp(wxMouseEvent& event);
    void onKey_O(wxKeyEvent& event);
    void onKey_A(wxKeyEvent& event);
    void OnDraw(wxDC& dc) override;
    wxPoint getMousePos();
    wxString getImage();

public:
    ImageCanvas(wxWindow* parent, wxWindowID id);
    ~ImageCanvas();

    void centerScrollbars();
    wxPoint getCanvasCenter();
};

