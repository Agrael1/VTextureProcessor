#pragma once
#include <QMainWindow>
#include <QMenuBar>
#include <UI/NodeBox.h>
#include <UI/NodeView.h>

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
	
	NodeBox scene;
	NodeSceneView view;
};