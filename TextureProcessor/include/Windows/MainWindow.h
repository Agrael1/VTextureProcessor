/**
 * @file Window.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Main window event handling
 */
#pragma once
#include <QMainWindow>
#include <QMenu>

#include <Windows/Properties.h>
#include <Windows/REProperties.h>
#include <Windows/TabRelay.h>
#include <UI/FlowCodex.h>


class MainWindow : public QMainWindow
{
public:
	MainWindow(int32_t width, int32_t height, std::filesystem::path&& projPath, QSize resolution);
public:
	void closeEvent(QCloseEvent* event)override;

	void OnClearTriggered();
	void OnProps()
	{
		xprops.show();
	}
	void OnExport();
	void OnSave()
	{
		tab.OnSave();
	}
	void OnSaveAs()
	{
		tab.OnSaveAs();
	}
	void OnLoad();
	void OnCreateNode();
	void OnLoadNode();
	void OnViewDelete();
	void OnViewClrSel();
private:
	UI::Windows::TabRelay tab;
	UI::Windows::XProperties xprops;

	QMenu file;
	QMenu windows;
	QAction Aclear;
	QAction Aexport;
	QAction Asave;
	QAction Asaveas;
	QAction Aload;
	QAction Aprops;

	QMenu nodes;
	QAction Acreaten;
	QAction Aloadn;

	QMenu view;
	QAction Adelet;
	QAction Aclrselect;
	UI::Windows::SceneTab* cur_scene = nullptr;
};