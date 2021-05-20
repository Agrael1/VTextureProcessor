/**
 * @file Window.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Handles menubar actions
 */

#include <UI/Window.h>
#include <QFileDialog>

namespace fs = std::filesystem;

/**
 * @brief Creates a main application window
 * @param width of a window
 * @param height of a window
 * @param xprojPath file project that is being worked upon
*/
Window::Window(int32_t width, int32_t height, std::filesystem::path&& xprojPath)
	:file("File")
	, Aclear("Clear")
	, windows("Windows")
	, Aprops("Properties")
	, Aexport("Export All")
	, Asave("Save")
	, Aedit("Editor")
	, Asaveas("Save As")
	, Aload("Load"),
	projPath(std::move(xprojPath))
{
	a.emplace(this);

	resize(width, height);
	auto& mb = *menuBar();
	mb.addMenu(&file);
	a->view.AppendViewMenu(mb);
	mb.addMenu(&windows);

	connect(&Aclear, &QAction::triggered, this, &Window::OnClearTriggered);
	connect(&Aprops, &QAction::triggered, this, &Window::OnProps);
	connect(&Aexport, &QAction::triggered, this, &Window::OnExport);
	connect(&Asave, &QAction::triggered, this, &Window::OnSave);
	connect(&Asaveas, &QAction::triggered, this, &Window::OnSaveAs);
	connect(&Aload, &QAction::triggered, this, &Window::OnLoad);
	connect(&Aedit, &QAction::triggered, this, &Window::OnEdit);
	file.addAction(&Aclear);
	file.addAction(&Aedit);
	file.addAction(&Aload);
	file.addAction(&Asave);
	file.addAction(&Asaveas);
	file.addSeparator();
	file.addAction(&Aexport);
	windows.addAction(&Aprops);

	Asave.setShortcut(QKeySequence{ QKeySequence::StandardKey::Save });
	Asaveas.setShortcut(QKeySequence{ tr("Ctrl+Shift+S") });
	Aload.setShortcut(QKeySequence{ QKeySequence::StandardKey::Open });

	a->scene.setSceneRect(-32000, -32000, 64000, 64000);

	if (!projPath.empty())
		LoadFile();

	setCentralWidget(&a->tab);
	addDockWidget(Qt::RightDockWidgetArea, &a->props);
}

/**
 * @brief clear menu button
*/
void Window::OnClearTriggered()
{
	a->scene.Clear();
}

/**
 * @brief Windows->Properties
*/
void Window::OnProps()
{
	a->props.show();
}

/**
 * @brief File->Export
*/
void Window::OnExport()
{
	a->scene.ExportAll();
}

/**
 * @brief File->Load
*/
void Window::OnLoad()
{
	fs::path proj_path{ QFileDialog::getOpenFileName(
		nullptr,
		"Open existing project",
		"",
		"(*.vtproj);;"
		).toStdString() };

	if (proj_path.empty()) return;
	projPath = std::move(proj_path);
	LoadFile();
}

void Window::OnEdit()
{
	a->tab.addTab(&a->edits.emplace_back(), "New Node");
	a->tab.setCurrentIndex(a->edits.size());
}

/**
 * @brief Loads specified file
*/
void Window::LoadFile()
{
	using namespace std::string_literals;
	a->scene.Clear();
	std::fstream t;
	t.open(projPath, std::ios::in);

	setWindowTitle((AppName.data() + " - "s + projPath.filename().string()).c_str());

	std::string str;
	t.seekg(0, std::ios::end);

	//preallocation
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return; }
	a->scene.Deserialize(json.object());
}

/**
 * @brief file->Save as
*/
void Window::OnSaveAs()
{
	fs::path proj_path{ QFileDialog::getSaveFileName(
		nullptr,
		"Create new project",
		"",
		"(*.vtproj);;"
	).toStdString() };

	if (proj_path.empty()) return;

	if (!proj_path.has_extension()) {
		proj_path.replace_extension(".vtproj");
	}

	std::fstream f;
	f.open(proj_path, std::ios::out);
	if (!f.is_open()) return;
	QJsonDocument doc{ a->scene.Serialize() };
	f << doc.toJson().constData();
}

/**
 * @brief File->save
*/
void Window::OnSave()
{
	std::fstream f;
	f.open(projPath, std::ios::out);
	QJsonDocument doc{ a->scene.Serialize() };
	f << doc.toJson().constData();
}
