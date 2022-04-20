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

using namespace QtLiterals;

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
private:
	static constexpr ver::u16string_literal AppVer = u"0.6.0";
	static constexpr ver::u16string_literal AppTheme = u"Fusion";
	static constexpr ver::u16string_literal AppName = u"Veritas Texture Editor";
};