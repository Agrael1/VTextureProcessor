/**
 * @file App.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for loading project view initialization
 */
#pragma once
#include <QApplication>
#include <QStyleFactory>
#include <UI/Window.h>
#include <UI/ProjectsWindow.h>
#include <optional>

class App : public QObject
{
public:
	App(int &xargc, char** xargv);
public:
	int Start();
private:
	bool event(QEvent* e);
	std::optional<Window> window;
	std::optional<UI::ProjectsWindow> projects;
	QApplication app;
private:
	static constexpr std::string_view AppVer = "0.1.1";
	static constexpr std::string_view AppTheme = "Fusion";
};