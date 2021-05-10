#pragma once
#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <UI/Frameless.h>
#include <Logic/ProjectsData.h>
#include <qpushbutton.h>
#include <QFileDialog>
#include <QApplication>
#include <UI/ProjectEvent.h>


class ProjectsCW : public QWidget
{
public:
	ProjectsCW(QWidget* parent)
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
public:
	void Restore() { OnRestoreClicked(false); }
	QVBoxLayout& Layout()noexcept { return lay; }
protected:
	void OnCloseClicked(bool checked)
	{
		parentWidget()->close();
	}
	void OnMaximizedClicked(bool checked)
	{
		maximize.setIcon(QIcon{ ":/icon_window_restore.png" });
		connect(&maximize, &QToolButton::clicked, this, &ProjectsCW::OnRestoreClicked);
		parentWidget()->showMaximized();
	}
	void OnRestoreClicked(bool checked)
	{
		maximize.setIcon(QIcon{ ":/icon_window_maximize.png" });
		connect(&maximize, &QToolButton::clicked, this, &ProjectsCW::OnMaximizedClicked);
		parentWidget()->showNormal();
	}
	void OnMinimizedClicked(bool checked)
	{
		parentWidget()->showMinimized();
	}
private:
	QVBoxLayout lay;
	QHBoxLayout buttons;
	QToolButton close;
	QToolButton minimize;
	QToolButton maximize;

	static constexpr const char* style = R"(QToolButton{
		background-color:none;
		border:none;
		width:25px;
		height:16px;
		padding:4px;
	}
	QToolButton:hover{
		background-color:gray;
	}
	QToolButton:pressed{
		background-color:palette(highlight);
	})";
	static constexpr const char* closestyle = R"(QToolButton{
		background-color:none;
		border:none;
		width:25px;
		height:16px;
		padding:4px;
	}
	QToolButton:hover{
		background-color:red;
	}
	QToolButton:pressed{
		background-color:palette(highlight);
	})";
};


class ProjectsWindow : public QMainWindow
{
	struct Project : public QWidget
	{
		Project(const std::filesystem::path& xpath)
			:filename(xpath.filename().string().c_str())
			, path(xpath.parent_path().string().c_str())
		{
			QFont f{ "Arial", 12 };
			f.setBold(true);
			filename.setFont(f);
			ico.setPixmap(QPixmap{ ":/tlr.ico" }.scaled({32,32},Qt::IgnoreAspectRatio,Qt::TransformationMode::SmoothTransformation));
			hlay.setAlignment(Qt::AlignLeft);
			hlay.addWidget(&ico);
			vlay.addWidget(&filename);
			vlay.addWidget(&path);
			hlay.addLayout(&vlay);
			setLayout(&hlay);
		}
		QHBoxLayout hlay;
		QVBoxLayout vlay;

		QLabel ico;
		QLabel filename;
		QLabel path;
	};
	struct XButton : public QToolButton
	{
		XButton(std::string_view head, std::string_view descr = "", std::string_view xicon = "")
			:header(head.data()), desc(descr.data())
		{
			setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
			setStyleSheet(style);
			lay.setAlignment(Qt::AlignLeft);
			vlay.setAlignment(Qt::AlignTop);


			if(!xicon.empty())icon.setPixmap({ xicon.data() });
			lay.addWidget(&icon);

			QFont f{ "Arial", 12 };
			f.setBold(true);
			header.setFont(f);
			desc.setFont({ "Arial", 10});

			vlay.addWidget(&header);
			vlay.addWidget(&desc);

			lay.addLayout(&vlay);
			setLayout(&lay);
		}
		QHBoxLayout lay;
		QVBoxLayout vlay;
		QLabel icon;
		QLabel header;
		QLabel desc;
		static constexpr const char* style = R"(QToolButton{
				background-color:#292929;
				border:none;
				height:60px;
				padding:4px;
			}
			QToolButton:hover{
				background-color:gray;
			}
			QToolButton:pressed{
				background-color:palette(highlight);
			})";
	};
public:
	ProjectsWindow(int32_t width, int32_t height, QObject &app)
		:QMainWindow(nullptr, Qt::FramelessWindowHint),
		window(this), f(this)
		, name("Veritas Texture Flow")
		, recent("Open Recent")
		, begin("Start now")
		, create("Create New Project", "Create a new project file with specified\nresolution of the output texture", ":/icons8-add-file.png")
		, open("Open Existing Project", "Open existing project from any location", ":/icons8-opened-folder.png"),
		app(app)
	{
		resize(width, height);
		setCentralWidget(&window);
		window.Layout().addLayout(&lay);
		lay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
		buttonLay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
		name.setSizePolicy({ QSizePolicy::Expanding ,QSizePolicy::Preferred });
		name.setFont({ "Arial", 24 });
		lay.setContentsMargins({ 40,0,40,0 });
		lay.addWidget(&name);

		hlay.setContentsMargins({ 0,20,0,20 });
		hlay.addLayout(&innerLay, 2);
		hlay.addLayout(&buttonLay, 1);

		recent.setFont({ "Arial", 16 });
		innerLay.addWidget(&recent);
		search.setPlaceholderText("Search...");
		search.setFixedHeight(30);
		search.setClearButtonEnabled(true);
		innerLay.addWidget(&search);

		FillTree();
		selection.setHeaderHidden(true);
		search.connect(&search, &QLineEdit::textEdited, this, &ProjectsWindow::OnFilterChanged);
		innerLay.addWidget(&selection);

		begin.setFont({ "Arial", 16 });
		buttonLay.addWidget(&begin);
		buttonLay.setContentsMargins({ 20,0,0,50 });
		buttonLay.addWidget(&create);
		buttonLay.addWidget(&open);

		connect(&create, &QToolButton::clicked, this, &ProjectsWindow::OnCreateClicked);
		connect(&open, &QToolButton::clicked, this, &ProjectsWindow::OnOpenClicked);
		connect(&selection, &QTreeWidget::itemDoubleClicked, this, &ProjectsWindow::OnItemDoubleClicked);

		lay.addLayout(&hlay);
	}
	void mouseMoveEvent(QMouseEvent* e)override
	{
		if (isMaximized())
		{
			window.Restore();
			move(e->globalPos() / 2);
			return;
		}
		f.MouseMove(e);
	}
protected:
	void OnCreateClicked(bool checked) {
		auto projName = QFileDialog::getSaveFileName(
			nullptr,
			"Create new project",
			"",
			"(*.vtproj);;"
		).toStdString();
		if (projName.empty()) return;

		std::filesystem::path projPath{projName};

		if (!pdata.InCache(projName)) {
			pdata.AppendCache(projName);
		}

		OpenApp(projPath);
	}
	void OnOpenClicked(bool checked) {
		auto projName = QFileDialog::getOpenFileName(
			nullptr,
			"Open existing project",
			"",
			"(*.vtproj);;"
		).toStdString();
		if (projName.empty()) return;

		std::filesystem::path projPath{projName};

		if (!pdata.InCache(projPath)) {
			pdata.AppendCache(projName);
		}

		OpenApp(projPath);
	}
	void OpenApp(std::filesystem::path projPath) {
		qApp->postEvent(&app, new ProjectEvent(projPath));
	}
private:
	void FillTree()
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
	void OnFilterChanged(const QString& text)
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
	void OnItemDoubleClicked(QTreeWidgetItem*item, int column) {
		OpenApp(std::filesystem::path{item->data(1, Qt::UserRole).toString().toStdString()});
	}

private:
	QObject &app;
	ProjectsCW window;
	FrameLess f;

	QVBoxLayout lay;
	QHBoxLayout hlay;
	QVBoxLayout innerLay;
	QVBoxLayout buttonLay;

	QLabel name;
	QLabel recent;
	QLineEdit search;
	QTreeWidget selection;

	QLabel begin;
	XButton create;
	XButton open;
	ProjectsData pdata;
};
