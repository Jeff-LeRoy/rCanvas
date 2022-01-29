#pragma once

class ImageWidgetX : public wxPanel
{
private:
    bool m_IwMouseDragging{ false };
    //wxWindow* m_IwParent = nullptr;
    wxBitmap* m_IwImage = nullptr;
    int m_IwMouseLocal_x{};
    int m_IwMouseLocal_y{};
    //int m_IwImgHeight{};
    //int m_IwImgWidth{};

    void mouseMoving(wxMouseEvent& event);
    void leftDown(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void leftUp(wxMouseEvent& event);
public:
    ImageWidgetX(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidgetX();
};
