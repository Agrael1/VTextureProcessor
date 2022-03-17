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

#include <Logic/Constants.h>
#include <Logic/Engine.h>
#include <UI/ProjectEvent.h>


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
{
	Engine::Instance();
	resize(width, height);
	tab.emplace(this, cur_scene);

	auto& mb = *menuBar();
	mb.addMenu(&file);
	mb.addMenu(&view);
	mb.addMenu(&nodes);
	mb.addMenu(&windows);

	file.addAction("Clear", [this]() { OnClearTriggered(); });
	file.addAction("Load", [this]() { OnLoad(); });
	file.addAction("Save", [this]() {tab->OnSave(); }, { QKeySequence::StandardKey::Save });
	file.addAction("Save As", [this]() {tab->OnSaveAs(); }, { tr("Ctrl+Shift+S") });
	file.addSeparator();
	file.addAction("Export", [this]() {OnExport(); });

	windows.addAction("Properties", [this]() { OnProps(); });

	nodes.addAction("Create", [this]() {OnCreateNode(); }, { QKeySequence::StandardKey::New });
	nodes.addAction("Load", [this]() {OnLoadNode(); }, { QKeySequence::StandardKey::Open });

	view.addAction("Delete", [this]() {OnViewDelete(); }, { QKeySequence::StandardKey::Delete });
	view.addAction("Clear Selection", [this]() {OnViewClrSel(); });


	setCentralWidget(&*tab);
	addDockWidget(Qt::RightDockWidgetArea, &property_dock);
	resizeDocks({ &property_dock }, { 250 }, Qt::Horizontal);

	auto& toolbar = *addToolBar("");
	toolbar.setIconSize({ 16,16 });
	toolbar.addAction(QIcon(":/build.png"), "Compile", [this]() {tab->RequestActive(UI::Request::Compile); });

	tab->LoadTab<SceneTab>(std::move(xprojPath) , xprojPath.filename().string(), property_dock, resolution);
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
		ver::proj_filter.c_str()
	).toStdString() };

	if (proj_path.empty()) return;
	proj_path = proj_path.make_preferred();
	auto& cs = tab->LoadTab<SceneTab>(std::move(proj_path), proj_path.filename().string(), property_dock, QSize(256, 256));
}

void MainWindow::OnCreateNode()
{
	tab->TempTab<EditorTab>("New Node", property_dock);
}

void MainWindow::OnLoadNode()
{
	fs::path node_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing node",
		"nodes",
		ver::node_filter.c_str()
	).toStdString() };

	if (node_path.empty()) return;
	node_path = node_path.make_preferred();
	auto& cs = tab->LoadTab<EditorTab>(std::move(node_path), node_path.filename().string(), property_dock);
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
