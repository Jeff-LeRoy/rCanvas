#pragma once

class PopupWindow : public wxPopupTransientWindow
{
private:
	void ExitWindow(wxMouseEvent& event);
public:
	PopupWindow(wxWindow* parent);
};

