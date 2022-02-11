#pragma once

class ImageWidget : public wxPanel
{
private:
    wxPoint m_imageWidgetClickPos{};
    bool m_WidgetDragging{ false };
    wxImage* m_image = nullptr;
    wxPoint m_scale{ 1,1 };
    double percent{ 100.0 };


    void OnCaptureLost(wxMouseCaptureLostEvent&);
    void mouseScrolling(wxMouseEvent& event);
    void mouseMoving(wxMouseEvent& event);
    void leftDown(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void leftUp(wxMouseEvent& event);

public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidget();
};
