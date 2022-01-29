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

    //Global key bindings
    Bind(wxEVT_CHAR_HOOK, &ImageCanvas::onKeyOpen, this);
}

ImageCanvas::~ImageCanvas(){}

wxString ImageCanvas::getImage()
{
    wxFileDialog openFileDialog
    (this, _("Open Image"), "", "", ".jpg files (*.jpg)|*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return wxEmptyString;

    //Get path to image
    wxString fileLocation = (openFileDialog.GetPath());
    //wxString fileName = openFileDialog.GetFilename();

    return fileLocation;
}

void ImageCanvas::onKeyOpen(wxKeyEvent& event)
{
    wxString fileLocation = ImageCanvas::getImage();

    wxChar key = event.GetUnicodeKey();
    if (key == 'O')
    {
        wxLogStatus(fileLocation);
        ImageWidget* imageWidget01 = new ImageWidget(this, wxID_ANY, wxDefaultPosition, wxSize(250, 250), "image3.jpg");
    }

}

void ImageCanvas::rightDown(wxMouseEvent& event) {}

void ImageCanvas::rightUp(wxMouseEvent& event){}

void ImageCanvas::OnDraw(wxDC& dc){}

//---------------------------------------------------------------------------
// MainFrame
//---------------------------------------------------------------------------

MainFrame::MainFrame(wxWindow* parent, wxWindowID 	id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
{
    CreateStatusBar();
}

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    //Create box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    //Create a mainFrame and mainImageCanvas
    MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, "rCanvas", wxPoint(100,100), wxSize(1280,720));
    ImageCanvas* mainImageCanvas = new ImageCanvas(mainFrame, wxID_ANY);

    ImageWidget* imageWidget02 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(500, 500), "image.jpg");
    ImageWidget* imageWidget01 = new ImageWidget(mainImageCanvas, wxID_ANY, wxDefaultPosition, wxSize(250, 250), "image3.jpg");

    //Add panel to sizer, fit frame to sizer
    sizer->Add(mainImageCanvas, 1, wxEXPAND | wxALL, 3);
    mainFrame->SetSizer(sizer);
    mainFrame->Show(true);
    mainFrame->Center();

    return true;
}

wxIMPLEMENT_APP(MyApp);
