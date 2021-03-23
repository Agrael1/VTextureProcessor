#pragma once
#include <QApplication>
#include <QStyleFactory>
#include <UI/Window.h>
#include <optional>

class App
{
public:
	App(int argc, char** argv);
public:
	int Start();
private:
	std::optional<Window> window;
	QApplication app;
private:
	static constexpr std::string_view AppName = "VTexEditor";
	static constexpr std::string_view AppVer = "0.0.1";
	static constexpr std::string_view AppTheme = "Fusion";
};