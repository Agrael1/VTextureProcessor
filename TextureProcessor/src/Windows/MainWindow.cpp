/**
 * @file Window.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Handles menubar actions
 */

#include <Windows/MainWindow.h>
#include <Windows/SceneTab.h>
#include <Windows/EditorTab.h>
#include <QFileDialog>
#include <QMenuBar>

#include <Logic/Constants.h>
#include <Logic/Engine.h>

#include <Projects/CreatePage.h>
#include <Projects/Frameless.h>

namespace fs = std::filesystem;
using namespace UI::Windows;


class CreationDialog : public QDialog
{
public:
	CreationDialog()
		:fw(this)
	{
		vl.addWidget(&cp);
		setLayout(&vl);
	}
public:
	void mouseMoveEvent(QMouseEvent* e)override
	{
		//if (isMaximized())
		//{
		//	window.Restore();
		//	move(e->globalPos() / 2);
		//	return;
		//}
		fw.MouseMove(e);
	}
private:
	QVBoxLayout vl;
	UI::CreatePage cp;
	UI::FrameLess fw;
};

/**
 * @brief Creates a main application window
 * @param width of a window
 * @param height of a window
 * @param xprojPath file project that is being worked upon
*/
MainWindow::MainWindow(int32_t width, int32_t height, ProjectDescriptor desc)
	:file(u"File"_qs)
	, windows(u"Windows"_qs)
	, nodes(u"Nodes"_qs)
	, view(u"View"_qs)
{
	Engine::Instance();
	resize(width, height);
	tab.emplace(this);

	auto& mb = *menuBar();
	mb.addMenu(&file);
	mb.addMenu(&view);
	mb.addMenu(&nodes);
	mb.addMenu(&windows);

	auto save = [this]() {tab->RequestActive(UI::Request::Save); };
	auto saveas = [this]() {tab->RequestActive(UI::Request::SaveAs); };
	auto load = [this]() { OnLoad(); };
	auto xnew = [this]() { OnNew(); };

	file.addAction("New", xnew, { QKeySequence::StandardKey::New });
	file.addAction("Load", load, { QKeySequence::StandardKey::Open });
	file.addAction("Save", save, { QKeySequence::StandardKey::Save });
	file.addAction("Save As", saveas, { tr("Ctrl+Shift+S") });
	file.addSeparator();
	file.addAction("Clear", [this]() { tab->RequestActive(UI::Request::Clear); });
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
	toolbar.addAction(QIcon(":/save.png"), "Save", save);
	toolbar.addAction(QIcon(":/saveas.png"), "Save As", saveas);
	toolbar.addAction(QIcon(":/open.png"), "Open Project", load);
	toolbar.addSeparator();
	toolbar.addAction(QIcon(":/build.png"), "Compile", [this]() {tab->RequestActive(UI::Request::Compile); });

	tab->LoadTab<SceneTab>(std::move(desc.project_path), desc.project_path.filename().string().c_str(), property_dock, desc.pixel_size);
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
		ver::proj_filter
	).toStdString() };

	if (proj_path.empty()) return;
	proj_path = proj_path.make_preferred();
	tab->LoadTab<SceneTab>(std::move(proj_path), proj_path.filename().string().c_str(), property_dock, QSize(256, 256));
}

void MainWindow::OnNew()
{
	CreationDialog* a = new CreationDialog;
	a->exec();
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
		ver::node_filter
	).toStdString() };

	if (node_path.empty()) return;
	node_path = node_path.make_preferred();
	tab->LoadTab<EditorTab>(std::move(node_path), node_path.filename().string().c_str(), property_dock);
}

