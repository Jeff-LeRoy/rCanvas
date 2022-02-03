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
    wxPoint startMousePos;
    //wxPoint inProgressMousePos;
    bool panCanvas{ false };
    void rightDown(wxMouseEvent& event);
    void rightDragging(wxMouseEvent& event);
    void procesCanvasPan(wxPoint inProgressMousePos, wxPoint startMousePos);
    void rightUp(wxMouseEvent& event);
    wxPoint processDistance(wxPoint pt, wxPoint start);
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
