/**
 * @file Window.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Handles menubar actions
 */

#include <UI/Window.h>
#include <Windows/SceneTab.h>
#include <Windows/EditorTab.h>

namespace fs = std::filesystem;

/**
 * @brief Creates a main application window
 * @param width of a window
 * @param height of a window
 * @param xprojPath file project that is being worked upon
*/
Window::Window(int32_t width, int32_t height, std::filesystem::path&& xprojPath)
{
	a.emplace(this, std::move(xprojPath));
	resize(width, height);
}

Window::Internal::Internal(QMainWindow* x, std::filesystem::path&& projPath)
	: tab(x, cur_scene)
	, file("File")
	, windows("Windows")
	, nodes("Nodes")
	, view("View")
	, Aclear("Clear")
	, Aprops("Properties")
	, Aexport("Export All")
	, Asave("Save")
	, Asaveas("Save As")
	, Aload("Open Project")
	, Acreaten("Create Node")
	, Aloadn("Load Existing")
	, Adelet("Delete Selected")
	, Aclrselect("Clear Selection")
{
	auto& cs = tab.LoadTab<SceneTab>({ projPath }, projPath.filename().string(), props, std::move(projPath));
	auto& mb = *x->menuBar();

	mb.addMenu(&file);
	mb.addMenu(&view);
	mb.addMenu(&nodes);
	mb.addMenu(&windows);


	x->connect(&Aclear, &QAction::triggered, [this]() { OnClearTriggered(); });
	x->connect(&Aprops, &QAction::triggered, [this]() { OnProps(); });
	x->connect(&Aexport, &QAction::triggered, [this]() {OnExport(); });
	x->connect(&Asave, &QAction::triggered, [this]() {OnSave(); });
	x->connect(&Asaveas, &QAction::triggered, [this]() {OnSaveAs(); });
	x->connect(&Aload, &QAction::triggered, [this]() {OnLoad(); });
	x->connect(&Acreaten, &QAction::triggered, [this]() {OnCreateNode(); });

	x->connect(&Adelet, &QAction::triggered, [this]() {OnViewDelete(); });
	x->connect(&Aclrselect, &QAction::triggered, [this]() {OnViewClrSel(); });

	file.addAction(&Aclear);
	file.addAction(&Aload);
	file.addAction(&Asave);
	file.addAction(&Asaveas);
	file.addSeparator();
	file.addAction(&Aexport);
	windows.addAction(&Aprops);

	nodes.addAction(&Acreaten);
	nodes.addAction(&Aloadn);

	view.addAction(&Adelet);
	view.addAction(&Aclrselect);

	Asave.setShortcut(QKeySequence{ QKeySequence::StandardKey::Save });
	Asaveas.setShortcut(QKeySequence{ x->tr("Ctrl+Shift+S") });
	Aload.setShortcut(QKeySequence{ QKeySequence::StandardKey::Open });

	x->setCentralWidget(&tab);
	x->addDockWidget(Qt::RightDockWidgetArea, &props);
	x->resizeDocks({ &props }, { 250 }, Qt::Horizontal);
}

void Window::Internal::OnLoad()
{
	fs::path proj_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing project",
		"",
		"(*.vtproj);;"
	).toStdString() };

	if (proj_path.empty()) return;
	proj_path = proj_path.make_preferred();
	auto& cs = tab.LoadTab<SceneTab>({ proj_path }, proj_path.filename().string(), props, std::move(proj_path));
}

void Window::Internal::OnCreateNode()
{
	tab.TempTab<EditorTab>("New Node");
}
