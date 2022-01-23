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

public:
    ImagePanel(wxWindow* parent, wxWindowID id);
    ~ImagePanel();
    void OnDraw(wxDC& dc) override;
    void rightClick(wxMouseEvent& event);
    
    DECLARE_EVENT_TABLE()
};
