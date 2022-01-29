#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class ImageCanvas : public wxScrolledWindow
{
private:
    //wxVector<ImageWidget> vecImageWidgets{};
    wxPoint rightDownPos;
    wxPoint inProgressMousePos;
    bool panCanvas{ false };
    void rightDown(wxMouseEvent& event);
    void rightDragging(wxMouseEvent& event);
    void rightUp(wxMouseEvent& event);
    void onKeyOpen(wxKeyEvent& event);
    void OnDraw(wxDC& dc) override;
    wxString getImage();

public:
    ImageCanvas(wxWindow* parent, wxWindowID id);
    ~ImageCanvas();
};

class MainFrame : public wxFrame
{
private:

public:
    MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size);
};
