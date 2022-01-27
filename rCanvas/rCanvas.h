#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class ImageCanvas : public wxScrolledWindow
{
private:
    wxRect imageBoundingBox{};
    int m_imgHeight{};
    int m_imgWidth{};
    wxBitmap* image;

    void OnDraw(wxDC& dc) override;
    void rightDown(wxMouseEvent& event);
    void rightUp(wxMouseEvent& event);
    void inMotion(wxMouseEvent& event);
    void leftDown(wxMouseEvent& event);
public:
    ImageCanvas(wxWindow* parent, wxWindowID id, wxString imgPath);
    ~ImageCanvas();
};

class ImageWidget : public wxPanel
{
private:
    bool m_mouseDragging{ false };
    int m_x{};
    int m_y{};
    //int m_previous_x;
    //int m_previous_y;
    wxWindow* m_parent;

    void leftDown(wxMouseEvent& event);
    void leftUp(wxMouseEvent& event);
    void mouseMoving(wxMouseEvent& event);
public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size);
};
