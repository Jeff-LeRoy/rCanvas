// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/wx.h>
#include "rCanvas.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "Hello World")
{
    //Create File Menu
    wxMenu* menuFile = new wxMenu;
    wxMenuItem* menuImport = new wxMenuItem(menuFile, 1001, "Import");
    menuFile->Append(menuImport);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    //Create menu bar
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    //Create Status bar
    CreateStatusBar();
    //SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnImport(wxCommandEvent& event)
{

}
