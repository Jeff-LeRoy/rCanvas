#pragma once
class rCanvas
{
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnExit(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
};

class ImagePanel : public wxPanel
{
    wxBitmap image;

public:
    ImagePanel(wxWindow* parent);
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render(wxDC& dc);
};
