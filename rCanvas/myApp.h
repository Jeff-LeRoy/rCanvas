#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class MainFrame : public wxFrame
{
private:
    PopupWindow* helpPopup{};
    void OnAbout(wxKeyEvent& event);

public:
    MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size);
};
