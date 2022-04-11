#pragma once

class ImageWidget : public wxPanel
{
private:
    //Member Variables
    wxDouble m_scaleIncrimentor{ 100.0 };
    wxPoint m_imageWidgetClickPos{};
    const bool* m_isCanvasPanning{}; //From Canvas class
    const bool* m_loadingSaveFile{}; //From Canvas class
    bool m_widgetDragging{ false };
    wxPoint m_originalDimensions{};
    wxBitmap* m_bitmap = nullptr;
    const wxPoint* m_viewStart{}; //From Canvas class
    wxImage* m_image = nullptr;
    int m_scaleMultiplier{ 1 };
    bool m_canDelete = false;
    wxStatusBar* m_statusBar; //From mainFrame class
    wxPoint m_scale{ 1, 1 };
    bool m_scalingImage{};
    wxString m_imgPath{};
    wxTimer m_timer;


    //Member Functions
    void RescaleImage(wxBitmap* bitmap, int max);
    void CalculateAspectRatio(int max);
    wxPoint CalcPositionOnCanvas();
    void CalculateAspectRatio(); //Might not need seperate member for this 
    void RenderScaled(wxDC& dc);
    void Render(wxDC& dc);
    void ZoomToCursor(wxPoint& mousePos, bool scalingUp, 
        wxPoint2DDouble sizeAfterScale, wxPoint2DDouble sizeBeforeScale);


    //Event Handlers
    void OnCaptureLost(wxMouseCaptureLostEvent&);
    void ScrollWheelZoom(wxMouseEvent& event);
    void LeftIsDragging(wxMouseEvent& event);
    void HoverPrinting(wxMouseEvent& event);//Remove later
    void RightIsDown(wxMouseEvent& event);
    void EnterWindow(wxMouseEvent& event);
    void ExitWindow(wxMouseEvent& event);
    void LeftIsDown(wxMouseEvent& event);
    void LeftIsUp(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnKey_D(wxKeyEvent& event);
    void OnKey_F(wxKeyEvent& event);
    void OnTimer(wxTimerEvent& event);

public:
    ImageWidget(wxWindow* parent, 
        wxWindowID id, 
        wxPoint pos, 
        wxSize size, 
        wxString imgPath, 
        const bool& m_panCanvas, 
        wxStatusBar& statusBar,
        const wxPoint& viewStart,
        const bool& saving,
        int imageHeight);
    ~ImageWidget();

    //Getters
    wxString GetImgPath() { return m_imgPath; }
    wxPoint GetCurrentScale() { return m_scale; }
    wxPoint GetPositionOnCanvas() { return wxPoint(CalcPositionOnCanvas().x, CalcPositionOnCanvas().y); }

    //for scaling all images (not doing anything atm)
    void SetGlobalScale();
};
