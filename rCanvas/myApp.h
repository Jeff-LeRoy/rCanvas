#pragma once

class MyApp : public wxApp
{
private:

public:
    virtual bool OnInit() override;
};

class MainFrame : public wxFrame
{
private:
    void OnAbout(wxKeyEvent& event);
    CanvasPopup* helpPopup{};
    void ToggleFullScreen(wxKeyEvent& event);

public:
    MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size);
};
