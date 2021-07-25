/**
 * @file ProjectsWindow.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Node header color specification
 */

#include <Windows/ProjectsWindow.h>
#include <UI/ProjectEvent.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QMouseEvent>

#include <fstream>

using namespace UI;
namespace fs = std::filesystem;

/**
 * @brief Creates a path subwidget for tree view
 * @param xpath path to suggest
*/
MainPage::Project::Project(const std::filesystem::path& xpath)
	:filename(xpath.filename().string().c_str())
	, path(xpath.parent_path().string().c_str())
{
	QFont f{ "Arial", 12 };
	f.setBold(true);
	filename.setFont(f);
	ico.setPixmap(QPixmap{ ":/tlr.ico" }.scaled({ 32,32 }, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	hlay.setAlignment(Qt::AlignLeft);
	hlay.addWidget(&ico);
	vlay.addWidget(&filename);
	vlay.addWidget(&path);
	hlay.addLayout(&vlay);
	setLayout(&hlay);
}

/**
 * @brief Button subwidget for custom window
 * @param head header of the button
 * @param descr description
 * @param xicon path to icon
*/
MainPage::XButton::XButton(std::string_view head, std::string_view descr, std::string_view xicon)
	:header(head.data()), desc(descr.data())
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	setStyleSheet(style);
	lay.setAlignment(Qt::AlignLeft);
	vlay.setAlignment(Qt::AlignTop);


	if (!xicon.empty())icon.setPixmap({ xicon.data() });
	lay.addWidget(&icon);

	QFont f{ "Arial", 12 };
	f.setBold(true);
	header.setFont(f);
	desc.setFont({ "Arial", 10 });

	vlay.addWidget(&header);
	vlay.addWidget(&desc);

	lay.addLayout(&vlay);
	setLayout(&lay);
}


/**
 * @brief Creates custom window for project initialization
 * @param width window width
 * @param height window height
 * @param app tied applicaion to call back to
*/
MainPage::MainPage(QObject& app)
	:name("Veritas Texture Flow")
	, recent("Open Recent")
	, begin("Start now")
	, create("Create New Project", "Create a new project file with specified\nresolution of the output texture", ":/icons8-add-file.png")
	, open("Open Existing Project", "Open existing project from any location", ":/icons8-opened-folder.png"),
	app(app)
{
	setLayout(&lay);
	lay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	buttonLay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	name.setSizePolicy({ QSizePolicy::Expanding ,QSizePolicy::Preferred });
	name.setFont({ "Arial", 24 });
	lay.setContentsMargins({ 40,0,40,0 });
	lay.addWidget(&name);

	hlay.setContentsMargins({ 0,20,0,20 });
	hlay.addLayout(&innerLay, 2);
	hlay.addLayout(&buttonLay, 1);

	// Filtering
	recent.setFont({ "Arial", 16 });
	innerLay.addWidget(&recent);
	search.setPlaceholderText("Search...");
	search.setFixedHeight(30);
	search.setClearButtonEnabled(true);
	innerLay.addWidget(&search);

	// Show all recent projects from project cache
	FillTree();
	selection.setHeaderHidden(true);
	search.connect(&search, &QLineEdit::textEdited, this, &MainPage::OnFilterChanged);
	innerLay.addWidget(&selection);

	begin.setFont({ "Arial", 16 });
	buttonLay.addWidget(&begin);
	buttonLay.setContentsMargins({ 20,0,0,50 });
	buttonLay.addWidget(&create);
	buttonLay.addWidget(&open);

	connect(&create, &QToolButton::clicked, this, &MainPage::OnCreateClicked);
	connect(&open, &QToolButton::clicked, this, &MainPage::OnOpenClicked);
	connect(&selection, &QTreeWidget::itemDoubleClicked, this, &MainPage::OnItemDoubleClicked);

	lay.addLayout(&hlay);
}

/**
 * @brief On create new project button clicked callback
 * @param checked [unused]
*/
void MainPage::OnCreateClicked(bool checked)
{
	fs::path proj_path { QFileDialog::getSaveFileName(
		nullptr,
		"Create new project",
		"",
		"(*.vtproj);;"
	).toStdString() };

	if (proj_path.empty()) return;

	if (!proj_path.has_extension()) {
		proj_path.replace_extension(".vtproj");
	}

	if (!pdata.contains(proj_path))
		pdata.AppendCache(proj_path);

	//create ifnot exists
	if (!fs::exists(proj_path))
		std::ofstream{ proj_path };

	OpenApp(std::move(proj_path));
}

/**
 * @brief On open existing project button clicked callback
 * @param checked [unused]
*/
void MainPage::OnOpenClicked(bool checked)
{
	fs::path proj_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing project",
		"",
		"(*.vtproj);;"
	).toStdString() };

	if (proj_path.empty()) return;

	if (!pdata.contains(proj_path))
		pdata.AppendCache(proj_path);

	OpenApp(std::move(proj_path));
}

void MainPage::OpenApp(std::filesystem::path&& projPath) {
	QApplication::postEvent(&app, new ProjectEvent(std::move(projPath)));
}

/**
 * @brief Fills tree widget with subwidgets
*/
void MainPage::FillTree()
{
	selection.clear();
	for (auto& x : pdata.GetRecent())
	{
		auto& xitem = *new QTreeWidgetItem(&selection);
		selection.setItemWidget(&xitem, 0, new Project(x));
		xitem.setData(0, Qt::UserRole, x.filename().string().c_str());
		xitem.setData(1, Qt::UserRole, x.string().c_str());
	}
}

/**
 * @brief Filtering search text box event callback
 * @param text text written (new filter)
*/
void MainPage::OnFilterChanged(const QString& text)
{
	auto cnt = selection.topLevelItemCount();

	for (int j = 0; j < cnt; j++)
	{
		auto* topLvlItem = selection.topLevelItem(j);
		auto child_count = topLvlItem->childCount();

		auto modelName = topLvlItem->data(0, Qt::UserRole).toString();
		const bool match = (modelName.contains(text, Qt::CaseInsensitive));
		topLvlItem->setHidden(!match);
	}
}

/**
 * @brief Callback on selected item
 * @param item clicked item
 * @param column [unused]
*/
void MainPage::OnItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	std::filesystem::path file{ item->data(1, Qt::UserRole).toString().toStdString() };

	// Non-existing file warning
	if (!fs::exists(file) || !fs::is_regular_file(file))
	{
		QMessageBox m(QMessageBox::Icon::Warning, "Warning", "Specified file does not exist\nDo you want to delete the entry?",
			QMessageBox::Ok | QMessageBox::Cancel);
		if (m.exec() == QMessageBox::Ok)
		{
			pdata.erase(file);
			FillTree();
		}
		return;
	}
	OpenApp(std::move(file));
}

UI::Internal::ProjectsCW::ProjectsCW(QWidget* parent)
	:QWidget(parent)
{
	lay.setAlignment(Qt::AlignTop | Qt::AlignRight);
	setSizePolicy({ QSizePolicy::Maximum ,QSizePolicy::Maximum });

	close.setIcon(QIcon{ ":/icon_window_close.png" });
	maximize.setIcon(QIcon{ ":/icon_window_maximize.png" });
	minimize.setIcon(QIcon{ ":/icon_window_minimize.png" });

	minimize.setStyleSheet(style);
	maximize.setStyleSheet(style);
	close.setStyleSheet(closestyle);

	buttons.setAlignment(Qt::AlignTop | Qt::AlignRight);
	buttons.addWidget(&minimize);
	buttons.addWidget(&maximize);
	buttons.addWidget(&close);

	connect(&close, &QToolButton::clicked, this, &ProjectsCW::OnCloseClicked);
	connect(&minimize, &QToolButton::clicked, this, &ProjectsCW::OnMinimizedClicked);
	connect(&maximize, &QToolButton::clicked, this, &ProjectsCW::OnMaximizedClicked);
	lay.addLayout(&buttons);
	setLayout(&lay);
}

ProjectsWindow::ProjectsWindow(int32_t width, int32_t height, QObject& app)
	:QMainWindow(nullptr, Qt::FramelessWindowHint),
	window(this), f(this), pw(app)
{
	resize(width, height);
	setCentralWidget(&window);
	window.Layout().addWidget(&pw);
}

void ProjectsWindow::mouseMoveEvent(QMouseEvent* e)
{
	if (isMaximized())
	{
		window.Restore();
		move(e->globalPos() / 2);
		return;
	}
	f.MouseMove(e);
}
