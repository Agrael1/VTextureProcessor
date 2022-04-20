#include <Projects/CreatePage.h>
#include <QFileDialog>
#include <QApplication>
#include <filesystem>
#include <utils/utils.h>

#include <UI/ProjectEvent.h>

namespace fs = std::filesystem;

UI::CreatePage::CreatePage(QWidget* app)
	: name(u"New Project"_qs)
	, project_name(u"Project Name"_qs)
	, project_folder(u"Project Folder"_qs)
	, search(u"..."_qs)
	, cancel(u"Cancel"_qs), create(u"Create"_qs), parent(app)
	, tex_size(u"Texture Dimensions"_qs)
	, lock(locked, "")
{
	lay.setAlignment(Qt::AlignLeft);
	name.setSizePolicy({ QSizePolicy::Expanding ,QSizePolicy::Preferred });
	name.setFont({ u"Arial"_qs, 24 });
	lay.setContentsMargins( 40,0,40,30 );
	lay.addWidget(&name);
	lay.setSpacing(10);

	QFont font = { u"Arial"_qs, 12 };
	QFont rfont = { u"Arial"_qs, 16 };

	project_name.setFont(rfont);

	pname.setFont(font);
	pname.setTextMargins(2, 2, 2, 2);
	lay.addWidget(&project_name);
	lay.addWidget(&pname);

	search.setMaximumWidth(40);
	project_folder.setFont(rfont);
	lay.addWidget(&project_folder);
	fl.addWidget(&pfolder);
	fl.addWidget(&search);

	auto xfolder = cfg.GetProjFolder();
	pfolder.setFont(font);
	pfolder.setTextMargins(2, 2, 2, 2);
	pfolder.setText(QString::fromStdU16String(std::filesystem::absolute(xfolder).u16string()));
	search.setSizePolicy({ QSizePolicy::Preferred ,QSizePolicy::Preferred });
	lay.addLayout(&fl);

	auto p = ver::generate(fs::path(xfolder) / proj_def_name.c_str(),"");
	pname.setText(QString::fromStdU16String(p.stem().u16string()));

	// texture dims
	tex_size.setFont(rfont);
	xwidth.setFont(font);
	xwidth.setValidator(&size_v);
	xwidth.setText(size_std_s);
	xheight.setFont(font);
	xheight.setValidator(&size_v);
	xheight.setText(size_std_s);


	lock.setCheckable(true);
	lock.setSizePolicy({ QSizePolicy::Preferred ,QSizePolicy::Preferred });
	lock.setStyleSheet(u"QPushButton { background-color: transparent; border: 0px }"_qs);
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
	fs::path p{ pfolder.text().toStdU16String() };
	p = (p / pname.text().toStdU16String()).replace_extension(ver::proj_ext.c_str());
	QApplication::postEvent(parent, new ProjectEvent(std::move(p), { xwidth.text().toInt(), xheight.text().toInt() }));
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
	QString dir = QFileDialog::getExistingDirectory(this, u"Open Directory"_qs,
		cfg.GetQProjFolder(),
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty())
		pfolder.setText(dir);
}

void UI::CreatePage::OnWidthEChanged()
{
	auto x = xwidth.text().toInt();
	if (x < size_min)
		xwidth.setText(size_min_s);
}
void UI::CreatePage::OnHeightEChanged()
{
	auto x = xheight.text().toInt();
	if (x < size_min)
		xheight.setText(size_min_s);
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
