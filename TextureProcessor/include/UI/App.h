/**
 * @file App.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for loading project view initialization
 */
#pragma once
#include <QApplication>
#include <Windows/MainWindow.h>
#include <Windows/ProjectsWindow.h>
#include <Logic/ApplicationConfig.h>
#include <variant>

class App : public QObject
{
public:
	App(int &xargc, char** xargv);
public:
	int Start();
private:
	bool event(QEvent* e);
	std::variant<MainWindow, ProjectsWindow, bool> window;
	QApplication app;
	ApplicationConfig cfg;
private:
	static constexpr std::string_view AppVer = "0.2.1";
	static constexpr std::string_view AppTheme = "Fusion";
};