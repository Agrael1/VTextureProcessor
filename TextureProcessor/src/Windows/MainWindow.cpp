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
	tab.emplace(this);

	auto& mb = *menuBar();
	mb.addMenu(&file);
	mb.addMenu(&view);
	mb.addMenu(&nodes);
	mb.addMenu(&windows);

	file.addAction("Clear", [this]() { tab->RequestActive(UI::Request::Clear); });
	file.addAction("Load", [this]() { OnLoad(); });
	file.addAction("Save", [this]() {tab->RequestActive(UI::Request::Save); }, { QKeySequence::StandardKey::Save });
	file.addAction("Save As", [this]() {tab->RequestActive(UI::Request::SaveAs); }, { tr("Ctrl+Shift+S") });
	file.addSeparator();
	file.addAction("Export", [this]() {tab->RequestActive(UI::Request::Export); });

	windows.addAction("Properties", [this]() { OnProps(); });

	nodes.addAction("Create", [this]() {OnCreateNode(); }, { tr("Ctrl+Shift+N") });
	nodes.addAction("Load", [this]() {OnLoadNode(); }, { tr("Ctrl+Shift+O") });

	view.addAction("Delete", [this]() {tab->RequestActive(UI::Request::Delete); }, { QKeySequence::StandardKey::Delete });
	view.addAction("Clear Selection", [this]() {tab->RequestActive(UI::Request::ClearSel); });


	setCentralWidget(&*tab);
	addDockWidget(Qt::RightDockWidgetArea, &property_dock);
	resizeDocks({ &property_dock }, { 260 }, Qt::Horizontal);

	auto& toolbar = *addToolBar("");
	toolbar.setIconSize({ 16,16 });
	toolbar.addAction(QIcon(":/build.png"), "Compile", [this]() {tab->RequestActive(UI::Request::Compile); });

	tab->LoadTab<SceneTab>(std::move(xprojPath), xprojPath.filename().string().c_str(), property_dock, resolution);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	tab.reset();
	Engine::Destroy();
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
	tab->LoadTab<SceneTab>(std::move(proj_path), proj_path.filename().string().c_str(), property_dock, QSize(256, 256));
}

void MainWindow::OnCreateNode()
{
	tab->LoadTab<EditorTab>({}, QStringLiteral("New Node"), property_dock);
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
	tab->LoadTab<EditorTab>(std::move(node_path), node_path.filename().string().c_str(), property_dock);
}

