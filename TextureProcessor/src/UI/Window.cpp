#include <UI/Window.h>


Window::Window(int32_t width, int32_t height/*, std::fstream&& project*/)
	:file("File")
	, Aclear("Clear")
	, windows("Windows")
	, Aprops("Properties")
	, Aexport("Export All")
	, Asave("Save")
	, Aload("Load")
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
	connect(&Aload, &QAction::triggered, this, &Window::OnLoad);
	file.addAction(&Aclear);
	file.addAction(&Aload);
	file.addAction(&Asave);
	file.addAction(&Aexport);
	windows.addAction(&Aprops);

	a->scene.setSceneRect(-32000, -32000, 64000, 64000);

	setCentralWidget(&a->view);
	addDockWidget(Qt::RightDockWidgetArea, &a->props);
}

void Window::OnClearTriggered()
{
	a->scene.Clear();
}
void Window::OnProps()
{
	a->props.show();
}
void Window::OnExport()
{
	a->scene.ExportAll();
}
void Window::OnLoad()
{
	std::fstream t;
	t.open("s.json", std::ios::in);

	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return; }
	a->scene.Deserialize(json.object());
}
void Window::OnSave()
{
	std::fstream f;
	f.open("s.json", std::ios::out);
	QJsonDocument doc{ a->scene.Serialize() };
	f << doc.toJson().constData();
}