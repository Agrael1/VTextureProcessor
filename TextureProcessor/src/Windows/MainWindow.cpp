/**
 * @file Window.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Handles menubar actions
 */

#include <Windows/MainWindow.h>
#include <Windows/SceneTab.h>
#include <Windows/EditorTab.h>
#include <QFileDialog>
#include <QMenuBar>

namespace fs = std::filesystem;
using namespace UI::Windows;

/**
 * @brief Creates a main application window
 * @param width of a window
 * @param height of a window
 * @param xprojPath file project that is being worked upon
*/
MainWindow::MainWindow(int32_t width, int32_t height, std::filesystem::path&& xprojPath, std::pair<int, int> resolution)
{
	a.emplace(this, std::move(xprojPath), resolution);
	resize(width, height);
}

MainWindow::Internal::Internal(QMainWindow* x, std::filesystem::path&& projPath, std::pair<int, int> resolution)
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
	x->connect(&Aloadn, &QAction::triggered, [this]() {OnLoadNode(); });

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

void MainWindow::Internal::OnClearTriggered()
{
	if (cur_scene)
		cur_scene->Clear();
}

void MainWindow::Internal::OnExport()
{
	if (cur_scene)
		cur_scene->Export();
}

void MainWindow::Internal::OnLoad()
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

void MainWindow::Internal::OnCreateNode()
{
	tab.TempTab<EditorTab>("New Node");
}

void MainWindow::Internal::OnLoadNode()
{
	fs::path node_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing node",
		"nodes",
		"(*.json);;"
	).toStdString() };

	if (node_path.empty()) return;
	node_path = node_path.make_preferred();
	auto& cs = tab.LoadTab<EditorTab>({ node_path }, node_path.filename().string(), std::move(node_path));
}

void MainWindow::Internal::OnViewDelete()
{
	if (cur_scene)
		cur_scene->DeleteSelected();
}

void MainWindow::Internal::OnViewClrSel()
{
	if (cur_scene)
		cur_scene->ClearSelection();
}
