#pragma once

class ImageWidget : public wxPanel
{
private:
    wxDouble m_scaleIncrimentor{ 100.0 };
    wxPoint2DDouble m_scale{ 1.0, 1.0 };
    wxPoint m_imageWidgetClickPos{};
    bool m_WidgetDragging{ false };
    wxPoint originalDimensions{};
    wxImage* m_image = nullptr;
    int m_scaleMultiplier{ 1 };
    wxDouble m_aspect{}; //Can move this to local


    void OnCaptureLost(wxMouseCaptureLostEvent&);
    void scrollWheelZoom(wxMouseEvent& event);
    void leftIsDragging(wxMouseEvent& event);
    void rightIsDown(wxMouseEvent& event);
    void leftIsDown(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void leftIsUp(wxMouseEvent& event);
    void hoverPrinting(wxMouseEvent& event);//Remove later

public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidget();
};
