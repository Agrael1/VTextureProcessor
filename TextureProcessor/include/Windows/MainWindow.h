/**
 * @file Window.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Main window event handling
 */
#pragma once
#include <QMainWindow>
#include <QMenu>

#include <Windows/Properties.h>
#include <Windows/TabRelay.h>
#include <Projects/ProjectDescriptor.h>

class MainWindow : public QMainWindow
{
public:
	MainWindow(int32_t width, int32_t height, ProjectDescriptor desc);
public:
	void closeEvent(QCloseEvent* event)override;

	void OnProps()
	{
		property_dock.show();
	}
	void OnLoad();
	void OnCreateNode();
	void OnLoadNode();
private:
	std::optional<UI::Windows::TabRelay> tab;
	UI::Windows::Properties property_dock;

	QMenu file;
	QMenu windows;
	QMenu nodes;
	QMenu view;
};