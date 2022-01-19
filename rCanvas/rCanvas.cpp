#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "rCanvas.h"

ImagePanel::ImagePanel(wxWindow* parent) : wxPanel(parent)
{
    image.LoadFile("image.jpg", wxBITMAP_TYPE_JPEG);
}

void ImagePanel::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(image, 0, 0, false);
}


bool MyApp::OnInit()
{
    // Initialize all image handlers
    wxInitAllImageHandlers();

    // Initialize box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    // Initialize a frame
    wxFrame* frame = new wxFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(640,480));
    ImagePanel* drawImage = new ImagePanel(frame);

    sizer->Add(drawImage, 1, wxEXPAND);

    frame->SetSizer(sizer);

    frame->Show(true);

    return true;
}

wxIMPLEMENT_APP(MyApp);


MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "rCanvas")
{
    /*
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
    */
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