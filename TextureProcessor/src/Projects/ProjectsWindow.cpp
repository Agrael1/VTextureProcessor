/**
 * @file ProjectsWindow.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node header color specification
 */

#include <Projects/ProjectsWindow.h>
#include <UI/ProjectEvent.h>

#include <QApplication>
#include <QMouseEvent>

#include <fstream>

using namespace UI;
namespace fs = std::filesystem;


ProjectsWindow::ProjectsWindow(int32_t width, int32_t height, QObject* xapp)
	:QMainWindow(nullptr, Qt::FramelessWindowHint),
	window(this), f(this), app(xapp)
{
	resize(width, height);
	setCentralWidget(&window);
	window.Layout().addWidget(&mp);
	window.Layout().addWidget(&cp);
	cp.hide();

	connect(&mp, &MainPage::OpenProject, [this](ProjectDescriptor pd) {
		QApplication::postEvent(app, new ProjectEvent(std::move(pd)));
		});
	connect(&mp, &MainPage::CreateClicked, [this]() {
		mp.hide();
		cp.show();
		});
	connect(&cp, &CreatePage::CreateClicked, [this](ProjectDescriptor pd) {
		mp.AddProject(pd.project_path);
		//create if not exists
		if (!fs::exists(pd.project_path))
			std::ofstream{ pd.project_path };
		QApplication::postEvent(app, new ProjectEvent(std::move(pd)));
		});
	connect(&cp, &CreatePage::CancelClicked, [this]() {
		cp.hide();
		mp.show();
		});
}

void ProjectsWindow::mouseMoveEvent(QMouseEvent* e)
{
	if (isMaximized())
	{
		window.Restore();
		move(e->globalPos() / 2);
		return;
	}
	f.MouseMove(e);
}
