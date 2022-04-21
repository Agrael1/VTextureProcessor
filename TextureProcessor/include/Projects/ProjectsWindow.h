/**
 * @file ProjectsWindow.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node header color specification
 */
#pragma once
#include <Projects/Frameless.h>
#include <Projects/CreatePage.h>
#include <Projects/FrameControl.h>
#include <Projects/StartWindow.h>


class ProjectsWindow : public QMainWindow
{
public:
	ProjectsWindow(int32_t width, int32_t height, QObject* app);
protected:
	void mouseMoveEvent(QMouseEvent* e)override;
private:
	QObject* app;
	ProjectsData pdata;
	
	UI::Internal::FrameControl window;
	UI::FrameLess f;
	UI::MainPage mp;
	UI::CreatePage cp;
};
