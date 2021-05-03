#pragma once
#include <QMainWindow>
#include <UI/FlowView.h>

class Window : public QMainWindow
{
	struct Internal
	{
		Internal(QObject* x):scene(x), view(scene){}
		UI::FlowScene scene;
		FlowView view;
	};
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
		a.reset();
	}

private:
	QMenu menu;
	QAction Aclear;
	
	std::optional<Internal> a;
};