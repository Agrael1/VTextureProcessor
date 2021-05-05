#pragma once
#include <QMainWindow>
#include <UI/FlowView.h>
#include <UI/Properties.h>

class Window : public QMainWindow
{
	struct Internal
	{
		Internal(QWidget* x) :scene(x), view(scene), props(nullptr) { }
		UI::FlowScene scene;
		FlowView view;
		Properties props;
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