/**
 * @file ProjectsWindow.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Node header color specification
 */

#include <Windows/ProjectsWindow.h>
#include <UI/ProjectEvent.h>
#include <Logic/ApplicationConfig.h>
#include <utils/utils.h>

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
MainPage::MainPage(QWidget* xparent, ProjectsData& pdata)
	:parent(xparent), pdata(pdata)
	, name("Veritas Texture Flow")
	, recent("Open Recent")
	, begin("Start now")
	, create("Create New Project", "Create a new project file with specified\nresolution of the output texture", ":/icons8-add-file.png")
	, open("Open Existing Project", "Open existing project from any location", ":/icons8-opened-folder.png")
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
	QApplication::postEvent(parent, new NewProjEvent());
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
		ver::proj_filter.data()
	).toStdString() };

	if (proj_path.empty()) return;
	QApplication::postEvent(parent, new ProjectEvent(std::move(proj_path), {}));
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
	QApplication::postEvent(parent, new ProjectEvent(std::move(file), {}));
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

ProjectsWindow::ProjectsWindow(int32_t width, int32_t height, QObject& app, ApplicationConfig& cfg)
	:QMainWindow(nullptr, Qt::FramelessWindowHint),
	window(this), f(this), mp(this, pdata), app(app), cp(this, cfg)
{
	resize(width, height);
	setCentralWidget(&window);
	window.Layout().addWidget(&mp);
	window.Layout().addWidget(&cp);
	cp.hide();
	QEvent::registerEventType(QEvent::User + 2);
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

void ProjectsWindow::customEvent(QEvent* e)
{
	// Handling of opening projects

	if (e->type() == QEvent::User + 1)
	{
		auto& x = static_cast<ProjectEvent&>(*e);
		const auto& proj_path = x.projPath;
		if (!pdata.contains(proj_path))
			pdata.AppendCache(proj_path);

		//create ifnot exists
		if (!fs::exists(proj_path))
			std::ofstream{ proj_path };
		QApplication::postEvent(&app, new ProjectEvent(std::move(x.projPath), x.size));
		return;
	}
	if (e->type() == QEvent::User + 2) {
		NewProjEvent& proj = static_cast<NewProjEvent&>(*e);
		switch (proj.ty)
		{
		case NewProjEvent::Type::Forward:
			mp.hide();
			cp.show();
			break;
		case NewProjEvent::Type::Back:
			cp.hide();
			mp.show();
			break;
		default:
			break;
		}
		return;
	}
}

UI::CreatePage::CreatePage(QWidget* app, ApplicationConfig& cfg)
	: cfg(cfg), name("New Project")
	, project_name("Project Name")
	, project_folder("Project Folder")
	, search("...")
	, cancel("Cancel"), create("Create"), parent(app)
	, tex_size("Texture Dimensions")
	, lock(locked,"")
{
	lay.setAlignment(Qt::AlignLeft);
	name.setSizePolicy({ QSizePolicy::Expanding ,QSizePolicy::Preferred });
	name.setFont({ "Arial", 24 });
	lay.setContentsMargins({ 40,0,40,30 });
	lay.addWidget(&name);
	lay.addSpacing(30);
	lay.setSpacing(10);

	QFont font = { "Arial", 12 };
	QFont rfont = { "Arial", 16 };

	project_name.setFont(rfont);

	pname.setFont(font);
	pname.setTextMargins(2, 2, 2, 2);
	lay.addWidget(&project_name);
	lay.addWidget(&pname);
	lay.addSpacing(30);

	search.setMaximumWidth(40);
	project_folder.setFont(rfont);
	lay.addWidget(&project_folder);
	fl.addWidget(&pfolder);
	fl.addWidget(&search);

	auto xfolder = cfg.GetProjFolder();
	pfolder.setFont(font);
	pfolder.setTextMargins(2, 2, 2, 2);
	pfolder.setText(std::filesystem::absolute(xfolder).string().c_str());
	search.setSizePolicy({ QSizePolicy::Preferred ,QSizePolicy::Preferred });
	lay.addLayout(&fl);

	auto p = ver::generate(fs::path(xfolder) / proj_def_name.data());
	pname.setText(p.stem().string().c_str());

	lay.addSpacing(30);
	// texture dims
	tex_size.setFont(rfont);
	xwidth.setFont(font);
	xwidth.setValidator(&size_v);
	xwidth.setText(size_std_s.c_str());
	xheight.setFont(font);
	xheight.setValidator(&size_v);
	xheight.setText(size_std_s.c_str());


	lock.setCheckable(true);
	lock.setSizePolicy({ QSizePolicy::Preferred ,QSizePolicy::Preferred });
	lock.setStyleSheet("QPushButton { background-color: transparent; border: 0px }");
	szl.addWidget(&xwidth);
	szl.addWidget(&lock);
	szl.addWidget(&xheight);
	szl.addStretch(2);
	szl.setSpacing(0);
	lay.addWidget(&tex_size);
	lay.addLayout(&szl);

	lay.addStretch(1);

	cancel.setFixedHeight(25);
	create.setFixedHeight(25);
	bl.addWidget(&cancel);
	bl.addWidget(&create);
	bl.setAlignment(Qt::AlignRight | Qt::AlignBottom);
	lay.addLayout(&bl);


	connect(&cancel, &QPushButton::clicked, this, &CreatePage::OnCancelClicked);
	connect(&create, &QPushButton::clicked, this, &CreatePage::OnCreateClicked);
	connect(&lock, &QPushButton::clicked, this, &CreatePage::OnLink);
	connect(&search, &QPushButton::clicked, this, &CreatePage::OnFolderChange);
	connect(&xwidth, &QLineEdit::textChanged, this, &CreatePage::OnWidthChanged);
	connect(&xheight, &QLineEdit::textChanged, this, &CreatePage::OnHeightChanged);
	connect(&xwidth, &QLineEdit::editingFinished, this, &CreatePage::OnWidthEChanged);
	connect(&xheight, &QLineEdit::editingFinished, this, &CreatePage::OnHeightEChanged);

	setLayout(&lay);
}

void UI::CreatePage::OnCreateClicked(bool checked)
{
	fs::path p{ pfolder.text().toStdString() };
	p = (p / pname.text().toStdString()).replace_extension(ver::proj_ext.c_str());
	QApplication::postEvent(parent, new ProjectEvent(std::move(p), {xwidth.text().toInt(), xheight.text().toInt() }));
}

void UI::CreatePage::OnCancelClicked(bool checked)
{
	QApplication::postEvent(parent, new NewProjEvent(NewProjEvent::Type::Back));
}

void UI::CreatePage::OnLink(bool checked)
{
	if (checked)
	{
		lock.setIcon(unlocked);
		return;
	}
	lock.setIcon(locked);
}

void UI::CreatePage::OnFolderChange(bool checked)
{
	QString dir = QFileDialog::getExistingDirectory(this, "Open Directory",
		cfg.GetProjFolder().data(),
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if(!dir.isEmpty())
		pfolder.setText(dir);
}

void UI::CreatePage::OnWidthEChanged()
{
	auto x = xwidth.text().toInt();
	if (x < size_min)
		xwidth.setText(size_min_s.c_str());
}
void UI::CreatePage::OnHeightEChanged()
{
	auto x = xheight.text().toInt();
	if (x < size_min)
		xheight.setText(size_min_s.c_str());
}

void UI::CreatePage::OnWidthChanged(const QString& text)
{
	if (!lock.isChecked())
		xheight.setText(xwidth.text());
}

void UI::CreatePage::OnHeightChanged(const QString& text)
{
	if (!lock.isChecked())
		xwidth.setText(xheight.text());
}
