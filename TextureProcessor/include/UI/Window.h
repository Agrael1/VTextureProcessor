#pragma once
#include <QMainWindow>
#include <UI/FlowView.h>

class Window : public QMainWindow
{
public:
	Window(int32_t width, int32_t height);
public:
	void onClearTriggered();
	void ShowMaximized()
	{
		showMaximized();
	}
	void Show()
	{
		showNormal();
	}
	void closeEvent(QCloseEvent* event)override
	{
		scene.RemoveEngine();
	}

private:
	QMenu menu;
	QAction Aclear;
	
	UI::FlowScene scene;
	FlowView view;
};