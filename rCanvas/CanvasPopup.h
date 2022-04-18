#pragma once

class CanvasPopup : public wxPopupTransientWindow
{
private:
	//void OnKillFocus(wxFocusEvent& event);
	//void OnSetFocus(wxFocusEvent& event);
	//void OnKeyDown(wxKeyEvent& event);

public:
	CanvasPopup(wxWindow* parent);
};


class CanvasDialog : public wxDialog
{
private:
	wxTextCtrl* widthEntry{};
	wxTextCtrl* heigthEntry{};

public:
	CanvasDialog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxPoint& canvasSize);
	wxPoint GetCanvasSize();
};

