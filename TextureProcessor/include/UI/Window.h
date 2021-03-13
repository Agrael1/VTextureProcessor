#pragma once
#include <QMainWindow>
#include <QMenuBar>
#include <UI/FlowScene.h>
#include <UI/FlowView.h>
#include <UI/UINode.h>

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
private:
	QMenu menu;
	QAction Aclear;
	
	FlowScene scene;
	FlowView view;

	UI::Node node;
};