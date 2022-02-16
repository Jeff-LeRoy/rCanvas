#pragma once

class ImageWidget : public wxPanel
{
private:
    wxDouble m_scaleIncrimentor{ 100.0 };
    wxPoint2DDouble m_scale{ 1.0, 1.0 };
    wxPoint m_imageWidgetClickPos{};
    bool m_widgetDragging{ false };
    wxPoint m_originalDimensions{};
    wxImage* m_image = nullptr;
    wxBitmap* m_bitmap = nullptr;
    int m_scaleMultiplier{ 1 };
    wxDouble m_aspectRatio{}; //Can move this to local
    wxString m_imgPath{};
    bool m_scalingImage{};

    void OnCaptureLost(wxMouseCaptureLostEvent&);
    void scrollWheelZoom(wxMouseEvent& event);
    void leftIsDragging(wxMouseEvent& event);
    void rightIsDown(wxMouseEvent& event);
    void leftIsDown(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void leftIsUp(wxMouseEvent& event);
    void onKey_T(wxKeyEvent& event);
    void hoverPrinting(wxMouseEvent& event);//Remove later
    void calculateAspectRatio(); //Might not need seperate member for this 
    void renderScaled(wxDC& dc);
    void render(wxDC& dc);

public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidget();
    void setGlobalScale();
};
