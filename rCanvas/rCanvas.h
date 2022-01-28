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
    int mouseLocal_x{};
    int mouseLocal_y{};
    wxWindow* m_parent = nullptr;
    wxBitmap* image = nullptr;
    int m_imgHeight{};
    int m_imgWidth{};

    void leftDown(wxMouseEvent& event);
    void leftUp(wxMouseEvent& event);
    void mouseMoving(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
public:
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidget();
};
