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


class MainWindow : public QMainWindow
{
public:
	MainWindow(int32_t width, int32_t height, std::filesystem::path&& projPath, QSize resolution);
public:
	void closeEvent(QCloseEvent* event)override;

	void OnClearTriggered();
	void OnProps()
	{
		property_dock.show();
	}
	void OnExport();
	void OnLoad();
	void OnCreateNode();
	void OnLoadNode();
	void OnViewDelete();
	void OnViewClrSel();
private:
	std::optional<UI::Windows::TabRelay> tab;
	UI::Windows::Properties property_dock;

	QMenu file;
	QMenu windows;
	QMenu nodes;
	QMenu view;
	UI::Windows::SceneTab* cur_scene = nullptr;
};