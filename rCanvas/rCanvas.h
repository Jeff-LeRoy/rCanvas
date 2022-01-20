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

class ImagePanel : public wxPanel
{
    wxBitmap image;
public:
    ImagePanel(wxWindow* parent);
    void OnPaint(wxPaintEvent& event);
    void DrawMyImage(wxDC& dc);
    void paintNow();
};
