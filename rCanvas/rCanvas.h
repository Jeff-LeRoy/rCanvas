#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class ImagePanel : public wxScrolledWindow
{
private:
    wxBitmap* image;
    int m_imgWidth{};
    int m_imgHeight{};

    void OnDraw(wxDC& dc) override;

    void rightDown(wxMouseEvent& event);
    void rightUp(wxMouseEvent& event);
    void inMotion(wxMouseEvent& event);

public:
    //Declarations
    ImagePanel(wxWindow* parent, wxWindowID id, wxString imgPath);
    ~ImagePanel();
};
