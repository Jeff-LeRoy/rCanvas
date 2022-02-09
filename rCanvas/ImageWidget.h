#pragma once

class ImageWidget : public wxPanel
{
private:
    bool m_WidgetDragging{ false };
    wxBitmap* m_Image = nullptr;
    wxPoint m_imageWidgetClickPos{};
    int m_ImgHeight{};
    int m_ImgWidth{};

    void OnCaptureLost(wxMouseCaptureLostEvent&);
    void mouseMoving(wxMouseEvent& event);
    void leftDown(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void leftUp(wxMouseEvent& event);

public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidget();
};
