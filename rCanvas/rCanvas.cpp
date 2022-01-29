#include <wx/wx.h>
#include "rCanvas.h"
#include "ImageWidget.h"

//---------------------------------------------------------------------------
// ImageCanvas
//---------------------------------------------------------------------------

ImageCanvas::ImageCanvas(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id)
{
    this->SetBackgroundColour(wxColor(34, 35, 37));
    this->SetDoubleBuffered(true);

    SetScrollbars(1, 1, 1000, 1000, 0, 0);
    SetScrollRate(5, 5);

    //Bind(wxEVT_RIGHT_DOWN, &ImageCanvas::rightDown, this);
    //Bind(wxEVT_RIGHT_UP, &ImageCanvas::rightUp, this);
    //Bind(wxEVT_MOTION, &ImageCanvas::inMotion, this);
    //Bind(wxEVT_LEFT_DOWN, &ImageCanvas::leftDown, this);
    Bind(wxEVT_KEY_DOWN, &ImageCanvas::open, this);
}

ImageCanvas::~ImageCanvas(){}

void ImageCanvas::rightDown(wxMouseEvent& event){}

void ImageCanvas::open(wxKeyEvent& event) 
{
    wxChar key = event.GetUnicodeKey();
    if (key == 'O')
        wxLogStatus("you clicked Open");
}

void ImageCanvas::rightUp(wxMouseEvent& event){}

void ImageCanvas::OnDraw(wxDC& dc){}

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------

MainFrame::MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
{

}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a mainFrame and Canvas
    MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(1280,720));
    //mainFrame->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnKeyDown, this);

    wxStatusBar* statusBar = mainFrame->CreateStatusBar();
    ImageCanvas* mainImageCanvas = new ImageCanvas(mainFrame, wxID_ANY);

    ImageWidgetX* imageWidget01 = new ImageWidgetX(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(250, 250), "image3.jpg");
    ImageWidgetX* imageWidget02 = new ImageWidgetX(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(500, 500), "image.jpg");

    //Add panel to sizer, fit frame to sizer
    sizer->Add(mainImageCanvas, 1, wxEXPAND | wxALL, 10);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);
    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);

//void MyFrame::OnImport(wxCommandEvent& event)
//{
//    wxFileDialog openFileDialog
//    (this, _("Open Image"), "", "", ".jpg files (*.jpg)|*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
//
//    if (openFileDialog.ShowModal() == wxID_CANCEL)
//        return;
//
//    //Get path to image
//    wxString fileLocation = (openFileDialog.GetPath());
//    wxString fileName = openFileDialog.GetFilename();
//}