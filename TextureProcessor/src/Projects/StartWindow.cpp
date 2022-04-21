#include <Projects/StartWindow.h>
#include <QMessageBox>

using namespace UI;
namespace fs = std::filesystem;

/**
 * @brief Creates a path subwidget for tree view
 * @param xpath path to suggest
*/
MainPage::Project::Project(const std::filesystem::path& xpath)
	:filename(QString::fromStdU16String(xpath.filename().u16string()))
	, path(QString::fromStdU16String(xpath.parent_path().u16string()))
{
	QFont f{ u"Arial"_qs, 12 };
	f.setBold(true);
	filename.setFont(f);
	ico.setPixmap(QPixmap{ u":/tlr.ico"_qs }.scaled({ 32,32 }, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
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
MainPage::XButton::XButton(const QString& head, const QString& descr, const QString& xicon)
	:header(head), desc(descr)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	setStyleSheet(style);
	lay.setAlignment(Qt::AlignLeft);
	vlay.setAlignment(Qt::AlignTop);


	if (!xicon.isEmpty())icon.setPixmap({ xicon });
	lay.addWidget(&icon);

	QFont f{ u"Arial"_qs, 12 };
	f.setBold(true);
	header.setFont(f);
	desc.setFont({ u"Arial"_qs, 10 });

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
MainPage::MainPage()
	: name(ver::app_name)
	, recent(u"Open Recent"_qs)
	, begin(u"Start now"_qs)
	, create(u"Create New Project"_qs, u"Create a new project file with specified\nresolution of the output texture"_qs, u":/icons8-add-file.png"_qs)
	, open(u"Open Existing Project"_qs, u"Open existing project from any location"_qs, u":/icons8-opened-folder.png"_qs)
{
	setLayout(&lay);
	lay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	buttonLay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	name.setSizePolicy({ QSizePolicy::Expanding ,QSizePolicy::Preferred });
	name.setFont({ u"Arial"_qs, 24 });
	lay.setContentsMargins({ 40,0,40,0 });
	lay.addWidget(&name);

	hlay.setContentsMargins({ 0,20,0,20 });
	hlay.addLayout(&innerLay, 2);
	hlay.addLayout(&buttonLay, 1);

	// Filtering
	recent.setFont({ u"Arial"_qs, 16 });
	innerLay.addWidget(&recent);
	search.setPlaceholderText(u"Search..."_qs);
	search.setFixedHeight(30);
	search.setClearButtonEnabled(true);
	innerLay.addWidget(&search);

	// Show all recent projects from project cache
	FillTree();
	selection.setHeaderHidden(true);
	search.connect(&search, &QLineEdit::textEdited, this, &MainPage::OnFilterChanged);
	innerLay.addWidget(&selection);

	begin.setFont({ u"Arial"_qs, 16 });
	buttonLay.addWidget(&begin);
	buttonLay.setContentsMargins({ 20,0,0,50 });
	buttonLay.addWidget(&create);
	buttonLay.addWidget(&open);

	connect(&create, &QToolButton::clicked, this, &MainPage::OnCreateClicked);
	connect(&open, &QToolButton::clicked, this, &MainPage::OnOpenClicked);
	connect(&selection, &QTreeWidget::itemDoubleClicked, this, &MainPage::OnItemDoubleClicked);

	lay.addLayout(&hlay);
}

void UI::MainPage::AddProject(const std::filesystem::path& xpath)
{
	if (!pdata.contains(xpath))
		pdata.AppendCache(xpath);
}

/**
 * @brief On create new project button clicked callback
 * @param checked [unused]
*/
void MainPage::OnCreateClicked(bool checked)
{
	emit CreateClicked();
}



/**
 * @brief On open existing project button clicked callback
 * @param checked [unused]
*/
void MainPage::OnOpenClicked(bool checked)
{
	fs::path proj_path{ QFileDialog::getOpenFileName(
		nullptr,
		u"Open existing project"_qs,
		"",
		ver::proj_filter
	).toStdU16String() };

	if (proj_path.empty()) return;
	AddProject(proj_path);
	emit OpenProject({ std::move(proj_path), {} });
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
		xitem.setData(0, Qt::UserRole, QString::fromStdU16String(x.filename().u16string()));
		xitem.setData(1, Qt::UserRole, QString::fromStdU16String(x.u16string()));
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
	std::filesystem::path file{ item->data(1, Qt::UserRole).toString().toStdU16String() };

	// Non-existing file warning
	if (!fs::exists(file) || !fs::is_regular_file(file))
	{
		QMessageBox m(QMessageBox::Icon::Warning, u"Warning"_qs, u"Specified file does not exist\nDo you want to delete the entry?"_qs,
			QMessageBox::Ok | QMessageBox::Cancel);
		if (m.exec() == QMessageBox::Ok)
		{
			pdata.erase(file);
			FillTree();
		}
		return;
	}
	emit OpenProject({ std::move(file), {} });
}
