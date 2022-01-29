#pragma once

class ImageWidget : public wxPanel
{
private:
    bool m_MouseDragging{ false };
    //wxWindow* m_IwParent = nullptr;
    wxBitmap* m_Image = nullptr;
    int m_MouseLocal_x{};
    int m_MouseLocal_y{};
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
