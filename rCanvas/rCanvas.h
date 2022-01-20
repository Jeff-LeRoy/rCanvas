#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

//class MyFrame : public wxFrame
//{
//public:
//    MyFrame();
//private:
//    void OnImport(wxCommandEvent& event);
//};

class ImagePanel : public wxScrolledWindow
{
    wxBitmap image;
    int w;
    int h;
public:
    ImagePanel(wxWindow* parent);
    void OnPaint(wxPaintEvent& event);
    void DrawMyImage(wxDC& dc);

    DECLARE_EVENT_TABLE()
};
