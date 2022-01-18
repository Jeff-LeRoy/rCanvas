#include <wx/wxprec.h>
#include <wx/wx.h>
#include "rCanvas.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    frame->Center(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "rCanvas")
{
    //Create File Menu
    wxMenu* menuFile = new wxMenu;
    wxMenuItem* menuImport = new wxMenuItem(menuFile, 1001, "Import");
    menuFile->Append(menuImport);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnImport, this, 1001);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    //Create menu bar
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    //Create Status bar
    //CreateStatusBar();
    //SetStatusText("Welcome to wxWidgets!");
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnImport(wxCommandEvent& event)
{
    wxFileDialog openFileDialog
    (this, _("Open Image"), "", "", ".jpg files (*.jpg)|*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    //Get path to image
    wxString fileLocation = (openFileDialog.GetPath());
    wxString fileName = openFileDialog.GetFilename();
}
