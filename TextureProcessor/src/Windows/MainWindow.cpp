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

#include <Logic/Engine.h>


namespace fs = std::filesystem;
using namespace UI::Windows;

/**
 * @brief Creates a main application window
 * @param width of a window
 * @param height of a window
 * @param xprojPath file project that is being worked upon
*/
MainWindow::MainWindow(int32_t width, int32_t height, std::filesystem::path&& xprojPath, QSize resolution)
	:file("File")
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
	Engine::Instance();
	resize(width, height);
	tab.emplace(this, cur_scene);

	auto& mb = *menuBar();
	mb.addMenu(&file);
	mb.addMenu(&view);
	mb.addMenu(&nodes);
	mb.addMenu(&windows);


	connect(&Aclear, &QAction::triggered, [this]() { OnClearTriggered(); });
	connect(&Aprops, &QAction::triggered, [this]() { OnProps(); });
	connect(&Aexport, &QAction::triggered, [this]() {OnExport(); });
	connect(&Asave, &QAction::triggered, [this]() {OnSave(); });
	connect(&Asaveas, &QAction::triggered, [this]() {OnSaveAs(); });
	connect(&Aload, &QAction::triggered, [this]() {OnLoad(); });
	connect(&Acreaten, &QAction::triggered, [this]() {OnCreateNode(); });
	connect(&Aloadn, &QAction::triggered, [this]() {OnLoadNode(); });

	connect(&Adelet, &QAction::triggered, [this]() {OnViewDelete(); });
	connect(&Aclrselect, &QAction::triggered, [this]() {OnViewClrSel(); });

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
	Asaveas.setShortcut(QKeySequence{ tr("Ctrl+Shift+S") });
	Adelet.setShortcut(QKeySequence{ QKeySequence::StandardKey::Delete });
	Aload.setShortcut(QKeySequence{ QKeySequence::StandardKey::Open });

	setCentralWidget(&*tab);
	addDockWidget(Qt::RightDockWidgetArea, &xprops);
	resizeDocks({ &xprops }, { 250 }, Qt::Horizontal);
	
	tab->LoadTab<SceneTab>({ xprojPath }, xprojPath.filename().string(), xprops, std::move(xprojPath), resolution);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	tab.reset();
	Engine::Destroy();
}

void MainWindow::OnClearTriggered()
{
	if (cur_scene)
		cur_scene->Clear();
}

void MainWindow::OnExport()
{
	if (cur_scene)
		cur_scene->Export();
}

void MainWindow::OnLoad()
{
	fs::path proj_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing project",
		"",
		"(*.vtproj);;"
	).toStdString() };

	if (proj_path.empty()) return;
	proj_path = proj_path.make_preferred();
	auto& cs = tab->LoadTab<SceneTab>({ proj_path }, proj_path.filename().string(), xprops, std::move(proj_path), QSize(256, 256));
}

void MainWindow::OnCreateNode()
{
	tab->TempTab<EditorTab>("New Node");
}

void MainWindow::OnLoadNode()
{
	fs::path node_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing node",
		"nodes",
		"(*.json);;"
	).toStdString() };

	if (node_path.empty()) return;
	node_path = node_path.make_preferred();
	auto& cs = tab->LoadTab<EditorTab>({ node_path }, node_path.filename().string(), std::move(node_path));
}

void MainWindow::OnViewDelete()
{
	if (cur_scene)
		cur_scene->DeleteSelected();
}

void MainWindow::OnViewClrSel()
{
	if (cur_scene)
		cur_scene->ClearSelection();
}
