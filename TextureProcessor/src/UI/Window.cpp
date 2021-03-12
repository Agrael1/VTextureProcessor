#include <UI/Window.h>


Window::Window(int32_t width, int32_t height)
	:menu("File"), Aclear("Clear", this), scene(this), view(&scene)
{
	resize(width, height);
	menuBar()->addMenu(&menu);

	connect(&Aclear, &QAction::triggered, this, &Window::onClearTriggered);
	menu.addAction(&Aclear);

	scene.setSceneRect(-32000, -32000, 64000, 64000);

	setCentralWidget(&view);

	scene.addItem(&node);
}

void Window::onClearTriggered()
{
	scene.clear();
}