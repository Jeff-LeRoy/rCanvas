#pragma once

class CanvasPopup : public wxPopupTransientWindow
{
private:

public:
	CanvasPopup(wxWindow* parent);
};


class CanvasDialog : public wxDialog
{
private:
	wxTextCtrl* m_widthEntry{};
	wxTextCtrl* m_heigthEntry{};

public:
	CanvasDialog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxPoint& canvasSize);
	wxPoint GetCanvasSizeEntered();
};

