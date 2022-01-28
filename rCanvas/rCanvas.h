#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class ImageCanvas : public wxScrolledWindow
{
private:
    void OnDraw(wxDC& dc) override;
    void rightDown(wxMouseEvent& event);
    void rightUp(wxMouseEvent& event);
    void inMotion(wxMouseEvent& event);
    void leftDown(wxMouseEvent& event);

public:
    ImageCanvas(wxWindow* parent, wxWindowID id);
    ~ImageCanvas();
};

class ImageWidget : public wxPanel
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
    ImageWidget(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, wxString imgPath);
    ~ImageWidget();
};
