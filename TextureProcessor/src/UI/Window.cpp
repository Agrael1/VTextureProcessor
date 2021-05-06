#include <UI/Window.h>


Window::Window(int32_t width, int32_t height)
	:file("File"), Aclear("Clear"), windows("Windows"), Aprops("Properties")
{
	a.emplace(this);

	resize(width, height);
	auto& mb = *menuBar();
	mb.addMenu(&file);
	a->view.AppendViewMenu(mb);
	mb.addMenu(&windows);

	connect(&Aclear, &QAction::triggered, this, &Window::OnClearTriggered);
	connect(&Aprops, &QAction::triggered, this, &Window::OnProps);
	file.addAction(&Aclear);
	windows.addAction(&Aprops);

	a->scene.setSceneRect(-32000, -32000, 64000, 64000);

	setCentralWidget(&a->view);
	addDockWidget(Qt::RightDockWidgetArea, &a->props);
}

void Window::OnClearTriggered()
{
	a->scene.Clear();
}
void Window::OnProps()
{
	a->props.show();
}