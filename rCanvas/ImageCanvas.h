#pragma once

class ImageCanvas : public wxScrolledWindow
{
private:
    //wxVector<ImageWidget> vecImageWidgets{};
    int subgridPixelSpacing{ 200 };
    bool panCanvas{ false };
    wxPoint virtualSize{};
    wxPoint startMousePos;

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

