#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class ImagePanel : public wxScrolledWindow
{
    wxBitmap image;
    int w{};
    int h{};
public:
    ImagePanel(wxWindow* parent, wxWindowID id);
    void OnRender(wxPaintEvent& event);
    //void OnDraw(wxDC& dc) override;

    DECLARE_EVENT_TABLE()
};
