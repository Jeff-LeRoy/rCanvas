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
    void onLeaveCanvasWindow(wxMouseEvent& event);
    void rightIsDragging(wxMouseEvent& event);
    void rightIsDown(wxMouseEvent& event);
    void rightIsUp(wxMouseEvent& event);
    void onKeyOpen(wxKeyEvent& event);
    void onKeyA(wxKeyEvent& event);
    void OnDraw(wxDC& dc) override;
    wxString getImage();

public:
    ImageCanvas(wxWindow* parent, wxWindowID id);
    void centerScrollbars();
    ~ImageCanvas();
};

