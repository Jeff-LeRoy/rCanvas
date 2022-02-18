#pragma once

class ImageWidget : public wxPanel
{
private:
    wxDouble m_scaleIncrimentor{ 100.0 };
    wxPoint2DDouble m_scale{ 1.0, 1.0 };
    wxPoint m_imageWidgetClickPos{};
    const bool* m_isCanvasPanning{};
    bool m_widgetDragging{ false };
    wxPoint m_originalDimensions{};
    wxBitmap* m_bitmap = nullptr;
    wxImage* m_image = nullptr;
    int m_scaleMultiplier{ 1 };
    wxDouble m_aspectRatio{}; //Can move this to local
    bool m_scalingImage{};
    wxString m_imgPath{};

    void OnCaptureLost(wxMouseCaptureLostEvent&);
    void rescaleImage(wxBitmap* bitmap, int max);
    void scrollWheelZoom(wxMouseEvent& event);
    //void onRightDClick(wxMouseEvent& event);
    void leftIsDragging(wxMouseEvent& event);
    void hoverPrinting(wxMouseEvent& event);//Remove later
    void rightIsDown(wxMouseEvent& event);
    void enterWindow(wxMouseEvent& event);
    void leftIsDown(wxMouseEvent& event);
    void calculateAspectRatio(int max);
    void leftIsUp(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void onKey_F(wxKeyEvent& event);
    void calculateAspectRatio(); //Might not need seperate member for this 
    void renderScaled(wxDC& dc);
    void render(wxDC& dc);

public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath, const bool& m_panCanvas);
    ~ImageWidget();

    void setGlobalScale();
};
