#include <UI/Window.h>


Window::Window(int32_t width, int32_t height)
	:menu("File"), Aclear("Clear", this)
{
	a.emplace(this);

	resize(width, height);
	auto& mb = *menuBar();
	mb.addMenu(&menu);
	a->view.AppendViewMenu(mb);

	connect(&Aclear, &QAction::triggered, this, &Window::onClearTriggered);
	menu.addAction(&Aclear);

	a->scene.setSceneRect(-32000, -32000, 64000, 64000);

	setCentralWidget(&a->view);
}

void Window::onClearTriggered()
{
	a->scene.Clear();
}