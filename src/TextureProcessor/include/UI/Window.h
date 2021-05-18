/**
 * @file Window.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node model logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <QMainWindow>
#include <UI/FlowView.h>
#include <UI/Properties.h>
#include <fstream>
#include <filesystem>

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
	static constexpr std::string_view AppName = "VTexEditor";
public:
	Window(int32_t width, int32_t height, std::filesystem::path&& projPath);
public:
	void OnClearTriggered();
	void OnProps();
	void OnExport();
	void OnSave();
	void OnSaveAs();
	void OnLoad();
	void LoadFile();
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
	QAction Asaveas;
	QAction Aload;
	QAction Aprops;
	std::filesystem::path projPath;
	std::optional<Internal> a;
};