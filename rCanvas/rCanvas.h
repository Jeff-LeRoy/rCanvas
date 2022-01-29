#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class ImageCanvas : public wxScrolledWindow
{
private:
    void rightDown(wxMouseEvent& event);
    void rightUp(wxMouseEvent& event);
    void OnDraw(wxDC& dc) override;
    void open(wxKeyEvent& event);

public:
    ImageCanvas(wxWindow* parent, wxWindowID id);
    ~ImageCanvas();
};

class MainFrame : public wxFrame
{
public:
    MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size);
};
