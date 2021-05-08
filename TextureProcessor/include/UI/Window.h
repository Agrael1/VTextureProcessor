#pragma once
#include <QMainWindow>
#include <UI/FlowView.h>
#include <UI/Properties.h>
#include <fstream>

class Window : public QMainWindow
{
	struct Internal
	{
		Internal(QWidget* x) :scene(x, props), view(scene) { }
		UI::Properties props;
		UI::FlowScene scene;
		FlowView view;
	};
public:
	Window(int32_t width, int32_t height/*, std::fstream&& project*/);
public:
	void OnClearTriggered();
	void OnProps();
	void OnExport();
	void OnSave();
	void OnLoad();
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
		a.reset();
	}

private:
	QMenu file;
	QMenu windows;
	QAction Aclear;
	QAction Aexport;
	QAction Asave;
	QAction Aload;
	QAction Aprops;
	std::optional<Internal> a;
};