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
#include <Projects/FrameControl.h>

namespace fs = std::filesystem;
using namespace UI::Windows;


class CreationDialog : public QDialog
{
public:
	CreationDialog()
		:fw(this), window(this)
	{
		resize(640, 320);
		window.Layout().addWidget(&cp);
		setLayout(&window.Layout());
		connect(&cp, &UI::CreatePage::CancelClicked, this, &QWidget::close);
	}
public:
	void mouseMoveEvent(QMouseEvent* e)override
	{
		fw.MouseMove(e);
	}
	auto& Page()
	{
		return cp;
	}
private:
	UI::Internal::FrameControl window;
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
	, project(u"Project"_qs)
	, nodes(u"Nodes"_qs)
	, view(u"View"_qs)
{
	Engine::Instance();
	resize(width, height);
	tab.emplace(this);

	auto& mb = *menuBar();
	mb.addMenu(&file);
	mb.addMenu(&view);
	mb.addMenu(&project);
	mb.addMenu(&nodes);
	mb.addMenu(&windows);

	auto save = [this]() {tab->RequestActive(UI::Request::Save); };
	auto saveas = [this]() {tab->RequestActive(UI::Request::SaveAs); };
	auto load = [this]() { OnLoad(); };
	auto xnew = [this]() { OnNew(); };
	auto compile = [this]() {tab->RequestActive(UI::Request::Compile); };

	file.addAction(u"New"_qs, xnew, { QKeySequence::StandardKey::New });
	file.addAction(u"Load"_qs, load, { QKeySequence::StandardKey::Open });
	file.addAction(u"Save"_qs, save, { QKeySequence::StandardKey::Save });
	file.addAction(u"Save As"_qs, saveas, { u"Ctrl+Shift+S"_qs });
	file.addSeparator();
	file.addAction(u"Clear"_qs, [this]() { tab->RequestActive(UI::Request::Clear); });
	file.addAction(u"Export"_qs, [this]() {tab->RequestActive(UI::Request::Export); });

	windows.addAction(u"Properties"_qs, [this]() { OnProps(); });
	
	project.addAction(u"Compile"_qs, compile, { u"F5"_qs});

	nodes.addAction(u"Create"_qs, [this]() {OnCreateNode(); }, { u"Ctrl+Shift+N"_qs });
	nodes.addAction(u"Load"_qs, [this]() {OnLoadNode(); }, { u"Ctrl+Shift+O"_qs });

	view.addAction(u"Delete"_qs, [this]() {tab->RequestActive(UI::Request::Delete); }, { QKeySequence::StandardKey::Delete });
	view.addAction(u"Clear Selection"_qs, [this]() {tab->RequestActive(UI::Request::ClearSel); });


	setCentralWidget(&*tab);
	addDockWidget(Qt::RightDockWidgetArea, &property_dock);
	resizeDocks({ &property_dock }, { 260 }, Qt::Horizontal);

	auto& toolbar = *addToolBar("");
	toolbar.setIconSize({ 16,16 });
	toolbar.addAction(QIcon(u":/save.png"_qs), u"Save"_qs, save);
	toolbar.addAction(QIcon(u":/saveas.png"_qs), u"Save As"_qs, saveas);
	toolbar.addAction(QIcon(u":/open.png"_qs), u"Open Project"_qs, load);
	toolbar.addSeparator();
	toolbar.addAction(QIcon(u":/build.png"_qs), u"Compile"_qs, compile);

	tab->LoadTab<SceneTab>(std::move(desc.project_path), 
		QString::fromStdU16String(desc.project_path.filename().u16string()), 
		property_dock, desc.pixel_size);
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
		u"Open existing project"_qs,
		"",
		ver::proj_filter
	).toStdString() };

	if (proj_path.empty()) return;
	proj_path = proj_path.make_preferred();
	tab->LoadTab<SceneTab>(std::move(proj_path), 
		QString::fromStdU16String(proj_path.filename().u16string())
		, property_dock, QSize(256, 256));
}

void MainWindow::OnNew()
{
	CreationDialog a;
	auto& pg = a.Page();
	connect(&pg, &UI::CreatePage::CreateClicked, [this, &a](ProjectDescriptor desc) {
		tab->LoadTab<SceneTab>(std::move(desc.project_path), 
			QString::fromStdU16String(desc.project_path.filename().u16string()), 
			property_dock, desc.pixel_size);
		a.close();
		});
	a.exec();
}

void MainWindow::OnCreateNode()
{
	tab->LoadTab<EditorTab>({}, u"New Node"_qs, property_dock);
}
void MainWindow::OnLoadNode()
{
	fs::path node_path{ QFileDialog::getOpenFileName(
		nullptr,
		u"Open existing node"_qs,
		u"nodes"_qs,
		ver::node_filter
	).toStdU16String() };

	if (node_path.empty()) return;
	node_path = node_path.make_preferred();
	tab->LoadTab<EditorTab>(std::move(node_path), 
		QString::fromStdU16String(node_path.filename().u16string()), 
		property_dock);
}

