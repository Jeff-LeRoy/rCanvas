#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class ImagePanel : public wxScrolledWindow
{
    wxBitmap* image;
    int w{};
    int h{};
public:
    ImagePanel(wxWindow* parent, wxWindowID id);
    ~ImagePanel();
    void OnDraw(wxDC& dc) override;
    
    //DECLARE_EVENT_TABLE()
};
